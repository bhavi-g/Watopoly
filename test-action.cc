// test-action.cc
import <iostream>;
import <vector>;
import Board;
import GameController;
import Player;

int main() {
    std::cout << "=== ACTION SQUARES TEST ===\n\n";

    GameController controller;
    Board* board = new Board();
    controller.setBoard(board);

    Player* vyomm = new Player("Vyomm", "V");
    controller.addPlayer(vyomm);

    // All non-ownable action squares and the steps to land on them
    struct Test {
        std::string name;
        int startPos;
        std::pair<int, int> roll;
        std::string explanation;
    };

    std::vector<Test> tests = {
        {"Collect OSAP", 38, {1, 1}, "passing 0 and landing on 0"},
        {"SLC (2)", 0, {1, 1}, "landing on position 2"},
        {"Tuition", 2, {1, 1}, "landing on position 4"},
        {"Needles Hall (7)", 5, {1, 1}, "landing on position 7"},
        {"DC Tims Line", 8, {1, 1}, "landing on position 10"},
        {"SLC (17)", 15, {1, 1}, "landing on position 17"},
        {"Goose Nesting", 18, {1, 1}, "landing on position 20"},
        {"Needles Hall (22)", 20, {1, 1}, "landing on position 22"},
        {"Go To Tims", 28, {1, 1}, "landing on position 30"},
        {"SLC (33)", 31, {1, 1}, "landing on position 33"},
        {"Needles Hall (36)", 34, {1, 1}, "landing on position 36"},
        {"Coop Fee", 36, {1, 1}, "landing on position 38"}
    };

    for (int i = 0; i < tests.size(); ++i) {
        std::cout << "\n=== TEST " << i + 1 << ": " << tests[i].name << " ===\n";
        std::cout << "[Setup] " << tests[i].explanation << "\n";
        vyomm->moveTo(tests[i].startPos);
        controller.playTurn(vyomm, tests[i].roll);  // simulate forced roll
    }

    delete vyomm;
    delete board;

    std::cout << "\n=== END OF ACTION SQUARES TEST ===\n";
    return 0;
}
