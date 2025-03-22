// test-turn.cc
import <iostream>;
import Board;
import GameController;
import Player;
import AcademicBuilding;
import Residence;
import Gym;
import Building;
import <cstdlib>;
import <ctime>;

int main() {
    std::srand(std::time(nullptr));
    std::cout << "=== WATOPOLY: Turn Simulation ===\n\n";

    // ---------- Game Setup ----------
    GameController controller;
    Board* board = new Board();
    controller.setBoard(board);

    Player* p1 = new Player("Vyomm", "V");
    Player* p2 = new Player("Bhavish", "B");

    controller.addPlayer(p1);
    controller.addPlayer(p2);

    // ---------- Manually assign a few properties ----------
auto* ev1 = dynamic_cast<Building*>(board->getSquareByName("EV1"));
auto* mkv = dynamic_cast<Building*>(board->getSquareByName("MKV"));
auto* pac = dynamic_cast<Building*>(board->getSquareByName("PAC"));

    ev1->setOwnerToken(p1->getToken());
    p1->addProperty(ev1->getName());

    mkv->setOwnerToken(p2->getToken());
    p2->addProperty(mkv->getName());

    pac->setOwnerToken(p1->getToken());
    p1->addProperty(pac->getName());

    std::cout << "--- Assigned EV1 and PAC to Vyomm, MKV to Bhavish ---\n\n";

    // ---------- Play 3 turns each ----------
    for (int turn = 1; turn <= 3; ++turn) {
        std::cout << "======= TURN " << turn << " =======\n";
        controller.playTurn(p1);
        controller.playTurn(p2);
        std::cout << "======================\n\n";
    }

    // ---------- Cleanup ----------
    delete p1;
    delete p2;
    delete board;

    return 0;
}
