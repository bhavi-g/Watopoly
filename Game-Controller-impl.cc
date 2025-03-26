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
void GameController::trade(const std::string& fromToken, const std::string& giveB,
                           const std::string& toToken, const std::string& receiveB) {
    auto* fromPlayer = getPlayer(fromToken);
    auto* toPlayer = getPlayer(toToken);
    auto* give = getBuilding(giveB);
    auto* receive = getBuilding(receiveB);

    if (!fromPlayer || !toPlayer || !give || !receive) {
        std::cout << "Trade failed: invalid player or building.\n";
        return;
    }

    if (give->getOwnerToken() != fromToken || receive->getOwnerToken() != toToken) {
        std::cout << "Trade failed: ownership mismatch.\n";
        return;
    }

    // Perform ownership transfer
    give->setOwnerToken(toToken);
    receive->setOwnerToken(fromToken);

    // Update each player's property list
    fromPlayer->removeProperty(giveB);
    fromPlayer->addProperty(receiveB);
    toPlayer->removeProperty(receiveB);
    toPlayer->addProperty(giveB);

    std::cout << fromToken << " traded " << giveB << " for " << receiveB
              << " with " << toToken << "!\n";
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
                p->pay(50);
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
                p->pay(50);
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

        case LandAction::RandomMoneyChange:
            std::cout << "[DEBUG] Controller noted a random money effect occurred (Needles Hall).\n";
            break;

        case LandAction::PayTuition: {
            std::cout << "[Tuition] " << p->getName()
              << " must choose to pay $300 or 10% of total worth.\n";
    
            std::cout << "[Controller]: Choose payment method:\n";
            std::cout << "1. Pay $300\n";
            std::cout << "2. Pay 10% of total worth\n";
            std::cout << "Enter choice (1 or 2): ";

            int choice = 1;
            std::cin >> choice;

            int totalWorth = p->getMoney();

            // Add value of owned buildings
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
                std::cout << "[Tuition] 10% of total worth ($" << totalWorth << ") = $" << fee << ".\n";
                p->pay(fee);
            } else {
                std::cout << "[Tuition] Paying flat $300 fee.\n";
                p->pay(300);
            }

            break;
        }


        case LandAction::Teleport: {
            // Player already moved during SLC; just handle new square
            Square* newSquare = board->getSquare(p->getPosition());
            LandAction nestedAction = newSquare->onLand(p);

            // Recursively handle the square landed on after teleport
            switch (nestedAction) {
                case LandAction::PromptPurchase: {
                    if (auto* b = dynamic_cast<Building*>(newSquare)) {
                        promptPurchase(p, b);
                    }
                    break;
                }

                case LandAction::PayRent: {
                    auto* b = dynamic_cast<Building*>(newSquare);  // FIXED
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

                case LandAction::GoToTims:
                    std::cout << "[Controller]: " << p->getName()
                      << " has been sent to DC Tims Line (Position 10).\n";
                    p->moveTo(10);
                    p->setInTims(true);
                    p->resetTimsTurns();
                    skipExtraTurn = true;
                    break;

                case LandAction::RandomMoneyChange:
                    std::cout << "[DEBUG] Controller noted a random money effect occurred (Needles Hall).\n";
                    break;

                case LandAction::PayTuition: {
                    std::cout << "[Tuition] " << p->getName()
                        << " must choose to pay $300 or 10% of total worth.\n";
    
                    std::cout << "[Controller]: Choose payment method:\n";
                    std::cout << "1. Pay $300\n";
                    std::cout << "2. Pay 10% of total worth\n";
                    std::cout << "Enter choice (1 or 2): ";

                    int choice = 1;
                    std::cin >> choice;

                    int totalWorth = p->getMoney();

                    // Add value of owned buildings
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
                        std::cout << "[Tuition] 10% of total worth ($" << totalWorth << ") = $" << fee << ".\n";
                        p->pay(fee);
                    } else {
                        std::cout << "[Tuition] Paying flat $300 fee.\n";
                        p->pay(300);
                    }

                    break;
                }

                default:
                    break;
            }
            break;
        }

        default:
            std::cout << "[Controller]: No action required.\n";
            break;
    }


    // ====== Handle doubles logic ======
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
        }
    } else {
        std::cout << "[Controller]: " << p->getName() << " declined to buy " << b->getName() << ".\n";
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

