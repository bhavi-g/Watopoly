/*import <iostream>;

import Square;
import Player;
import Board;

// This test will simulate a single player landing on all 40 Watopoly squares.
// Useful for verifying:
//   - All squares are initialized
//   - onLand() is functioning polymorphically
//   - Stub behavior is triggered correctly
int main() {
    Board board;
    Player* testPlayer = new Player("TestPlayer", "T");

    std::cout << "=== WATOPOLY BOARD LANDING TEST ===\n\n";

    for (int pos = 0; pos < 40; ++pos) {
        Square* square = board.getSquare(pos);
        std::cout << "[Square " << pos << "] ";
        square->onLand(testPlayer);
        std::cout << "-------------------------------------\n";
    }

    delete testPlayer;
    return 0;
}
*/

