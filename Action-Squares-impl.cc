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
    int roll = std::rand() % 18;  // 0 to 17
    int delta = 0;

    if (roll == 0) delta = -200;
    else if (roll <= 2) delta = -100;      // 1,2 → 2/18 = 1/9
    else if (roll <= 5) delta = -50;       // 3,4,5 → 3/18 = 1/6
    else if (roll <= 11) delta = 25;       // 6–11 → 6/18 = 1/3
    else if (roll <= 14) delta = 50;       // 12,13,14 → 3/18 = 1/6
    else if (roll <= 16) delta = 100;      // 15,16 → 2/18 = 1/9
    else delta = 200;                      // 17 → 1/18

    std::cout << p->getName() << " landed on " << getName()
              << " and received a financial change of " << delta << ".\n";

    if (delta >= 0) p->receive(delta);
    else p->pay(-delta);

    return LandAction::RandomMoneyChange;
}

// -------- SLC --------
SLC::SLC(std::string name, int position)
    : Square{name, position} {}

LandAction SLC::onLand(Player* p) {
    int roll = std::rand() % 24;  // 0 to 23
    int move = 0;
    std::string explanation;

    // --- Special Cases: 1/24 each ---
    if (roll == 0) {
        // 1/24 → Go to DC Tims Line
        explanation = "sent directly to DC Tims Line!";
        p->moveTo(10);
        p->setInTims(true);
        p->resetTimsTurns();
        std::cout << "[SLC] " << p->getName() << " is " << explanation << "\n";
        return LandAction::GoToTims;
    } else if (roll == 1) {
        // 1/24 → Advance to Collect OSAP
        explanation = "advanced to Collect OSAP!";
        p->moveTo(0);
        std::cout << "[SLC] " << p->getName() << " is " << explanation << "\n";
        return LandAction::CollectOSAP;
    }

    // --- Movement Cases: Distribution math ---
    // 22 outcomes left → assign as follows:
    //  - Back 3     → 1/8    → 22 * (1/8)    = 2.75 ≈ 3 slots → roll 2–4
    //  - Back 2     → 1/6    → 22 * (1/6)    = 3.66 ≈ 4 slots → roll 5–8
    //  - Back 1     → 1/6    → 22 * (1/6)    = 3.66 ≈ 4 slots → roll 9–12
    //  - Forward 1  → 1/8    → 22 * (1/8)    = 2.75 ≈ 3 slots → roll 13–15
    //  - Forward 2  → 1/6    → 22 * (1/6)    = 3.66 ≈ 4 slots → roll 16–19
    //  - Forward 3  → 1/6    → 22 * (1/6)    = 3.66 ≈ 4 slots → roll 20–23

    if (roll <= 4) move = -3;        // roll 2–4 → 3 slots
    else if (roll <= 8) move = -2;   // roll 5–8 → 4 slots
    else if (roll <= 12) move = -1;  // roll 9–12 → 4 slots
    else if (roll <= 15) move = 1;   // roll 13–15 → 3 slots
    else if (roll <= 19) move = 2;   // roll 16–19 → 4 slots
    else move = 3;                   // roll 20–23 → 4 slots

    int old = p->getPosition();
    p->move(move);
    int newPos = p->getPosition();

    std::cout << "[SLC] " << p->getName() << " moves from " << old
              << " to " << newPos << " (offset: " << move << ").\n";

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
