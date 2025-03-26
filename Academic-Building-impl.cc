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
import <map>;
import <vector>;
import LandAction;

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

void AcademicBuilding::addImprovement() {
    if (improvements >= 5) throw std::runtime_error("Max improvements reached");
    ++improvements;
}

void AcademicBuilding::removeImprovement() {
    if (improvements <= 0) throw std::runtime_error("No improvements to remove");
    --improvements;
}

int AcademicBuilding::getImprovementCount() const {
    return improvements;
}

int AcademicBuilding::getImprovementCost() const {
    return improvementCost;
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

int AcademicBuilding::calculateRent(int /*context*/) const {
    // Rent depends only on improvements
    static const std::map<std::string, std::vector<int>> rentTable = {
        { "AL", {2, 10, 30, 90, 160, 250} },
        { "ML", {4, 20, 60, 180, 320, 450} },
        { "ECH", {6, 30, 90, 270, 400, 550} },
        { "PAS", {6, 30, 90, 270, 400, 550} },
        { "HH", {8, 40, 100, 300, 450, 600} },
        { "RCH", {10, 50, 150, 450, 625, 750} },
        { "DWE", {10, 50, 150, 450, 625, 750} },
        { "CPH", {12, 60, 180, 500, 700, 900} },
        { "LHI", {14, 70, 200, 550, 750, 950} },
        { "BMH", {14, 70, 200, 550, 750, 950} },
        { "OPT", {16, 80, 220, 600, 800, 1000} },
        { "EV1", {18, 90, 250, 700, 875, 1050} },
        { "EV2", {18, 90, 250, 700, 875, 1050} },
        { "EV3", {20, 100, 300, 750, 925, 1100} },
        { "PHYS", {22, 110, 330, 800, 975, 1150} },
        { "B1", {22, 110, 330, 800, 975, 1150} },
        { "B2", {24, 120, 360, 850, 1025, 1200} },
        { "EIT", {26, 130, 390, 900, 1100, 1275} },
        { "ESC", {26, 130, 390, 900, 1100, 1275} },
        { "C2", {28, 150, 450, 1000, 1200, 1400} },
        { "MC", {35, 175, 500, 1100, 1300, 1500} },
        { "DC", {50, 200, 600, 1400, 1700, 2000} }
    };

    auto it = rentTable.find(getName());
    if (it == rentTable.end()) return 0;

    return it->second[improvements];  // 0–5
}



