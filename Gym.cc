// Gym.cc (interface)
// Module: Gym
// Description:
//   Represents an ownable Gym square in Watopoly (e.g., "PAC", "CIF").
//   Rent on Gyms is calculated based on a dice roll and the number of
//   Gyms owned by the same player (4x or 10x dice sum).
//
//   Gym inherits from Building and overrides the onLand behavior.
//   Actual dice rolling and rent logic may be delegated to GameController.
//
// Related Modules:
//   - Building (provides price/ownership framework)
//   - Player (passed to onLand for decision making)
//   - GameController (may be used later for rolling dice or applying fees)

export module Gym;

import LandAction;
import Building;
import Player;
import <string>;

export class Gym : public Building {
public:
    // Constructs a Gym with name, board position, and price.
    Gym(std::string name, int position, int price);

    // Called when a player lands on the Gym.
    // Future: GameController may determine dice outcome and apply rent.
    LandAction onLand(Player* p) override;
};
