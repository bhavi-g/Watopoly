// ActionSquares.cc (interface)
// Module: ActionSquares
// Description:
//   Contains a bundle of simple non-ownable square classes in Watopoly:
//     - ActionSquare     (e.g., "GO TO TIMS", "Collect OSAP")
//     - NeedlesHall      (random money gain/loss)
//     - SLC              (teleports player to random square)
//     - Tuition          (player pays flat amount or percentage)
//
//   These classes are bundled together for simplicity because:
//     - They are small and single-purpose
//     - None require internal state beyond name and position
//     - Their onLand() logic is easily readable side-by-side
//
//   This follows CS246 principles of:
//     - Low complexity for modular features
//     - Minimizing circular dependencies
//     - Avoiding file sprawl when classes are tightly related

export module ActionSquares;

import Square;
import Player;
import <string>;

// --------------------------------------------
// Generic one-off square for messages like:
// - "Go to Tims", "Collect OSAP", "Goose Nesting"
// --------------------------------------------
export class ActionSquare : public Square {
public:
    ActionSquare(std::string name, int position);
    void onLand(Player* p) override;
};

// --------------------------------------------
// Triggers a random money gain/loss event.
// Message-only for now; logic handled by controller later.
// --------------------------------------------
export class NeedlesHall : public Square {
public:
    NeedlesHall(std::string name, int position);
    void onLand(Player* p) override;
};

// --------------------------------------------
// Moves the player to another square randomly.
// Message-only for now.
// --------------------------------------------
export class SLC : public Square {
public:
    SLC(std::string name, int position);
    void onLand(Player* p) override;
};

// --------------------------------------------
// Tuition square: pay $300 or 10% of total worth.
// Decision logic will be implemented later.
// --------------------------------------------
export class Tuition : public Square {
public:
    Tuition(std::string name, int position);
    void onLand(Player* p) override;
};
