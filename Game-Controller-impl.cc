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
