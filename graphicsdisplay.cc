#include "GraphicsDisplay.h"
#include "Board.h"   // Assumes Board has: std::vector<Square*> getSquares() const;
#include "Square.h"  // Assumes Square has: std::string getName() const;
#include "Player.h"  // Assumes Player has: int getPosition() const, char getToken() const;
#include <string>

GraphicsDisplay::GraphicsDisplay() {
    win = new Xwindow(windowWidth, windowHeight);
}

GraphicsDisplay::~GraphicsDisplay() {
    delete win;
}

void GraphicsDisplay::render(const Board &board, const std::vector<Player*> &players) const {
    // Clear the window with a white background (using your Xwindow constants)
    win->fillRectangle(0, 0, windowWidth, windowHeight, Xwindow::White);
    
    // Retrieve squares from the board (assumed 40 squares)
    std::vector<Square*> squares = board.getSquares();
    
    // Layout assumption: Monopoly board style:
    // Top row: indices 0 - 9 (left to right)
    for (int i = 0; i < 10; ++i) {
        int x = i * squareSize;
        int y = 0;
        win->fillRectangle(x, y, squareSize, squareSize, Xwindow::Black);
        win->drawString(x + 5, y + 20, squares[i]->getName());
    }
    
    // Right column: indices 10 - 19 (top to bottom)
    for (int i = 10; i < 20; ++i) {
        int x = windowWidth - squareSize;
        int y = (i - 10) * squareSize;
        win->fillRectangle(x, y, squareSize, squareSize, Xwindow::Black);
        win->drawString(x + 5, y + 20, squares[i]->getName());
    }
    
    // Bottom row: indices 20 - 29 (right to left)
    for (int i = 20; i < 30; ++i) {
        int x = windowWidth - squareSize - ((i - 20) * squareSize);
        int y = windowHeight - squareSize;
        win->fillRectangle(x, y, squareSize, squareSize, Xwindow::Black);
        win->drawString(x + 5, y + 20, squares[i]->getName());
    }
    
    // Left column: indices 30 - 39 (bottom to top)
    for (int i = 30; i < 40; ++i) {
        int x = 0;
        int y = windowHeight - squareSize - ((i - 30) * squareSize);
        win->fillRectangle(x, y, squareSize, squareSize, Xwindow::Black);
        win->drawString(x + 5, y + 20, squares[i]->getName());
    }
    
    // Draw player tokens. Map board positions to coordinates using same layout as above.
    for (size_t i = 0; i < players.size(); ++i) {
        int pos = players[i]->getPosition();
        int x, y;
        if (pos < 10) {            // Top row
            x = pos * squareSize;
            y = 0;
        } else if (pos < 20) {      // Right column
            x = windowWidth - squareSize;
            y = (pos - 10) * squareSize;
        } else if (pos < 30) {      // Bottom row
            x = windowWidth - squareSize - ((pos - 20) * squareSize);
            y = windowHeight - squareSize;
        } else {                   // Left column
            x = 0;
            y = windowHeight - squareSize - ((pos - 30) * squareSize);
        }
        // Draw the player's token (as a string)
        win->drawString(x + 20, y + 40, std::string(1, players[i]->getToken()));
    }
}
