module Display;

import <iostream>;
import <iomanip>;
import <string>;
import <vector>;

using namespace std;

// Helper: produce a string of 'count' 'I' characters for improvements
static string makeImprovements(int count) {
    if (count <= 0) return "";
    return string(count, 'I');
}

// A helper to pad or truncate a string to exactly cellWidth characters.
static string fitToWidth(const string &s, int cellWidth) {
    if ((int)s.size() >= cellWidth) return s.substr(0, cellWidth);
    return s + string(cellWidth - s.size(), ' ');
}

// Print a horizontal separator line for 'count' cells, each of width cellWidth.
static void printSeparator(int count, int cellWidth) {
    for (int i = 0; i < count; i++) {
        cout << "|";
        cout << string(cellWidth, '_');
    }
    cout << "|\n";
}

// Retrieve a string of all player tokens on square index 'sqIndex'.
static string getPlayersOnSquare(int sqIndex, const vector<Player*> &players) {
    string result;
    for (auto p : players) {
        if (p->getPosition() == sqIndex) {
            // Append the token (assuming getToken() returns a std::string)
            result.append(p->getToken());
            result.push_back(' ');
        }
    }
    return result;
}

Display::Display() {}
Display::~Display() {}

void Display::printGameBoard(const Board &board, const vector<Player*> &players) {
    // Build a vector of 40 Square* by calling board.getSquare(i)
    vector<Square*> squares;
    for (int i = 0; i < 40; i++) {
        Square* sq = board.getSquare(i);
        if (!sq) {
            cerr << "Error: Board does not have a square at index " << i << ".\n";
            return;
        }
        squares.push_back(sq);
    }

    // Use a Monopoly-like perimeter layout:
    //   Top row: indices 20..30 (left to right)
    //   Right column: indices 31..39 (top to bottom)
    //   Bottom row: indices 10..0 (right to left)
    //   Left column: indices 19..11 (bottom to top)
    const int cellWidth = 9;

    // 1) Top row: squares 20..30
    printSeparator(11, cellWidth);

    auto printRow = [&](int start, int end, bool leftToRight) {
        int step = (start <= end) ? 1 : -1;

        // (a) Line: improvements (or abbreviated name)
        cout << "|";
        for (int i = start; i != end + step; i += step) {
            string topLine;
            if (auto *ab = dynamic_cast<AcademicBuilding*>(squares[i])) {
                int lvl = ab->getImprovementCount(); // Changed from getCurrentLevel()
                topLine = makeImprovements(lvl);
            } else {
                string nm = squares[i]->getName();
                if ((int)nm.size() > cellWidth) nm = nm.substr(0, cellWidth);
                topLine = nm;
            }
            cout << fitToWidth(topLine, cellWidth) << "|";
        }
        cout << "\n";

        // (b) Line: dashes
        cout << "|";
        for (int i = start; i != end + step; i += step) {
            cout << fitToWidth("-------", cellWidth) << "|";
        }
        cout << "\n";

        // (c) Line: full square name
        cout << "|";
        for (int i = start; i != end + step; i += step) {
            string nm = squares[i]->getName();
            cout << fitToWidth(nm, cellWidth) << "|";
        }
        cout << "\n";

        // (d) Line: players on each square
        cout << "|";
        for (int i = start; i != end + step; i += step) {
            string pStr = getPlayersOnSquare(i, players);
            cout << fitToWidth(pStr, cellWidth) << "|";
        }
        cout << "\n";

        // Separator line.
        printSeparator((end - start) * step + 1, cellWidth);
    };

    // Print top row (indices 20 to 30, left-to-right)
    printRow(20, 30, true);

    // 2) Middle area: left column (indices 19..11 descending) and right column (indices 31..39 ascending).
    auto printMiddleRow = [&](int leftIdx, int rightIdx) {
        auto topLineOf = [&](int idx) -> string {
            if (auto *ab = dynamic_cast<AcademicBuilding*>(squares[idx])) {
                int lvl = ab->getImprovementCount();
                return makeImprovements(lvl);
            } else {
                string nm = squares[idx]->getName();
                if ((int)nm.size() > cellWidth) nm = nm.substr(0, cellWidth);
                return nm;
            }
        };

        // Line 1: improvements/short name for left and right.
        cout << "|";
        cout << fitToWidth(topLineOf(leftIdx), cellWidth) << "|";
        cout << string(cellWidth * 8, ' ');
        cout << "|";
        cout << fitToWidth(topLineOf(rightIdx), cellWidth) << "|\n";

        // Line 2: full square names.
        auto fullNameOf = [&](int idx) {
            return fitToWidth(squares[idx]->getName(), cellWidth);
        };
        cout << "|";
        cout << fullNameOf(leftIdx) << "|";
        cout << string(cellWidth * 8, ' ');
        cout << "|";
        cout << fullNameOf(rightIdx) << "|\n";

        // Line 3: players.
        auto playersOf = [&](int idx) {
            return fitToWidth(getPlayersOnSquare(idx, players), cellWidth);
        };
        cout << "|";
        cout << playersOf(leftIdx) << "|";
        cout << string(cellWidth * 8, ' ');
        cout << "|";
        cout << playersOf(rightIdx) << "|\n";

        // Line 4: separator for these cells.
        cout << "|";
        cout << string(cellWidth, '_') << "|";
        cout << string(cellWidth * 8, ' ');
        cout << "|";
        cout << string(cellWidth, '_') << "|\n";
    };

    // Print 9 middle rows.
    for (int row = 0; row < 9; row++) {
        int leftIdx = 19 - row;    // Descending from 19 to 11.
        int rightIdx = 31 + row;   // Ascending from 31 to 39.
        printMiddleRow(leftIdx, rightIdx);
    }

    // 3) Bottom row: indices 10..0 (right-to-left)
    printRow(10, 0, false);

    // 4) Finally, print player status.
    cout << "\n=== PLAYER STATUS ===\n";
    for (auto p : players) {
        cout << "Player " << p->getToken()
             << " (" << p->getName() << ") has $"
             << p->getMoney() << "\n";
    }
    cout << "=====================\n\n";
}

