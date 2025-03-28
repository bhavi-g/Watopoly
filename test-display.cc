import new_Display;
import Board;
import Player;
import <vector>;
import <iostream>;
using namespace std;

int main() {
    // Create the board (Board default constructor creates all 40 squares)
    Board board;
    
    // Create some test players.
    // Player constructor signature: Player(std::string name, std::string token, int startMoney = 1500)
    vector<Player*> players;
    players.push_back(new Player("Alice", "A", 1500));
    players.push_back(new Player("Bob", "B", 1500));
    players.push_back(new Player("Charlie", "C", 1500));
    
    // Set their positions using moveTo() (since position is private)
    players[0]->moveTo(20);   // Place Alice on square index 20 (top row)
    players[1]->moveTo(39);   // Place Bob on square index 39 (right column)
    players[2]->moveTo(10);   // Place Charlie on square index 10 (bottom row)
    
    // Create a Display object and print the game board.
    new_Display disp;
    cout << "Initial board:\n";
    disp.printGameBoard(board, players);
    
    // Move players to new positions to test updating the display.
    players[1]->moveTo(5);   // Move Bob to square index 5
    players[2]->moveTo(25);  // Move Charlie to square index 25
    
    cout << "\n--- After moving some players ---\n\n";
    disp.printGameBoard(board, players);
    
    // Clean up player memory.
    for (auto p : players) {
        delete p;
    }
    
    return 0;
}
