// Residence-impl.cc (implementation)
// Module: Residence
// Description:
//   Implements the onLand behavior for Residence squares in Watopoly.
//   Residences include MKV, REV, V1, and UWP.
//
//   Rent depends on the number of residences the owner controls:
//     - 1:  $25
//     - 2:  $50
//     - 3: $100
//     - 4: $200
//
//   Actual rent charging logic will be handled later via GameController.

module Residence;

import <iostream>;
import LandAction;
import GameController;

// Constructs a Residence square with name, board index, and purchase price.
Residence::Residence(std::string name, int position, int price)
    : Building{name, position, price} {}

// Called when a player lands on a Residence square.
// Describes rent conditions or purchase options based on ownership.
LandAction Residence::onLand(Player* p) {
    std::cout << p->getName() << " landed on Residence " << getName() << ".\n";

    if (getOwnerToken() == "BANK") {
        std::cout << "You may buy this for $" << getPrice() << ".\n";
        return LandAction::PromptPurchase;
    } else if (getOwnerToken() != p->getToken()) {
        std::cout << "Rent is based on how many residences " << getOwnerToken() << " owns.\n";
        return LandAction::PayRent;
    } else {
        std::cout << "You own this.\n";
        return LandAction::Owned;
    }
}

