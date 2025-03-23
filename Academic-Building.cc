// AcademicBuilding.cc (interface)
// Module: AcademicBuilding
// Description:
//   Subclass of Building representing ownable academic buildings in Watopoly.
//   Each academic building belongs to a monopoly block (e.g., "Arts1", "Eng").
//   Buildings can be improved up to 5 times (4 bathrooms + 1 cafeteria).
//
//   Improvement count affects rent calculation (to be implemented).
//   GameController is responsible for invoking improvement-related actions
//   and checking monopoly ownership.
//
//   Used by: GameController (for gameplay decisions), Board (as Square*),
//            Player (owns buildings via name), Rent logic.

export module AcademicBuilding;

import LandAction;
import Building;  // Inherits from Building, which is-a Square
import Player;
import <string>;

export class AcademicBuilding : public Building {
private:
    std::string monopolyBlock;   // Name of the monopoly block this building belongs to
    int improvementCost;         // Cost per improvement (bathroom/cafeteria)
    int improvements;            // Current number of improvements (0–5)

public:
    // Constructs an academic building with monopoly metadata and improvement cost.
    AcademicBuilding(std::string name, int position, int price,
                     std::string monopolyBlock, int improvementCost);

    // Returns the monopoly block name (e.g., "Sci1", "Math").
    std::string getMonopolyBlock() const;

    // Called when a player lands on this academic building.
    // Displays rent/purchase logic. Future version will calculate rent based on improvements.
    LandAction onLand(Player* p) override;

    int calculateRent(int context = 0) const override;

    void addImprovement();             // increments improvement count

    void removeImprovement();          // decrements

    int getImprovementCount() const;   // returns count
    
    int getImprovementCost() const;    // stored from constructor

};
