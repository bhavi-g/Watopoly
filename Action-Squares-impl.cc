// ActionSquares-impl.cc (implementation)
// Module: ActionSquares
// Description:
//   Implements onLand() logic for all simple non-ownable squares.
//   All messages are placeholders to be replaced by real controller logic.
//
//   Grouping these implementations avoids redundant files and allows easy
//   navigation and editing of small square types during iteration.

module ActionSquares;

import <iostream>;

// -------- ActionSquare --------
ActionSquare::ActionSquare(std::string name, int position)
    : Square{name, position} {}

void ActionSquare::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and arrived at a generic action square. Nothing happens.\n";
}

// -------- NeedlesHall --------
NeedlesHall::NeedlesHall(std::string name, int position)
    : Square{name, position} {}

void NeedlesHall::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and triggers a random monetary effect (gain or loss).\n";
}

// -------- SLC --------
SLC::SLC(std::string name, int position)
    : Square{name, position} {}

void SLC::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and is teleported somewhere else on the board.\n";
}

// -------- Tuition --------
Tuition::Tuition(std::string name, int position)
    : Square{name, position} {}

void Tuition::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and must pay $300 or 10% of total worth.\n";
}
