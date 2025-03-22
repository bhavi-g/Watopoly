// test-rent.cc
import <iostream>;
import GameController;
import Board;
import Player;
import Building;
import LandAction;
import AcademicBuilding;
import Residence;
import Gym;
import Square;

int main() {
    std::cout << "=== RENT TEST ===\n";

    // ---------- Setup ----------
    GameController controller;
    Board* board = new Board();
    controller.setBoard(board);

    Player* p1 = new Player("Vyomm", "V");   // Owner
    Player* p2 = new Player("Bhavish", "B"); // Renter

    controller.addPlayer(p1);
    controller.addPlayer(p2);

    // Grab three buildings: one of each type
    auto* dc  = dynamic_cast<AcademicBuilding*>(board->getSquareByName("DC"));
    auto* mkv = dynamic_cast<Residence*>(board->getSquareByName("MKV"));
    auto* pac = dynamic_cast<Gym*>(board->getSquareByName("PAC"));

    // Assign ownership of all to Vyomm
    dc->setOwnerToken(p1->getToken());
    p1->addProperty(dc->getName());

    mkv->setOwnerToken(p1->getToken());
    p1->addProperty(mkv->getName());

    pac->setOwnerToken(p1->getToken());
    p1->addProperty(pac->getName());

    // ---------- Simulate Bhavish landing on each ----------
    auto landOn = [&](Square* sqr) {
    std::cout << "\n--- Bhavish lands on " << sqr->getName() << " ---\n";
    int target = sqr->getPosition();
    int current = p2->getPosition();
    int steps = (target - current + 40) % 40;

    std::cout << "[BEFORE] Bhavish: $" << p2->getMoney()
              << " | Vyomm: $" << p1->getMoney() << "\n";

    p2->move(steps);  // Move Bhavish directly to the square

    Square* landed = board->getSquare(p2->getPosition());
    LandAction action = landed->onLand(p2);

    switch (action) {
        case LandAction::PayRent: {
            auto* b = dynamic_cast<Building*>(landed);
            if (!b) break;

            int rent = 0;

            if (auto* res = dynamic_cast<Residence*>(b)) {
                int count = controller.getResidenceCount(b->getOwnerToken());
                rent = res->calculateRent(count);
            } else if (dynamic_cast<Gym*>(b)) {
                int numGymsOwned = controller.getGymCount(b->getOwnerToken());
                rent = numGymsOwned * 8;  // Simulate fixed dice roll = 8
            } else {
                rent = b->calculateRent(0);  // AcademicBuilding uses internal improvements
            }

            std::cout << "[Controller]: Bhavish must pay $" << rent << " in rent.\n";
            p2->pay(rent);
            controller.getPlayer(b->getOwnerToken())->receive(rent);
            break;
        }

        default:
            std::cout << "[Controller]: No rent triggered.\n";
            break;
    }

    std::cout << "[AFTER ] Bhavish: $" << p2->getMoney()
              << " | Vyomm: $" << p1->getMoney() << "\n";
};

    landOn(dc);   // AcademicBuilding (rent based on improvements)
    landOn(mkv);  // Residence (rent based on # owned)
    landOn(pac);  // Gym (rent based on dice roll and ownership)

    // ---------- Cleanup ----------
    delete p1;
    delete p2;
    delete board;

    return 0;
}
