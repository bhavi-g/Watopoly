// Residence.cc (interface)
// Module: Residence
// Description:
//   Represents a residence property in Watopoly (e.g., MKV, REV, V1, UWP).
//   Rent is based on how many residences the owner controls:
//     - 1 owned:  $25
//     - 2 owned:  $50
//     - 3 owned: $100
//     - 4 owned: $200
//
//   Ownership and rent logic will eventually be handled by GameController,
//   but basic onLand behavior is overridden here.
//
// Related Modules:
//   - Building (base class for all ownable properties)
//   - Player (used to identify who landed on the square)
//   - GameController (may later apply rent and handle ownership logic)

export module Residence;

import Building;
import Player;
import <string>;

export class Residence : public Building {
public:
    // Constructs a Residence with name, position on board, and purchase price.
    Residence(std::string name, int position, int price);

    // Defines behavior when a player lands on this square.
    // Rent is printed based on owner status (actual rent logic handled elsewhere).
    void onLand(Player* p) override;
};
