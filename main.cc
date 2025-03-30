// main.cc
import <iostream>;
import <string>;
import <vector>;
import <map>;
import <set>;
import <ctime>;
import Player;
import Board;
import GameController;
import AcademicBuilding; // Needed for dynamic_cast

int main() {
    std::srand(static_cast<unsigned>(time(nullptr)));

    Board board;
    GameController controller;
    controller.setBoard(&board);

    const std::map<std::string, char> validPlayers = {
        {"Goose", 'G'},
        {"GRTBus", 'B'},
        {"TimHortonsDoughnut", 'D'},
        {"Professor", 'P'},
        {"Student", 'S'},
        {"Money", '$'},
        {"Laptop", 'L'},
        {"PinkTie", 'T'}
    };

    int numPlayers = 0;
    std::cout << "Enter number of players (2–8): ";
    while (numPlayers < 2 || numPlayers > 8) {
        std::cin >> numPlayers;
        if (numPlayers < 2 || numPlayers > 8)
            std::cout << "Invalid number. Please enter 2–8 players: ";
    }

    std::vector<Player*> players;
    std::set<std::string> usedNames;

    std::cout << "\nAvailable Player Pieces:\n";
    for (const auto& [name, token] : validPlayers) {
        std::cout << "  " << name << " (" << token << ")\n";
    }

    for (int i = 0; i < numPlayers; ++i) {
        std::string name;
        std::cout << "\nChoose player " << (i + 1) << " from the list above (case-sensitive): ";
        std::cin >> name;

        while (!validPlayers.count(name) || usedNames.count(name)) {
            std::cout << "Invalid or already chosen. Choose another: ";
            std::cin >> name;
        }

        usedNames.insert(name);
        char token = validPlayers.at(name);
        Player* p = new Player(name, std::string(1, token));
        controller.addPlayer(p);
        players.push_back(p);
    }

    int current = 0;
    while (true) {
        if (players[current]->isBankrupt()) {
            current = (current + 1) % numPlayers;
            continue;
        }

        Player* p = players[current];
        std::string command;
        bool rolled = false;

        std::cout << "\nIt's " << p->getName() << "'s turn. Type command: ";

        while (true) {
            std::cin >> command;

            if (command == "roll" && !rolled) {
                controller.playTurn(p);
                rolled = true;
                break;
            } else if (command == "next" && rolled) {
                break;
            } else if (command == "trade") {
                std::string to, give, receive;
                std::cin >> to >> give >> receive;
                controller.trade(p->getToken(), give, to, receive);
            } else if (command == "improve") {
                std::string property, action;
                std::cin >> property >> action;
                if (auto* ab = dynamic_cast<AcademicBuilding*>(controller.getBuilding(property))) {
                    if (action == "buy") controller.improveBuilding(p, ab);
                    else if (action == "sell") controller.degradeBuilding(p, ab);
                }
            } else if (command == "mortgage") {
                std::string property;
                std::cin >> property;
                controller.mortgageBuilding(p, controller.getBuilding(property));
            } else if (command == "unmortgage") {
                std::string property;
                std::cin >> property;
                controller.unmortgageBuilding(p, controller.getBuilding(property));
            } else if (command == "assets") {
                controller.printAssets(p);
            } else if (command == "all") {
                for (auto* player : players) {
                    controller.printAssets(player);
                    std::cout << "\n";
                }
            } else if (command == "bankrupt") {
                controller.declareBankruptcy(p, nullptr);
                break;
            } else if (command == "save") {
                std::string filename;
                std::cin >> filename;
                std::cout << "[TODO] Save game to: " << filename << "\n";
            } else {
                std::cout << "Unknown or invalid command. Try again.\n";
            }

            std::cout << "Command: ";
        }

        int active = 0;
        Player* last = nullptr;
        for (auto* pl : players) {
            if (!pl->isBankrupt()) {
                ++active;
                last = pl;
            }
        }

        if (active == 1) {
            std::cout << "\n🎉 " << last->getName() << " wins the game! 🎉\n";
            break;
        }

        current = (current + 1) % numPlayers;
    }

    for (auto* p : players) delete p;
    return 0;
}
