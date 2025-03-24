#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>

class Board;
class Square;
class Player;

class Display {
public:
    Display();
    ~Display();

    // Print the entire Watopoly board in ASCII.
    // 'board' is your Board object with 40 squares.
    // 'players' is a vector of Player pointers.
    void printGameBoard(const Board &board, const std::vector<Player*> &players);
};

#endif
