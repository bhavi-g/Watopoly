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
        case LandAction::PromptPurchase:
            std::cout << "[Controller]: Would you like to purchase this property? (Prompt logic goes here)\n";
            break;

        case LandAction::PayRent:
            std::cout << "[Controller]: Rent must be paid. (Rent logic goes here)\n";
            break;

        case LandAction::Owned:
            std::cout << "[Controller]: You landed on your own property. Nothing to do.\n";
            break;

        case LandAction::None:
        default:
            std::cout << "[Controller]: No action required.\n";
            break;
    }
}
