#include "TextDisplay.h"
#include "Board.h"   // Assumes Board has: std::vector<Square*> getSquares() const;
#include "Square.h"  // Assumes Square has: std::string getName() const;
#include "Player.h"  // Assumes Player has: int getPosition() const, char getToken() const, std::string getName() const, int getMoney() const;
#include <iostream>

TextDisplay::TextDisplay() {}

void TextDisplay::render(const Board &board, const std::vector<Player*> &players) const {
    // Retrieve the board squares (assumed to be 40 in number)
    std::vector<Square*> squares = board.getSquares();

    std::cout << "=============================================\n";
    std::cout << "                WATOPOLY BOARD               \n";
    std::cout << "=============================================\n";
    
    // Print squares in a single row (adjust layout if needed)
    for (size_t i = 0; i < squares.size(); ++i) {
        std::cout << "[" << squares[i]->getName();
        
        // Append any players present on this square
        for (size_t j = 0; j < players.size(); j++) {
            if (players[j]->getPosition() == static_cast<int>(i))
                std::cout << players[j]->getToken();
        }
        std::cout << "] ";
    }
    std::cout << "\n=============================================\n";
    
    // Print player status information
    for (size_t i = 0; i < players.size(); i++) {
        std::cout << "Player " << players[i]->getToken() 
                  << " (" << players[i]->getName() 
                  << ") - Money: $" << players[i]->getMoney() << "\n";
    }
}
