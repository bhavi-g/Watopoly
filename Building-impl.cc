// Building-impl.cc (implementation)
// Module: Building
// Description:
//   Implements the base logic for any ownable square on the Watopoly board.
//   This includes basic data members like name, price, and ownerToken,
//   as well as onLand(Player*) polymorphic behavior.
//
//   Building is an abstract foundation for:
//     - AcademicBuilding
//     - Residence
//     - Gym
//
//   GameController is responsible for handling actual ownership transfer,
//   rent payment, or player decisions. onLand() here only handles the
//   presentation logic and condition flow.

module Building;

import <iostream>;
import LandAction;
import Player;  // Required to access Player* methods

// Constructor: initializes building with name, position, price.
// Owner is defaulted to "BANK" (unowned).
Building::Building(std::string name, int position, int price)
    : Square{name, position}, price{price}, ownerToken{"BANK"} {}

// Returns the name of the building (inherited from Square).
std::string Building::getName() const {
    return name;
}

// Returns the price to purchase this building.
int Building::getPrice() const {
    return price;
}

// Returns the owner token string ("BANK" if unowned).
std::string Building::getOwnerToken() const {
    return ownerToken;
}

// Updates the building's owner token (used by GameController).
void Building::setOwnerToken(const std::string& token) {
    ownerToken = token;
}

// Triggered when a player lands on this square.
// Outputs the result: buy, rent, or no-op depending on ownership.
LandAction Building::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName() << " at position "
              << getPosition() << ".\n";

    if (ownerToken == "BANK") {
        std::cout << "This property is unowned. You may buy it for $" << price << ".\n";
        return LandAction::PromptPurchase;
    } else if (ownerToken != p->getToken()) {
        std::cout << "This property is owned by " << ownerToken << ". Rent logic goes here.\n";
        return LandAction::PayRent;
    } else {
        std::cout << "You own this property.\n";
        return LandAction::Owned;
    }
}

