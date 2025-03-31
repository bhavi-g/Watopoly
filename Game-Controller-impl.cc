// GameController-impl.cc (implementation)
// Module: GameController
// Description:
//   Implements the core functionality of the mediator in Watopoly.
//   Handles all logic involving relationships between players and buildings.
//   Provides utility methods for adding, retrieving, and trading entities.
//
//   Trade logic enforces token-based ownership validation and ensures
//   buildings and players involved in the trade exist and are correctly owned.
//
//   This is where high-level game logic starts to take shape.

module GameController;
import <random>;
import <ctime>;
import <algorithm>;  // For std::all_of
import <cctype>;     // For ::isdigit
import LandAction;
import Square;

// Registers a Player with the controller using their token as the key.
void GameController::addPlayer(Player* p) {
    players[p->getToken()] = p;
}

// Registers a Building with the controller using its name as the key.
void GameController::addBuilding(Building* b) {
    buildings[b->getName()] = b;
}

// Retrieves a Player pointer by token.
// Returns nullptr if the player does not exist.
Player* GameController::getPlayer(const std::string& token) const {
    auto it = players.find(token);
    return (it != players.end()) ? it->second : nullptr;
}

// Retrieves a Building pointer by name.
// Returns nullptr if the building does not exist.
Building* GameController::getBuilding(const std::string& name) const {
    auto it = buildings.find(name);
    return (it != buildings.end()) ? it->second : nullptr;
}

// Facilitates a trade between two players involving two buildings.
// This may represent a property-for-property or money-for-property trade.
// Only succeeds if:
//   - Both players exist
//   - Both buildings exist
//   - Each player owns the building they are offering
void GameController::trade(const std::string& fromToken, const std::string& giveStr,
                           const std::string& toToken, const std::string& receiveStr) {
    Player* fromPlayer = getPlayer(fromToken);
    Player* toPlayer = getPlayer(toToken);

    if (!fromPlayer || !toPlayer) {
        std::cout << "[Error] Invalid player token.\n";
        return;
    }

    auto isPositiveNumber = [](const std::string& s) {
        return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
    };

    bool giveIsMoney = isPositiveNumber(giveStr);
    bool receiveIsMoney = isPositiveNumber(receiveStr);

    if (giveIsMoney && receiveIsMoney) {
        std::cout << "[Error] Cannot trade money for money.\n";
        return;
    }

    auto* giveBuilding = getBuilding(giveStr);
    if (!giveIsMoney) {
        if (!giveBuilding || giveBuilding->getOwnerToken() != fromToken) {
            std::cout << "[Error] You do not own the property \"" << giveStr << "\" or it doesn't exist.\n";
            return;
        }

        if (auto* ab = dynamic_cast<AcademicBuilding*>(giveBuilding)) {
            std::string block = ab->getMonopolyBlock();
            for (const auto& [_, bldg] : buildings) {
                auto* other = dynamic_cast<AcademicBuilding*>(bldg);
                if (other && other->getMonopolyBlock() == block && other->getImprovementCount() > 0) {
                    std::cout << "[Error] Cannot trade \"" << giveStr << "\" because it or another property in its monopoly has improvements.\n";
                    return;
                }
            }
        }
    }

    auto* receiveBuilding = getBuilding(receiveStr);
    if (!receiveIsMoney) {
        if (!receiveBuilding || receiveBuilding->getOwnerToken() != toToken) {
            std::cout << "[Error] The player \"" << toToken << "\" does not own \"" << receiveStr << "\" or it doesn't exist.\n";
            return;
        }

        if (auto* ab = dynamic_cast<AcademicBuilding*>(receiveBuilding)) {
            std::string block = ab->getMonopolyBlock();
            for (const auto& [_, bldg] : buildings) {
                auto* other = dynamic_cast<AcademicBuilding*>(bldg);
                if (other && other->getMonopolyBlock() == block && other->getImprovementCount() > 0) {
                    std::cout << "[Error] Cannot receive \"" << receiveStr << "\" because it or another property in its monopoly has improvements.\n";
                    return;
                }
            }
        }
    }

    if (giveIsMoney) {
        int amount = std::stoi(giveStr);
        if (fromPlayer->getMoney() < amount) {
            std::cout << "[Error] Not enough funds to offer $" << amount << ".\n";
            return;
        }
    }

    if (receiveIsMoney) {
        int amount = std::stoi(receiveStr);
        if (toPlayer->getMoney() < amount) {
            std::cout << "[Error] " << toPlayer->getName() << " does not have $" << amount << ".\n";
            return;
        }
    }

    std::cout << "[Offer] " << fromPlayer->getName() << " offers ";
    if (giveIsMoney) {
        std::cout << "$" << giveStr;
    } else {
        std::cout << "property " << giveStr;
    }
    std::cout << " in exchange for ";
    if (receiveIsMoney) {
        std::cout << "$" << receiveStr;
    } else {
        std::cout << "property " << receiveStr;
    }
    std::cout << ".\n";

    std::cout << toPlayer->getName() << ", do you accept the trade offer from "
              << fromPlayer->getName() << "? (y/n): ";

    std::string response;
    std::cin >> response;

    if (response != "y" && response != "Y") {
        std::cout << "[Trade] Offer rejected.\n";
        return;
    }

    if (giveIsMoney) {
        int amount = std::stoi(giveStr);
        fromPlayer->pay(amount);
        toPlayer->receive(amount);
    } else {
        fromPlayer->removeProperty(giveStr);
        toPlayer->addProperty(giveStr);
        getBuilding(giveStr)->setOwnerToken(toToken);
    }

    if (receiveIsMoney) {
        int amount = std::stoi(receiveStr);
        toPlayer->pay(amount);
        fromPlayer->receive(amount);
    } else {
        toPlayer->removeProperty(receiveStr);
        fromPlayer->addProperty(receiveStr);
        getBuilding(receiveStr)->setOwnerToken(fromToken);
    }

    std::cout << "[Trade] " << fromPlayer->getName() << " traded " << giveStr
              << " with " << toPlayer->getName() << " for " << receiveStr << ".\n";
}



void GameController::setBoard(Board* b) {
    board = b;

    // Populate controller's buildings map from board squares
    for (int i = 0; i < 40; ++i) {
        Square* sq = board->getSquare(i);
        if (auto* bldg = dynamic_cast<Building*>(sq)) {
            buildings[bldg->getName()] = bldg;
        }
    }
}

void GameController::playTurn(Player* p, std::optional<std::pair<int, int>> forcedDice) {
    bool skipRoll = false;
    bool skipExtraTurn = false;
    bool escapedJail = false;

    int die1 = 0, die2 = 0, steps = 0;
    static int doublesStreak = 0;

    int oldPos = p->getPosition();
    int newPos = oldPos;

    auto rollDice = [&]() {
        if (forcedDice) {
            die1 = forcedDice->first;
            die2 = forcedDice->second;
            std::cout << "[TEST] Simulating roll: " << die1 << " and " << die2
                      << " (Total: " << die1 + die2 << ")\n";
        } else {
            die1 = std::rand() % 6 + 1;
            die2 = std::rand() % 6 + 1;
        }
        steps = die1 + die2;
    };

    // ====== Jail Logic ======
    if (p->isInTims()) {
        std::cout << "[STATUS] " << p->getName()
                  << " is in DC Tims Line (Turn " << p->getTimsTurns() + 1 << "/3) | "
                  << "Roll Up Cups: " << p->getRollUpCups() << " | "
                  << "Money: $" << p->getMoney() << "\n";

        std::string choice;

        if (p->getRollUpCups() > 0) {
            std::cout << "[Controller]: Use Roll Up the Rim cup? (y/n): ";
            std::cin >> choice;
            if (choice == "y" || choice == "Y") {
                p->useRollUpCup();
                p->setInTims(false);
                p->resetTimsTurns();
                escapedJail = true;
                std::cout << "[ACTION] Used a Roll Up the Rim cup. Player is now free.\n";
            }
        }

        if (!escapedJail && p->getMoney() >= 50) {
            std::cout << "[Controller]: Pay $50 to get out of Tims? (y/n): ";
            std::cin >> choice;
            if (choice == "y" || choice == "Y") {
                enforcePayment(p, 50); // Enforcing chill as we have funds
                p->setInTims(false);
                p->resetTimsTurns();
                escapedJail = true;
                std::cout << "[ACTION] Paid $50. Player is now free.\n";
            }
        }

        if (!escapedJail && p->isInTims()) {
            std::cout << "[Controller]: Attempting jail escape with "
                      << (forcedDice ? "forced roll" : "random roll") << "...\n";
            rollDice();
            std::cout << p->getName() << " rolls " << die1 << " and " << die2 << ".\n";

            if (die1 == die2) {
                std::cout << "[SUCCESS] Doubles! " << p->getName()
                          << " escapes jail and moves " << steps << " steps.\n";
                p->move(steps);
                p->setInTims(false);
                p->resetTimsTurns();
                skipRoll = true;
                skipExtraTurn = true;
            } else if (p->getTimsTurns() == 2) {
                std::cout << "[FAIL] Third failed attempt. Paying $50 and moving " << steps << " steps.\n";
                enforcePayment(p, 50);
                p->move(steps);
                p->setInTims(false);
                p->resetTimsTurns();
                skipRoll = true;
                skipExtraTurn = true;
            } else {
                std::cout << "[FAIL] No doubles. Turn skipped.\n";
                p->incrementTimsTurn();
                return;
            }
        }
    }

    // ====== Roll if not already moved ======
    if (!skipRoll) {
        rollDice();
        std::cout << p->getName() << " rolled " << die1 << " and " << die2
                  << " for a total of " << steps << ".\n";

        if (die1 == die2) {
            ++doublesStreak;
            if (doublesStreak == 3) {
                std::cout << "[RULE] " << p->getName() << " rolled 3 consecutive doubles. Go to DC Tims Line!\n";
                p->moveTo(10);
                p->setInTims(true);
                p->resetTimsTurns();
                doublesStreak = 0;
                return;
            }
        } else {
            doublesStreak = 0;
        }

        oldPos = p->getPosition();
        p->move(steps);
        newPos = p->getPosition();

        if (newPos < oldPos && newPos != 0) {
            std::cout << p->getName() << " passed Collect OSAP and collects $200!\n";
            p->receive(200);
        }
    }

    // ====== Square effect ======
    Square* landed = board->getSquare(p->getPosition());
    LandAction action = landed->onLand(p);

    switch (action) {
        case LandAction::PromptPurchase: {
            if (auto* b = dynamic_cast<Building*>(landed)) {
                promptPurchase(p, b);
            }
            break;
        }

        case LandAction::PayRent: {
            auto* b = dynamic_cast<Building*>(landed);
            if (!b || b->isMortgaged()) break;

            Player* owner = getPlayer(b->getOwnerToken());
            if (!owner) break;

            if (owner->isInTims()) {
                std::cout << "[Controller]: " << owner->getName() << " is in jail. No rent collected.\n";
                break;
            }

            int context = 0;
            if (dynamic_cast<Residence*>(b)) {
                context = getResidenceCount(b->getOwnerToken());
            } else if (dynamic_cast<Gym*>(b)) {
                context = getGymCount(b->getOwnerToken()) * steps;
            } else if (auto* ab = dynamic_cast<AcademicBuilding*>(b)) {
                if (hasMonopoly(ab->getOwnerToken(), ab->getMonopolyBlock()) &&
                ab->getImprovementCount() == 0) {
                context = 1;  // signal double rent
                }
            }

            int rent = b->calculateRent(context);
            std::cout << "[Controller]: " << p->getName()
                      << " must pay $" << rent << " in rent.\n";
            enforcePayment(p, rent, owner);
            break;
        }

        case LandAction::Owned:
            std::cout << "[Controller]: You landed on your own property. Nothing to do.\n";
            break;

        case LandAction::GoToTims: {
            std::cout << "[Controller]: " << p->getName()
                      << " has been sent to DC Tims Line (Position 10).\n";
            p->moveTo(10);
            p->setInTims(true);
            p->resetTimsTurns();
            skipExtraTurn = true;
            break;
        }

        case LandAction::NEEDLESHALL: {
            int roll = std::rand() % 18;
            int delta = 0;

            if (roll == 0) delta = -200;
            else if (roll <= 2) delta = -100;
            else if (roll <= 5) delta = -50;
            else if (roll <= 11) delta = 25;
            else if (roll <= 14) delta = 50;
            else if (roll <= 16) delta = 100;
            else delta = 200;

            std::cout << p->getName() << " landed on " << landed->getName()
                      << " and received a financial change of " << delta << ".\n";

            if (delta >= 0) p->receive(delta);
            else enforcePayment(p, -delta);

            break;
        }

        case LandAction::PayTuition: {
            std::cout << "[TUITION] " << p->getName()
                      << " must choose to pay $300 or 10% of total worth.\n";

            std::cout << "[Controller]: Choose payment method:\n";
            std::cout << "1. Pay $300\n";
            std::cout << "2. Pay 10% of total worth\n";
            std::cout << "Enter choice (1 or 2): ";

            int choice = 1;
            std::cin >> choice;

            int totalWorth = p->getMoney();
            for (const auto& [_, building] : buildings) {
                if (building->getOwnerToken() == p->getToken()) {
                    totalWorth += building->getPrice();
                    if (auto* ab = dynamic_cast<AcademicBuilding*>(building)) {
                        totalWorth += ab->getImprovementCount() * ab->getImprovementCost();
                    }
                }
            }

            if (choice == 2) {
                int fee = totalWorth / 10;
                std::cout << "[TUITION] 10% of total worth ($" << totalWorth << ") = $" << fee << ".\n";
                enforcePayment(p, fee);
            } else {
                std::cout << "[TUITION] Paying flat $300 fee.\n";
                enforcePayment(p, 300);
            }

            break;
        }

        case LandAction::PayCoopFee: {
            std::cout << p->getName() << " landed on " << landed->getName()
                      << " and must pay the $150 Coop Fee.\n";
            enforcePayment(p, 150);
            break;
        }

        default:
            std::cout << "[Controller]: No action required.\n";
            break;
    }

    bool extraTurn = (die1 == die2) && !p->isInTims() && !skipExtraTurn;
    if (extraTurn) {
        std::cout << "[Controller]: " << p->getName()
                  << " rolled doubles and gets another turn!\n";

        if (forcedDice) {
            int nextDie1, nextDie2;
            std::cout << "[TEST INPUT]: Enter next dice roll (die1 die2): ";
            std::cin >> nextDie1 >> nextDie2;
            playTurn(p, std::pair{nextDie1, nextDie2});
        } else {
            playTurn(p);
        }
    }
}


void GameController::promptPurchase(Player* p, Building* b) {
    std::cout << "[Controller]: Would you like to buy " << b->getName()
              << " for $" << b->getPrice() << "? (y/n): ";

    std::string choice;
    std::cin >> choice;

    if (choice == "y" || choice == "Y") {
        if (p->getMoney() >= b->getPrice()) {
            p->pay(b->getPrice());
            p->addProperty(b->getName());
            b->setOwnerToken(p->getToken());

            std::cout << "[Controller]: " << p->getName() << " now owns " << b->getName() << "!\n";
        } else {
            std::cout << "[Controller]: " << p->getName() << " cannot afford this property.\n";
            handleAuction(b);
        }
    } else {
        std::cout << "[Controller]: " << p->getName() << " declined to buy " << b->getName() << ".\n";
        handleAuction(b);
    }
}

int GameController::getResidenceCount(const std::string& ownerToken) const {
    int count = 0;
    for (const auto& [name, b] : buildings) {
        if (dynamic_cast<Residence*>(b) && b->getOwnerToken() == ownerToken) {
            ++count;
        }
    }
    std::cout << "[DEBUG] Buildings in controller: " << buildings.size() << "\n";

    return count;
}

int GameController::getGymCount(const std::string& ownerToken) const {
    int count = 0;
    for (const auto& [name, b] : buildings) {
        if (dynamic_cast<Gym*>(b) && b->getOwnerToken() == ownerToken) {
            ++count;
        }
    }
    std::cout << "[DEBUG] Buildings in controller: " << buildings.size() << "\n";
    return count;
}

bool GameController::improveBuilding(Player* p, AcademicBuilding* ab) {
    if (ab->getOwnerToken() != p->getToken()) {
        std::cout << "[Error] You do not own this building.\n";
        return false;
    }

    if (ab->isMortgaged()) {
        std::cout << "[Error] Cannot improve a mortgaged building.\n";
        return false;
    }

    if (ab->getImprovementCount() >= 5) {
        std::cout << "[Error] Max improvements reached.\n";
        return false;
    }

    if (!hasMonopoly(p->getToken(), ab->getMonopolyBlock())) {
        std::cout << "[Error] You must own all buildings in the block to improve.\n";
        return false;
    }

    int cost = ab->getImprovementCost();
    if (p->getMoney() < cost) {
        std::cout << "[Error] Not enough funds to improve.\n";
        return false;
    }

    ab->addImprovement();
    p->pay(cost);

    std::cout << "[Success] " << ab->getName() << " improved to "
              << ab->getImprovementCount() << " level(s).\n";

    return true;
}

bool GameController::hasMonopoly(const std::string& token, const std::string& blockName) {
    for (const auto& [name, b] : buildings) {
        auto* ab = dynamic_cast<AcademicBuilding*>(b);
        if (ab && ab->getMonopolyBlock() == blockName) {
            if (ab->getOwnerToken() != token) {
                return false;  // Player doesn't own this building in the block
            }
        }
    }
    return true;  // All buildings in block are owned by player
}

bool GameController::degradeBuilding(Player* p, AcademicBuilding* ab) {
    if (ab->getOwnerToken() != p->getToken()) {
        std::cout << "[Error] You do not own this building.\n";
        return false;
    }

    if (ab->getImprovementCount() <= 0) {
        std::cout << "[Error] No improvements to remove.\n";
        return false;
    }

    ab->removeImprovement();
    int refund = ab->getImprovementCost() / 2;
    p->receive(refund);

    std::cout << "[Success] Sold 1 improvement from " << ab->getName()
              << ". New level: " << ab->getImprovementCount()
              << ". Refunded $" << refund << ".\n";

    return true;
}

bool GameController::mortgageBuilding(Player* p, Building* b) {

    if (!b) {
        std::cout << "[Error] Invalid building.\n";
        return false;
    }

    if (b->getOwnerToken() != p->getToken()) {
        std::cout << "[Error] You don't own " << b->getName() << ".\n";
        return false;
    }

    if (b->isMortgaged()) {
        std::cout << "[Error] " << b->getName() << " is already mortgaged.\n";
        return false;
    }

    // Check for improvements (only relevant for AcademicBuilding)
    if (auto* ab = dynamic_cast<AcademicBuilding*>(b)) {
        if (ab->getImprovementCount() > 0) {
            std::cout << "[Error] Cannot mortgage a building with improvements.\n";
            return false;
        }
    }

    if (auto* ab = dynamic_cast<AcademicBuilding*>(b)) {
        std::string block = ab->getMonopolyBlock();

        for (const auto& [name, bldg] : buildings) {
            auto* other = dynamic_cast<AcademicBuilding*>(bldg);
            if (other && other->getMonopolyBlock() == block) {
                if (other->getImprovementCount() > 0) {
                    std::cout << "[Error] Cannot mortgage: other properties in the block have improvements.\n";
                    return false;
                }
            }
        }
    }


    b->setMortgaged(true);
    int value = b->getPrice() / 2;
    p->receive(value);

    std::cout << "[Success] " << b->getName() << " mortgaged for $" << value << ".\n";
    return true;
}

bool GameController::unmortgageBuilding(Player* p, Building* b) {

    if (!b) {
        std::cout << "[Error] Invalid building.\n";
        return false;
    }
    
    if (b->getOwnerToken() != p->getToken()) {
        std::cout << "[Error] You don't own " << b->getName() << ".\n";
        return false;
    }

    if (!b->isMortgaged()) {
        std::cout << "[Error] " << b->getName() << " is not mortgaged.\n";
        return false;
    }

    int repay = (b->getPrice() / 2) * 1.1;  // 10% interest
    if (p->getMoney() < repay) {
        std::cout << "[Error] Not enough money to unmortgage.\n";
        return false;
    }

    b->setMortgaged(false);
    p->pay(repay);

    std::cout << "[Success] " << b->getName() << " unmortgaged for $" << repay << ".\n";
    return true;
}

void GameController::simulateTurn(Player* p, int forcedDie1, int forcedDie2) {
    bool skipRoll = false;
    bool skipExtraTurn = false;
    bool escapedJail = false;
    int die1 = forcedDie1, die2 = forcedDie2, steps = die1 + die2;
    int oldPos = p->getPosition();
    int newPos = oldPos;

    std::cout << "[TEST] Simulating roll: " << die1 << " and " << die2
              << " (Total: " << steps << ")\n";

    if (p->isInTims()) {
        std::cout << "[STATUS] " << p->getName()
            << " is in DC Tims Line (Turn " << p->getTimsTurns() + 1 << "/3) | "
            << "Roll Up Cups: " << p->getRollUpCups() << " | "
            << "Money: $" << p->getMoney() << "\n";

        std::string choice;

        // === Option 1: Use Roll Up the Rim Cup ===
        if (p->getRollUpCups() > 0) {
            std::cout << "[Controller]: Use Roll Up the Rim cup? (y/n): ";
            std::cin >> choice;
            if (choice == "y" || choice == "Y") {
                p->useRollUpCup();
                p->setInTims(false);
                p->resetTimsTurns();
                escapedJail = true;
                std::cout << "[ACTION] Used a Roll Up the Rim cup. Player is now free.\n";
            }
        }

        // === Option 2: Pay $50 to escape early ===
        if (!escapedJail && p->getMoney() >= 50) {
            std::cout << "[Controller]: Pay $50 to get out of Tims? (y/n): ";
            std::cin >> choice;
            if (choice == "y" || choice == "Y") {
                p->pay(50);
                p->setInTims(false);
                p->resetTimsTurns();
                escapedJail = true;
                std::cout << "[ACTION] Paid $50. Player is now free.\n";
            }
        }

        // === Option 3: Try to roll doubles ===
        if (!escapedJail && p->isInTims()) {
            std::cout << "[Controller]: Attempting jail escape with forced roll...\n";
            std::cout << p->getName() << " rolls " << die1 << " and " << die2 << ".\n";

            if (die1 == die2) {
                std::cout << "[SUCCESS] Doubles! " << p->getName()
                    << " escapes jail and moves " << (die1 + die2) << " steps.\n";
                p->move(die1 + die2);
                p->setInTims(false);
                p->resetTimsTurns();

                skipRoll = true;
                skipExtraTurn = true;
            } else if (p->getTimsTurns() == 2) {
                std::cout << "[FAIL] Third failed attempt. Paying $50 and moving " << (die1 + die2) << " steps.\n";
                p->pay(50);
                p->move(die1 + die2);
                p->setInTims(false);
                p->resetTimsTurns();

                skipRoll = true;
                skipExtraTurn = true;
            } else {
                std::cout << "[FAIL] No doubles. Turn skipped.\n";
                p->incrementTimsTurn();
                return;  // End turn
            }
        }
    }

    if (!skipRoll) {
        oldPos = p->getPosition();
        p->move(steps);
        newPos = p->getPosition();

        if (newPos < oldPos) {
            std::cout << p->getName() << " passed Collect OSAP and collects $200!\n";
            p->receive(200);
        }
    }

    Square* landed = board->getSquare(p->getPosition());
    LandAction action = landed->onLand(p);

    switch (action) {
        case LandAction::PromptPurchase: {
            if (auto* b = dynamic_cast<Building*>(landed)) {
                promptPurchase(p, b);
            }
            break;
        }

        case LandAction::PayRent: {
            auto* b = dynamic_cast<Building*>(landed);
            if (!b || b->isMortgaged()) break;

            int context = 0;
            if (dynamic_cast<Residence*>(b)) {
                context = getResidenceCount(b->getOwnerToken());
            } else if (dynamic_cast<Gym*>(b)) {
                context = getGymCount(b->getOwnerToken()) * steps;
            }

            int rent = b->calculateRent(context);
            std::cout << "[Controller]: " << p->getName()
                      << " must pay $" << rent << " in rent.\n";
            p->pay(rent);
            getPlayer(b->getOwnerToken())->receive(rent);
            break;
        }

        case LandAction::Owned:
            std::cout << "[Controller]: You landed on your own property. Nothing to do.\n";
            break;

        case LandAction::GoToTims: {
            std::cout << "[Controller]: " << p->getName()
                      << " has been sent to DC Tims Line (Position 10).\n";
            p->moveTo(10);
            p->setInTims(true);
            p->resetTimsTurns();
            skipExtraTurn = true;
            break;
        }

        default:
            std::cout << "[Controller]: No action required.\n";
            break;
    }

    bool extraTurn = (die1 == die2) && !p->isInTims() && !skipExtraTurn;
    if (extraTurn) {
        std::cout << "[Controller]: " << p->getName()
            << " rolled doubles and gets another turn!\n";

        int nextDie1, nextDie2;
        std::cout << "[TEST INPUT]: Enter next dice roll (die1 die2): ";
        std::cin >> nextDie1 >> nextDie2;

        simulateTurn(p, nextDie1, nextDie2);
    }
}

void GameController::handleAuction(Building* b) {
    std::cout << "[Auction] " << b->getName() << " is now up for auction!\n";

    struct Bidder {
        Player* p;
        bool active = true;
    };

    std::vector<Bidder> bidders;
    for (const auto& [token, player] : players) {
        if (!player->isBankrupt()) {
            bidders.push_back({player});
        }
    }

    if (bidders.empty()) {
        std::cout << "[Auction] No players available to bid.\n";
        return;
    }

    int highestBid = 0;
    Player* highestBidder = nullptr;

    while (true) {
        for (auto& bidder : bidders) {
            if (!bidder.active) continue;

            int bid = -1;
            std::string input;

            while (true) {
                std::cout << "[Auction] " << bidder.p->getName()
                          << " (Balance: $" << bidder.p->getMoney() << "), enter bid (0 to pass): ";
                std::cin >> input;

                bool valid = !input.empty() && std::all_of(input.begin(), input.end(), ::isdigit);
                if (!valid) {
                    std::cout << "[Error] Invalid input. Please enter a non-negative number.\n";
                    continue;
                }

                bid = std::stoi(input);

                if (bid == 0) {
                    bidder.active = false;
                    break;
                }

                if (bid <= highestBid) {
                    std::cout << "[Auction] Bid must be higher than current highest: $" << highestBid << ".\n";
                    continue;
                }

                if (bid > bidder.p->getMoney()) {
                    std::cout << "[Error] You don't have enough money. You're out.\n";
                    bidder.active = false;
                    break;
                }

                // Valid bid
                highestBid = bid;
                highestBidder = bidder.p;
                break;
            }

            // ✅ Early exit if only one bidder remains
            int activeCount = 0;
            for (const auto& b : bidders) {
                if (b.active) ++activeCount;
            }
            if (activeCount <= 1) goto AUCTION_END;
        }
    }

    AUCTION_END:
        // If only one active bidder remains and no bid was placed,
        // auto-assign property for $0
        if (!highestBidder) {
            for (auto& bidder : bidders) {
                if (bidder.active) {
                    highestBidder = bidder.p;
                    highestBid = 0;
                    std::cout << "[Auction] Only one player remained. Property goes to "
                      << highestBidder->getName() << " for FREE.\n";
                    break;
                }
            }
        }

        highestBidder->pay(highestBid);
        highestBidder->addProperty(b->getName());
        b->setOwnerToken(highestBidder->getToken());

    std::cout << "[Auction] " << highestBidder->getName()
              << " wins the auction for " << b->getName()
              << " at $" << highestBid << "!\n";
}

void GameController::declareBankruptcy(Player* debtor, Player* creditor) {
    std::cout << "[BANKRUPTCY] " << debtor->getName()
              << " is declaring bankruptcy"
              << (creditor ? " to " + creditor->getName() : " to the Bank") << ".\n";

    // === Transfer all properties ===
    for (auto& [name, b] : buildings) {
        if (b->getOwnerToken() != debtor->getToken()) continue;

        if (creditor) {
            // Bankruptcy to another player
            b->setOwnerToken(creditor->getToken());
            creditor->addProperty(name);
            debtor->removeProperty(name);

            std::cout << "[TRANSFER] " << name << " transferred to " << creditor->getName() << ".\n";

            if (b->isMortgaged()) {
                int interest = b->getPrice() / 10; // 10%
                std::cout << "[MORTGAGED] " << name << " is mortgaged. "
                          << creditor->getName() << " must pay $"
                          << interest << " in interest to the Bank.\n";
                creditor->pay(interest);
            }
        } else {
            // Bankruptcy to the Bank — return to open market
            b->setOwnerToken("BANK");
            b->setMortgaged(false);
            debtor->removeProperty(name);
            std::cout << "[RESET] " << name << " returned to Bank.\n";
        }
    }

    // === Transfer or destroy roll-up cups ===
    int cups = debtor->getRollUpCups();
    if (creditor) {
        for (int i = 0; i < cups; ++i) {
            creditor->addRollUpCup();
        }
        std::cout << "[TRANSFER] " << cups << " Roll Up the Rim cup(s) transferred.\n";
    } else {
        std::cout << "[DESTROY] " << cups << " Roll Up the Rim cup(s) destroyed.\n";
    }
    debtor->setRollUpCups(0);

    // === Transfer remaining money (if any) ===
    int moneyLeft = debtor->getMoney();
    if (creditor && moneyLeft > 0) {
        debtor->pay(moneyLeft);
        creditor->receive(moneyLeft);
        std::cout << "[TRANSFER] $" << moneyLeft << " transferred to " << creditor->getName() << ".\n";
    }

    // === Mark debtor as bankrupt ===
    debtor->setBankrupt(true);
    std::cout << "[STATUS] " << debtor->getName() << " is now out of the game.\n";
}

bool GameController::enforcePayment(Player* debtor, int amount, Player* creditor) {
    if (debtor->getMoney() >= amount) {
        debtor->pay(amount);
        if (creditor) creditor->receive(amount);
        return true;
    }

    std::cout << "[Bankruptcy Check] " << debtor->getName() << " can't afford to pay $" << amount << ".\n";
    
    bool rescued = attemptToRaiseFunds(debtor, amount);

    if (rescued) {
        std::cout << "[Recovery] " << debtor->getName() << " raised enough money. Paying...\n";
        debtor->pay(amount);
        if (creditor) creditor->receive(amount);
        return true;
    }

    std::cout << "[Bankruptcy] " << debtor->getName() << " is declaring bankruptcy!\n";
    declareBankruptcy(debtor, creditor);
    return false;
}

bool GameController::attemptToRaiseFunds(Player* p, int amountOwed) {
    std::cout << "\n💸 [Bankruptcy Warning] " << p->getName() << " owes $" << amountOwed << ".\n";
    std::cout << "💰 Current funds: $" << p->getMoney() << "\n";
    int deficit = amountOwed - p->getMoney();
    if (deficit <= 0) {
        std::cout << " You already have enough funds. No need to raise money.\n";
        return true;
    }

    std::cout << "❗ You are short by $" << deficit << ". You must raise funds manually.\n";

    while (p->getMoney() < amountOwed) {
        std::vector<AcademicBuilding*> improvable;
        std::vector<Building*> mortgageable;

        for (const auto& [name, b] : buildings) {
            if (b->getOwnerToken() != p->getToken()) continue;

            if (auto* ab = dynamic_cast<AcademicBuilding*>(b)) {
                if (ab->getImprovementCount() > 0) {
                    improvable.push_back(ab);
                }
            }

            if (!b->isMortgaged()) {
                auto* ab = dynamic_cast<AcademicBuilding*>(b);
                if (!ab || ab->getImprovementCount() == 0) {
                    mortgageable.push_back(b);
                }
            }
        }

        std::cout << "\n💸 [Liquidation Menu] Funds: $" << p->getMoney()
                  << " | Owe: $" << amountOwed << " | Remaining: $" << (amountOwed - p->getMoney()) << "\n";
        std::cout << "Choose an action:\n";
        std::cout << "1. Sell an Improvement\n";
        std::cout << "2. Mortgage a Property\n";
        std::cout << "3. Quit and declare bankruptcy\n";
        std::cout << "Enter your choice (1-3): ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            if (improvable.empty()) {
                std::cout << "[Error] No buildings with improvements to sell.\n";
                continue;
            }

            std::cout << "\n[Improvements Available to Sell]\n";
            for (size_t i = 0; i < improvable.size(); ++i) {
                std::cout << i + 1 << ". " << improvable[i]->getName()
                          << " (" << improvable[i]->getImprovementCount()
                          << " improvements at $" << improvable[i]->getImprovementCost() / 2 << " each)\n";
            }

            std::cout << "Choose which building to sell 1 improvement from (1-" << improvable.size() << "): ";
            int sel;
            std::cin >> sel;

            if (sel < 1 || static_cast<size_t>(sel) > improvable.size()) {
                std::cout << "[Error] Invalid choice.\n";
                continue;
            }

            auto* ab = improvable[sel - 1];
            degradeBuilding(p, ab);  // Handles printing
        }

        else if (choice == 2) {
            if (mortgageable.empty()) {
                std::cout << "[Error] No properties can be mortgaged.\n";
                continue;
            }

            std::cout << "\n[Properties Available to Mortgage]\n";
            for (size_t i = 0; i < mortgageable.size(); ++i) {
                std::cout << i + 1 << ". " << mortgageable[i]->getName()
                          << " (Mortgage value: $" << mortgageable[i]->getPrice() / 2 << ")\n";
            }

            std::cout << "Choose property to mortgage (1-" << mortgageable.size() << "): ";
            int sel;
            std::cin >> sel;

            if (sel < 1 || static_cast<size_t>(sel) > mortgageable.size()) {
                std::cout << "[Error] Invalid choice.\n";
                continue;
            }

            auto* b = mortgageable[sel - 1];
            mortgageBuilding(p, b);  // Handles printing
        }

        else if (choice == 3) {
            std::cout << "[INFO] You chose to stop raising funds and declare bankruptcy.\n";
            return false;
        }

        else {
            std::cout << "[Error] Invalid choice. Please try again.\n";
        }
    }

    std::cout << "\n You now have $" << p->getMoney()
              << " and can pay your debt of $" << amountOwed << ".\n";
    return true;
}

void GameController::printAssets(Player* p) {
    std::cout << "\n💼 Assets for " << p->getName() << " [" << p->getToken() << "]\n";
    std::cout << "💰 Money: $" << p->getMoney() << "\n";
    std::cout << "🥤 Roll Up Cups: " << p->getRollUpCups() << "\n";
    
    const auto& props = p->getProperties();
    if (props.empty()) {
        std::cout << "🏠 No properties owned.\n";
    } else {
        std::cout << "🏠 Properties:\n";
        for (const auto& name : props) {
            Building* b = getBuilding(name);
            std::cout << "  - " << name;
            if (auto* ab = dynamic_cast<AcademicBuilding*>(b)) {
                std::cout << " | Improvements: " << ab->getImprovementCount();
            }
            std::cout << (b->isMortgaged() ? " [MORTGAGED]" : "") << "\n";
        }
    }
}
