// Square-impl.cc (implementation)
// Module: Square
// Implements the core interface for abstract board squares in Watopoly.
// This file defines basic accessors and the constructor for Square.

module Square;

// Constructor for Square: initializes the name and board position.
Square::Square(std::string name, int position) : name{name}, position{position} {}

// Returns the name of the square.
std::string Square::getName() const {
    return name;
}

// Returns the position of the square on the board (0-indexed).
int Square::getPosition() const {
    return position;
}

