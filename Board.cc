// Board.cc (interface)
// Module: Board
// Description:
//   Represents the Watopoly game board containing 40 squares.
//   This class holds a vector of polymorphic Square* objects,
//   constructed and ordered according to Watopoly layout.
//
//   Board is purely a data container. It does NOT:
//     - Know about players
//     - Move players
//     - Execute game logic
//
//   All player interaction and movement is handled externally,
//   typically by GameController. This follows the CS246 principle
//   of separating data (Board) from control (GameController).
//
//   Design Patterns:
//     - Composite Pattern: Board holds a homogeneous collection of
//       polymorphic Square subclasses.
//     - Mediator Pattern: GameController drives interaction between
//       Board, Square, and Player.

export module Board;

import <vector>;
import <string>;
import Square;

export class Board {
private:
    std::vector<Square*> squares;  // Ordered list of 40 squares

public:
    // Constructs the full board with all 40 square types in order.
    Board();

    // Destroys all dynamically allocated Square* objects.
    ~Board();

    // Retrieves the square at the given board index (0–39).
    Square* getSquare(int position) const;

    // Retrieves a square by its name (e.g., "EV1", "PAC").
    Square* getSquareByName(const std::string& name) const;

    // Optional: Display method (text or X11) for board visualization.
    void display() const;
};
