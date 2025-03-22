// Gym-impl.cc (implementation)
// Module: Gym
// Description:
//   Implements the onLand behavior for Gym squares in Watopoly.
//   Rent depends on the number of Gyms owned by the current owner:
//     - 1 Gym  →  4x dice roll
//     - 2 Gyms → 10x dice roll
//
//   GameController may later provide dice rolling and rent charging logic.
//   This implementation simply prints expected behavior.

module Gym;

import <iostream>;
import LandAction;
import GameController;

// Constructs a Gym with name, board position, and purchase price.
// Delegates base initialization to Building.
Gym::Gym(std::string name, int position, int price)
    : Building{name, position, price} {}

// Called when a player lands on a Gym square.
// If unowned, gives purchase option.
// If owned by another player, prints rent rule.
// If owned by self, does nothing.
LandAction Gym::onLand(Player* p) {
    std::cout << p->getName() << " landed on Gym " << getName() << ".\n";

    if (getOwnerToken() == "BANK") {
        std::cout << "You may buy this for $" << getPrice() << ".\n";
        return LandAction::PromptPurchase;
    } else if (getOwnerToken() != p->getToken()) {
        std::cout << "Rent is 4x or 10x dice roll depending on # gyms owned by "
                  << getOwnerToken() << ".\n";
        return LandAction::PayRent;
    } else {
        std::cout << "You own this.\n";
        return LandAction::Owned;
    }
}

