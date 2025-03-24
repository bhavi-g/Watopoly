#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <vector>
#include <string>

// Forward declarations
class Board;
class Player;
class Square;

class TextDisplay {
public:
    TextDisplay();
    // Render the board and player info to the terminal
    void render(const Board &board, const std::vector<Player*> &players) const;
};

#endif
