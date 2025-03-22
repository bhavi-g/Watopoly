// Academic-Building-impl.cc (implementation)
// Module: AcademicBuilding
// Description:
//   Implements behavior for academic buildings on the board.
//   AcademicBuildings are ownable and upgradable (0–5 improvements).
//   Rent calculation is affected by improvements and monopoly ownership.
//
//   Logic for rent or improvement purchase will be delegated to GameController.

module AcademicBuilding;

import <iostream>;
import LandAction;
import GameController;  // For future integration of monopoly/rent logic

// Constructor initializes building fields and improvement tracking.
AcademicBuilding::AcademicBuilding(std::string name, int position, int price,
                                   std::string monopolyBlock, int improvementCost)
    : Building{name, position, price}, 
      monopolyBlock{monopolyBlock}, 
      improvementCost{improvementCost}, 
      improvements{0} {}

// Returns the name of the monopoly block (e.g., "Math", "Env").
std::string AcademicBuilding::getMonopolyBlock() const {
    return monopolyBlock;
}

// Returns the cost of a single improvement (bathroom/cafeteria).
int AcademicBuilding::getImprovementCost() const {
    return improvementCost;
}

// Returns how many improvements are currently on this building.
int AcademicBuilding::getImprovements() const {
    return improvements;
}

// Adds one improvement to the building, capped at 5.
void AcademicBuilding::addImprovement() {
    if (improvements < 5) ++improvements;
}

// Removes one improvement from the building, bottomed at 0.
void AcademicBuilding::removeImprovement() {
    if (improvements > 0) --improvements;
}

// Defines what happens when a player lands on this academic building.
// Ownership is checked and appropriate messages are printed.
// Future version will delegate rent calculation to GameController.
LandAction AcademicBuilding::onLand(Player* p) {
    std::cout << p->getName() << " landed on Academic Building " << getName() << ".\n";

    if (getOwnerToken() == "BANK") {
        std::cout << "You may buy this for $" << getPrice() << ".\n";
        return LandAction::PromptPurchase;
    } else if (getOwnerToken() != p->getToken()) {
        std::cout << "Rent logic for academic buildings goes here (considering improvements).\n";
        return LandAction::PayRent;
    } else {
        std::cout << "You own this.\n";
        return LandAction::Owned;
    }
}

