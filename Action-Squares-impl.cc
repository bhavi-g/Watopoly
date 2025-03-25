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
import LandAction;  // New enum for signaling control flow
import Player;

// -------- ActionSquare --------
ActionSquare::ActionSquare(std::string name, int position)
    : Square{name, position} {}

LandAction ActionSquare::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and arrived at a generic action square. Nothing happens.\n";
    return LandAction::None;
}

// -------- CollectOSAP --------
CollectOSAP::CollectOSAP(std::string name, int position)
    : Square{name, position} {}

LandAction CollectOSAP::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and collected $200 from OSAP.\n";
    p->receive(200);
    return LandAction::CollectOSAP;
}

// -------- Tuition --------
Tuition::Tuition(std::string name, int position)
    : Square{name, position} {}

LandAction Tuition::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and must pay $300 or 10% of total worth.\n";
    return LandAction::PayTuition;
}

// -------- NeedlesHall --------
NeedlesHall::NeedlesHall(std::string name, int position)
    : Square{name, position} {}

LandAction NeedlesHall::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and triggers a random monetary effect (gain or loss).\n";
    return LandAction::MoneyEvent;
}

// -------- SLC --------
SLC::SLC(std::string name, int position)
    : Square{name, position} {}

LandAction SLC::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and is teleported somewhere else on the board.\n";
    return LandAction::Teleport;
}

// -------- DCTimsLine --------
DCTimsLine::DCTimsLine(std::string name, int position)
    : Square{name, position} {}

LandAction DCTimsLine::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " but was not sent here — nothing happens.\n";
    return LandAction::None;
}

// -------- GooseNesting --------
GooseNesting::GooseNesting(std::string name, int position)
    : Square{name, position} {}

LandAction GooseNesting::onLand(Player* p) {
    std::cout << "A flock of geese attack " << p->getName()
              << " near " << getName() << "! But it’s just a scare.\n";
    return LandAction::None;
}

// -------- CoopFee --------
CoopFee::CoopFee(std::string name, int position)
    : Square{name, position} {}

LandAction CoopFee::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and must pay the $150 Coop Fee.\n";
    p->pay(150);
    return LandAction::PayCoopFee;
}

// -------- GoToTims --------
GoToTims::GoToTims(std::string name, int position)
    : Square{name, position} {}

LandAction GoToTims::onLand(Player* p) {
    std::cout << p->getName() << " landed on " << getName()
              << " and is sent directly to DC Tims Line.\n";
              
    return LandAction::GoToTims;
}
