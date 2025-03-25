#include "Display.h"
#include "Board.h"              // Board should have: std::vector<Square*> getSquares() const;
#include "Square.h"             // Square should have: std::string getName() const;
#include "Player.h"             // Player should have: getPosition(), getToken(), getName(), getMoney()
#include "AcademicBuilding.h"   // If you need to print improvements via dynamic_cast
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

// Helper: produce a string of 'count' 'I' characters for improvements
static string makeImprovements(int count) {
    if (count <= 0) return "";
    return string(count, 'I');
}

Display::Display() {}
Display::~Display() {}

// A small helper to pad or truncate a string to fit cellWidth
static string fitToWidth(const string &s, int cellWidth) {
    if ((int)s.size() >= cellWidth) return s.substr(0, cellWidth);
    return s + string(cellWidth - s.size(), ' ');
}

// Print a horizontal separator line for 'count' squares, each cell is cellWidth wide
static void printSeparator(int count, int cellWidth) {
    for (int i = 0; i < count; i++) {
        cout << "|";
        // We’ll do underscores to form the bottom edge
        cout << string(cellWidth, '_');
    }
    cout << "|\n";
}

// Retrieve the string of player tokens that occupy square index 'sqIndex'
static string getPlayersOnSquare(int sqIndex, const vector<Player*> &players) {
    string result;
    for (auto p : players) {
        if (p->getPosition() == sqIndex) {
            // Append the token and a space
            result.push_back(p->getToken());
            result.push_back(' ');
        }
    }
    return result;
}

void Display::printGameBoard(const Board &board, const std::vector<Player*> &players) {
    // Retrieve all 40 squares in order
    vector<Square*> squares = board.getSquares();
    if (squares.size() != 40) {
        cerr << "Error: Board does not have exactly 40 squares.\n";
        return;
    }

    // We’ll do a standard Monopoly-like perimeter:
    //  Top row: indices 20..30 (left to right)
    //  Right column: indices 31..39 (top to bottom)
    //  Bottom row: indices 10..0 (right to left)
    //  Left column: indices 19..11 (bottom to top) interspersed in the middle.

    // For simplicity, let's define a cell width
    const int cellWidth = 9;

    // 1) Print a top “border” line (underscores) for the top row (11 squares: 20..30 inclusive).
    printSeparator(11, cellWidth);

    // 2) Print top row squares (20..30)
    //    We'll do two lines for each row of squares:
    //    (a) line of improvements or short name
    //    (b) line of dashes or partial name
    //    (c) line of full name
    //    (d) line of players
    // Then a separator.

    auto printRow = [&](int start, int end, bool leftToRight) {
        // We do four lines, then a separator.
        // Because start..end can go either forward or backward, we decide step:
        int step = (start <= end) ? 1 : -1;

        // (a) improvements or short name
        cout << "|";
        for (int i = start; i != end + step; i += step) {
            // If this square is an AcademicBuilding, show improvement level
            string topLine;
            if (auto *ab = dynamic_cast<AcademicBuilding*>(squares[i])) {
                int lvl = ab->getCurrentLevel(); // or ab->getImprovements()
                topLine = makeImprovements(lvl);
            } else {
                // For non-building squares, we just take first 8 chars of name
                string nm = squares[i]->getName();
                if ((int)nm.size() > cellWidth) nm = nm.substr(0, cellWidth);
                topLine = nm;
            }
            cout << fitToWidth(topLine, cellWidth) << "|";
        }
        cout << "\n";

        // (b) a line of dashes or partial name
        cout << "|";
        for (int i = start; i != end + step; i += step) {
            // We can just do "-------"
            cout << fitToWidth("-------", cellWidth) << "|";
        }
        cout << "\n";

        // (c) the actual full square name
        cout << "|";
        for (int i = start; i != end + step; i += step) {
            string nm = squares[i]->getName();
            // If it's longer than cellWidth, we might wrap or just truncate
            cout << fitToWidth(nm, cellWidth) << "|";
        }
        cout << "\n";

        // (d) the players
        cout << "|";
        for (int i = start; i != end + step; i += step) {
            // gather all players whose getPosition() == i
            string pStr = getPlayersOnSquare(i, players);
            cout << fitToWidth(pStr, cellWidth) << "|";
        }
        cout << "\n";

        // Then a separator
        printSeparator((end - start) * step + 1, cellWidth);
    };

    // Print top row: squares 20..30 left to right
    printRow(20, 30, true);

    // 3) Middle area: left column = 19..11 descending, right column = 31..39 ascending
    //    We have 9 squares on each side, so we’ll print them in “rows.” 
    //    Each “row” is one square from the left, big space, one square from the right.

    // We'll define a mini-function to print exactly one square in that multi-line style
    // but more compact. For brevity, we’ll do 2 lines per square: 
    //   line of improvements/short name
    //   line of players
    // (You can expand to match the top row style if desired.)

    auto printMiddleRow = [&](int leftIdx, int rightIdx) {
        // line 1: improvements or short name for left, big gap, improvements or short name for right
        // We'll define a small lambda to get the topLine for a single square:
        auto topLineOf = [&](int idx) {
            if (auto *ab = dynamic_cast<AcademicBuilding*>(squares[idx])) {
                int lvl = ab->getCurrentLevel();
                return makeImprovements(lvl);
            } else {
                string nm = squares[idx]->getName();
                if ((int)nm.size() > cellWidth) nm = nm.substr(0, cellWidth);
                return nm;
            }
        };

        // line 1
        cout << "|";
        cout << fitToWidth(topLineOf(leftIdx), cellWidth);
        cout << "|";
        // big space in the middle for ~ 9 squares worth = 9*(cellWidth+1) if you want
        cout << string(cellWidth * 8, ' '); 
        cout << "|";
        cout << fitToWidth(topLineOf(rightIdx), cellWidth);
        cout << "|\n";

        // line 2: square name for left, square name for right
        auto fullNameOf = [&](int idx) {
            return fitToWidth(squares[idx]->getName(), cellWidth);
        };

        cout << "|";
        cout << fullNameOf(leftIdx);
        cout << "|";
        cout << string(cellWidth * 8, ' ');
        cout << "|";
        cout << fullNameOf(rightIdx);
        cout << "|\n";

        // line 3: players on left, players on right
        auto playersOf = [&](int idx) {
            return fitToWidth(getPlayersOnSquare(idx, players), cellWidth);
        };
        cout << "|";
        cout << playersOf(leftIdx);
        cout << "|";
        cout << string(cellWidth * 8, ' ');
        cout << "|";
        cout << playersOf(rightIdx);
        cout << "|\n";

        // line 4: a separator for each side
        cout << "|";
        cout << string(cellWidth, '_');
        cout << "|";
        cout << string(cellWidth * 8, ' ');
        cout << "|";
        cout << string(cellWidth, '_');
        cout << "|\n";
    };

    // We have 9 squares on each side to print
    for (int row = 0; row < 9; row++) {
        int leftIdx = 19 - row;   // 19 down to 11
        int rightIdx = 31 + row; // 31 up to 39
        printMiddleRow(leftIdx, rightIdx);
    }

    // 4) Bottom row: squares 10..0 (right to left)
    printRow(10, 0, false);

    // Finally, print each player’s money, etc.
    cout << "\n=== PLAYER STATUS ===\n";
    for (auto p : players) {
        cout << "Player " << p->getToken() 
             << " (" << p->getName() << ") has $"
             << p->getMoney() << endl;
    }
    cout << "=====================\n\n";
}
