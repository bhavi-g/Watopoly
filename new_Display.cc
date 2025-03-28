export module new_Display;

import Board;              // Board should have: Square* getSquare(int) const;
import Square;             // Square should have: std::string getName() const;
import Player;             // Player should have: getPosition(), getToken() (returns std::string), getName(), getMoney()
import AcademicBuilding;   // AcademicBuilding should have: getImprovementCount() returning improvement count.
import <string>;
import <vector>;

export class new_Display {
public:
    new_Display();
    ~new_Display();

    // Print the entire Watopoly board in ASCII.
    // 'board' is your Board object containing 40 squares.
    // 'players' is a vector of pointers to Player.
    void printGameBoard(const Board &board, const std::vector<Player*> &players);
};
