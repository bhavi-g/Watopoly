/*

// test-main.cc
// Purpose:
//   Basic integration test for Watopoly core functionality.
//   Verifies polymorphic behavior of Square::onLand(Player*),
//   player ownership assignment, and building-type specific messaging.
//
//   Tests the interaction between:
//     - GameController
//     - Player
//     - AcademicBuilding
//     - Residence
//     - Gym
//
// Test Overview:
//   1. Instantiate two players
//   2. Instantiate three different building types
//   3. Simulate initial landings on unowned buildings
//   4. Manually assign ownership to players
//   5. Simulate landings on owned buildings to verify rent/personal messages
//   6. Clean up heap allocations

import <iostream>;
import Player;
import AcademicBuilding;
import Residence;
import Gym;
import GameController;

int main() {
    // Create central controller to manage game logic
    GameController controller;

    // Create two test players
    Player* p1 = new Player("Vyomm", "V");   // Player 1, token "V"
    Player* p2 = new Player("Bhavish", "B"); // Player 2, token "B"

    controller.addPlayer(p1);
    controller.addPlayer(p2);

    // Create three types of buildings
    AcademicBuilding* ev1 = new AcademicBuilding("EV1", 6, 200, "Env", 150);
    Residence* mkv = new Residence("MKV", 3, 200);
    Gym* pac = new Gym("PAC", 5, 150);

    controller.addBuilding(ev1);
    controller.addBuilding(mkv);
    controller.addBuilding(pac);

    // Simulate landing on unowned buildings
    std::cout << "\n-- Simulating landings (initially unowned) --\n";
    ev1->onLand(p1);  // Vyomm lands on EV1
    mkv->onLand(p2);  // Bhavish lands on MKV
    pac->onLand(p1);  // Vyomm lands on PAC

    // Assign ownership manually
    ev1->setOwnerToken(p1->getToken()); // Vyomm now owns EV1
    mkv->setOwnerToken(p2->getToken()); // Bhavish owns MKV
    pac->setOwnerToken(p1->getToken()); // Vyomm owns PAC

    // Simulate landing on owned buildings
    std::cout << "\n-- Simulating landings after ownership --\n";
    ev1->onLand(p2);  // Bhavish lands on Vyomm's academic building (EV1)
    mkv->onLand(p1);  // Vyomm lands on Bhavish's residence (MKV)
    pac->onLand(p1);  // Vyomm lands on his own gym (PAC)

    // Clean up heap memory
    delete p1;
    delete p2;
    delete ev1;
    delete mkv;
    delete pac;

    return 0;
}

*/
