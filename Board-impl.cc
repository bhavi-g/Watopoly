// Board-impl.cc (implementation)
// Module: Board
// Description:
//   Initializes the 40 Square* instances that make up the Watopoly board.
//   Squares include ownable (AcademicBuilding, Residence, Gym) and
//   non-ownable (Needles Hall, SLC, Tuition, DC Tims Line, etc.).
//
//   This class is purely structural — it owns the squares and exposes
//   access to them. Gameplay control is handled externally (e.g., by GameController).

module Board;

import <iostream>;
import <algorithm>;
import AcademicBuilding;
import Residence;
import Gym;
import ActionSquares;

Board::Board() {
    // === Populate all 40 squares in order ===

    squares.push_back(new ActionSquare("Collect OSAP", 0));
    squares.push_back(new AcademicBuilding("AL", 1, 40, "Arts1", 50));
    squares.push_back(new SLC("SLC", 2));
    squares.push_back(new AcademicBuilding("ML", 3, 60, "Arts1", 50));
    squares.push_back(new Tuition("Tuition", 4));
    squares.push_back(new Residence("MKV", 5, 200));
    squares.push_back(new AcademicBuilding("ECH", 6, 100, "Arts2", 50));
    squares.push_back(new NeedlesHall("Needles Hall", 7));
    squares.push_back(new AcademicBuilding("PAS", 8, 100, "Arts2", 50));
    squares.push_back(new AcademicBuilding("HH", 9, 120, "Arts2", 50));
    squares.push_back(new ActionSquare("DC Tims Line", 10));
    squares.push_back(new AcademicBuilding("RCH", 11, 140, "Eng", 100));
    squares.push_back(new Gym("PAC", 12, 150));
    squares.push_back(new AcademicBuilding("DWE", 13, 140, "Eng", 100));
    squares.push_back(new AcademicBuilding("CPH", 14, 160, "Eng", 100));
    squares.push_back(new Residence("UWP", 15, 200));
    squares.push_back(new AcademicBuilding("LHI", 16, 180, "Health", 100));
    squares.push_back(new SLC("SLC", 17));
    squares.push_back(new AcademicBuilding("BMH", 18, 180, "Health", 100));
    squares.push_back(new AcademicBuilding("OPT", 19, 200, "Health", 100));
    squares.push_back(new ActionSquare("Goose Nesting", 20));
    squares.push_back(new AcademicBuilding("EV1", 21, 220, "Env", 150));
    squares.push_back(new NeedlesHall("Needles Hall", 22));
    squares.push_back(new AcademicBuilding("EV2", 23, 220, "Env", 150));
    squares.push_back(new AcademicBuilding("EV3", 24, 240, "Env", 150));
    squares.push_back(new Residence("V1", 25, 200));
    squares.push_back(new AcademicBuilding("PHYS", 26, 260, "Sci1", 150));
    squares.push_back(new AcademicBuilding("B1", 27, 260, "Sci1", 150));
    squares.push_back(new Gym("CIF", 28, 150));
    squares.push_back(new AcademicBuilding("B2", 29, 280, "Sci1", 150));
    squares.push_back(new GoToTims("GO TO TIMS", 30));
    squares.push_back(new AcademicBuilding("EIT", 31, 300, "Sci2", 200));
    squares.push_back(new AcademicBuilding("ESC", 32, 300, "Sci2", 200));
    squares.push_back(new SLC("SLC", 33));
    squares.push_back(new AcademicBuilding("C2", 34, 320, "Sci2", 200));
    squares.push_back(new Residence("REV", 35, 200));
    squares.push_back(new NeedlesHall("Needles Hall", 36));
    squares.push_back(new AcademicBuilding("MC", 37, 350, "Math", 200));
    squares.push_back(new ActionSquare("COOP FEE", 38));
    squares.push_back(new AcademicBuilding("DC", 39, 400, "Math", 200));
}

Board::~Board() {
    for (auto* square : squares) {
        delete square;
    }
    squares.clear();
}

// Returns the square at a given board position.
Square* Board::getSquare(int position) const {
    if (position < 0 || position >= static_cast<int>(squares.size())) return nullptr;
    return squares[position];
}

// Returns a square by its name (e.g., "DC", "EV1").
Square* Board::getSquareByName(const std::string& name) const {
    for (auto* square : squares) {
        if (square->getName() == name) return square;
    }
    return nullptr;
}

// Optional text display of the board state.
void Board::display() const {
    std::cout << "=== WATOPOLY BOARD ===\n";
    for (auto* square : squares) {
        std::cout << "[" << square->getPosition() << "] " << square->getName() << "\n";
    }
}
