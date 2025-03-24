#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include <vector>
#include <string>
#include "Xwindow.h"  // Use your provided Xwindow class interface

// Forward declarations
class Board;
class Player;
class Square;

class GraphicsDisplay {
public:
    GraphicsDisplay();
    ~GraphicsDisplay();
    // Render the board and players graphically in the window
    void render(const Board &board, const std::vector<Player*> &players) const;

private:
    Xwindow *win;
    static const int windowWidth = 800;
    static const int windowHeight = 800;
    static const int squareSize = 80; // Adjust as needed
};

#endif
