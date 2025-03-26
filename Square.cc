// Square.cc (interface)
// Module: Square
// Description:
//   Abstract base class representing any square on the Watopoly board.
//   Used as a parent class for ownable (e.g., Building) and non-ownable
//   (e.g., SLC, Needles Hall, Tuition) squares.
//
//   This class is designed for polymorphic use via the onLand method.
//   All subclasses must override onLand(Player*) to define what happens
//   when a player lands on that square.
//
// Related Modules:
//   - Player (used as forward dependency)
//   - Building, AcademicBuilding, Residence, Gym (inherit from Square)
//   - Board (stores std::vector<Square*>)
//   - GameController (invokes square->onLand(player) to trigger effects)

export module Square;

import <iostream>;
import <string>;
import LandAction;
import Player;

export class Square {
protected:
    std::string name;     // Display name of the square (e.g., "EV1", "MKV")
    int position;         // Board index (0-based from "Collect OSAP")

public:
    // Constructs a Square with a name and position on the board.
    Square(std::string name, int position);

    // Virtual destructor to allow polymorphic deletion.
    virtual ~Square() = default;

    // Returns the display name of the square.
    std::string getName() const;

    // Returns the position index of the square on the board.
    int getPosition() const;

    // Pure virtual function — must be overridden by all derived squares.
    // Defines what should happen when a player lands on this square.
    virtual LandAction onLand(Player* p) = 0;



};
