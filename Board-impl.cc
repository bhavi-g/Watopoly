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
    squares.push_back(new NeedlesHall("NEEDLES HALL", 2));
    squares.push_back(new Residence("MKV", 3, 200));
    squares.push_back(new Tuition("TUITION", 4));
    squares.push_back(new Gym("PAC", 5, 150));
    squares.push_back(new SLC("SLC", 6));
    squares.push_back(new AcademicBuilding("ML", 7, 60, "Arts1", 50));
    squares.push_back(new ActionSquare("DC Tims Line", 8));
    squares.push_back(new AcademicBuilding("ECH", 9, 100, "Arts2", 50));
    squares.push_back(new AcademicBuilding("PAS", 10, 100, "Arts2", 50));
    squares.push_back(new NeedlesHall("NEEDLES HALL", 11));
    squares.push_back(new Residence("V1", 12, 200));
    squares.push_back(new Gym("CIF", 13, 150));
    squares.push_back(new Residence("REV", 14, 200));
    squares.push_back(new AcademicBuilding("HH", 15, 120, "Arts2", 50));
    squares.push_back(new SLC("SLC", 16));
    squares.push_back(new AcademicBuilding("RCH", 17, 140, "Eng", 100));
    squares.push_back(new Gym("PAC", 18, 150));
    squares.push_back(new AcademicBuilding("CPH", 19, 160, "Eng", 100));
    squares.push_back(new ActionSquare("DC Tims Line", 20));
    squares.push_back(new AcademicBuilding("LHI", 21, 180, "Health", 100));
    squares.push_back(new NeedlesHall("NEEDLES HALL", 22));
    squares.push_back(new Residence("UWP", 23, 200));
    squares.push_back(new SLC("SLC", 24));
    squares.push_back(new AcademicBuilding("ESC", 25, 300, "Sci2", 200));
    squares.push_back(new AcademicBuilding("C2", 26, 320, "Sci2", 200));
    squares.push_back(new NeedlesHall("NEEDLES HALL", 27));
    squares.push_back(new AcademicBuilding("EIT", 28, 300, "Sci2", 200));
    squares.push_back(new AcademicBuilding("MC", 29, 350, "Math", 200));
    squares.push_back(new ActionSquare("GO TO TIMS", 30));
    squares.push_back(new AcademicBuilding("B1", 31, 260, "Sci1", 150));
    squares.push_back(new AcademicBuilding("B2", 32, 280, "Sci1", 150));
    squares.push_back(new SLC("SLC", 33));
    squares.push_back(new AcademicBuilding("PHYS", 34, 260, "Sci1", 150));
    squares.push_back(new AcademicBuilding("EV1", 35, 220, "Env", 150));
    squares.push_back(new NeedlesHall("NEEDLES HALL", 36));
    squares.push_back(new AcademicBuilding("EV2", 37, 220, "Env", 150));
    squares.push_back(new AcademicBuilding("EV3", 38, 240, "Env", 150));
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
