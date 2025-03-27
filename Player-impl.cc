// Player-impl.cc (implementation)
// Module: Player
// Description:
//   Implements the Player class, which models the state and actions of a
//   human-controlled player in Watopoly.
//
//   This includes tracking their name, token, balance, and buildings owned
//   (by name only to avoid circular dependencies with Building).
//
//   Used by: GameController (to manage player interactions),
//            Square subclasses (to check ownership or apply effects)

module Player;

import <iostream>;
import <algorithm>;  // Needed for std::find and std::remove

// Constructs a new player with name, token, and optional starting balance.
Player::Player(std::string name, std::string token, int startMoney)
    : name{name}, token{token}, money{startMoney} {}

// Returns the player's name.
std::string Player::getName() const {
    return name;
}

// Returns the player's token, used to identify ownership on buildings.
std::string Player::getToken() const {
    return token;
}

// Returns the player's current money balance.
int Player::getMoney() const {
    return money;
}

// Deducts a given amount from the player's money.
// Prints the result to the console.
void Player::pay(int amount) {
    money -= amount;
    std::cout << name << " paid $" << amount << ". Remaining: $" << money << "\n";
}

// Adds a given amount to the player's money.
// Prints the result to the console.
void Player::receive(int amount) {
    money += amount;
    std::cout << name << " received $" << amount << ". New total: $" << money << "\n";
}

// Adds a building to the player's list of owned properties.
// Uses building name string to avoid direct Building reference.
void Player::addProperty(const std::string& buildingName) {
    ownedBuildings.push_back(buildingName);
}

// Removes a building from the player's owned list if it exists.
void Player::removeProperty(const std::string& buildingName) {
    ownedBuildings.erase(
        std::remove(ownedBuildings.begin(), ownedBuildings.end(), buildingName),
        ownedBuildings.end()
    );
}

// Checks if the player owns a specific building by name.
bool Player::owns(const std::string& buildingName) const {
    return std::find(
        ownedBuildings.begin(), ownedBuildings.end(), buildingName
    ) != ownedBuildings.end();
}

// Returns the current board position of the player.
int Player::getPosition() const {
    return position;
}

// Moves the player forward by a number of steps with board wraparound.
void Player::move(int steps) {
    position = (position + steps) % 40;
    std::cout << name << " moves to position " << position << "\n";
}

void Player::moveTo(int newPosition) {
    position = newPosition % 40;
    std::cout << name << " moves directly to position " << position << "\n";
}

void Player::setMoney(int newAmount) {
    money = newAmount;
}

bool Player::isInTims() const {
    return inTims;
}

void Player::setInTims(bool value) {
    inTims = value;
}

int Player::getTimsTurns() const {
    return timsTurns;
}

void Player::incrementTimsTurn() {
    ++timsTurns;
}

void Player::resetTimsTurns() {
    timsTurns = 0;
}

int Player::getRollUpCups() const { return rollUpCups; }
void Player::addRollUpCup() { ++rollUpCups; }
void Player::useRollUpCup() {
    if (rollUpCups > 0) --rollUpCups;
}

bool Player::isBankrupt() const {
    return false; // 🔧 TEMPORARY: We'll handle actual bankruptcy logic later
}