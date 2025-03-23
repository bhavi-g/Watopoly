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

void GameController::playTurn(Player* p) {
    // === 1. Roll dice ===
    int die1 = std::rand() % 6 + 1;
    int die2 = std::rand() % 6 + 1;
    int steps = die1 + die2;

    std::cout << p->getName() << " rolled " << die1 << " and " << die2
              << " for a total of " << steps << ".\n";

    // === 2. Move player ===
    int oldPos = p->getPosition();
    p->move(steps);  // Wraps at 40 automatically

    int newPos = p->getPosition();
    Square* landed = board->getSquare(newPos);

    // === 3. Handle passing Collect OSAP (position 0) ===
    if (newPos < oldPos) {
        std::cout << p->getName() << " passed Collect OSAP and collects $200!\n";
        p->receive(200);
    }

    // === 4. Land on square and react based on LandAction ===
    LandAction action = landed->onLand(p);

    switch (action) {
        case LandAction::PromptPurchase: {
            // Cast to Building* so we can access getPrice/setOwner, etc.
            Building* b = dynamic_cast<Building*>(landed);
            if (b) {
                promptPurchase(p, b);
            } else {
                std::cout << "[Error]: PromptPurchase returned but square is not a Building!\n";
            }
            break;
        }

        case LandAction::PayRent: {
            auto* b = dynamic_cast<Building*>(landed);
            if (!b) break;

            if (b->isMortgaged()) {
                std::cout << "[Controller]: " << b->getName() << " is mortgaged. No rent collected.\n";
                break;
            }

            int context = 0;

            if (dynamic_cast<Residence*>(b)) {
                context = getResidenceCount(b->getOwnerToken());
            } else if (dynamic_cast<Gym*>(b)) {
                int numGymsOwned = getGymCount(b->getOwnerToken());
                context = numGymsOwned * (die1 + die2);  // Flattened: context = full rent
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

        case LandAction::CollectOSAP:
        case LandAction::PayTuition:
        case LandAction::PayCoopFee:
        case LandAction::Teleport:
        case LandAction::MoneyEvent:
            // These are handled inside onLand() already
            break;

        case LandAction::None:
        default:
            std::cout << "[Controller]: No action required.\n";
            break;
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

