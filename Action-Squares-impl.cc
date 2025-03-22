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

// -------- CollectOSAP --------
// Grants $200 on landing.
// GameController will later handle "passing" the square logic.
CollectOSAP::CollectOSAP(std::string name, int position)
    : Square{name, position} {}

void CollectOSAP::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and collected $200 from OSAP.\n";
    p->receive(200);
}

// -------- DCTimsLine --------
// Nothing happens when landing here directly.
DCTimsLine::DCTimsLine(std::string name, int position)
    : Square{name, position} {}

void DCTimsLine::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " but was not sent here — nothing happens.\n";
}

// -------- GooseNesting --------
// Player gets "attacked" but no actual effect.
GooseNesting::GooseNesting(std::string name, int position)
    : Square{name, position} {}

void GooseNesting::onLand(Player* p) {
    std::cout << "A flock of geese attack " << p->getName()
              << " near " << getName() << "! But it’s just a scare.\n";
}

// -------- CoopFee --------
// Deducts $150 from player.
CoopFee::CoopFee(std::string name, int position)
    : Square{name, position} {}

void CoopFee::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and must pay the $150 Coop Fee.\n";
    p->pay(150);
}
