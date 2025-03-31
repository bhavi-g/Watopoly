// main.cc
import <iostream>;
import <fstream>;
import <sstream>;
import <string>;
import <vector>;
import <map>;
import <set>;
import <ctime>;
import Player;
import Board;
import GameController;
import AcademicBuilding; // Needed for dynamic_cast
import new_Display;
import Building;

int main(int argc, char* argv[]) {
    std::srand(static_cast<unsigned>(time(nullptr)));

    Board board;
    GameController controller;
    controller.setBoard(&board);
    new_Display disp;

    const std::map<char, std::string> validTokens = {
        {'G', "Goose"},
        {'B', "GRT Bus"},
        {'D', "Tim Hortons Doughnut"},
        {'P', "Professor"},
        {'S', "Student"},
        {'$', "Money"},
        {'L', "Laptop"},
        {'T', "Pink tie"}
    };

    std::vector<Player*> players;
    std::set<char> usedTokens;
    int current = 0;

    bool testingMode = false;
    std::string loadFile;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-testing") {
            testingMode = true;
        } else if (std::string(argv[i]) == "-load" && i + 1 < argc) {
            loadFile = argv[i + 1];
        }
    }

    if (!loadFile.empty()) {
        std::ifstream in(loadFile);
        if (!in) {
            std::cerr << "Error opening file: " << loadFile << "\n";
            return 1;
        }

        int numPlayers;
        in >> numPlayers;

        for (int i = 0; i < numPlayers; ++i) {
            std::string name;
            char token;
            int cups, money, pos;
            in >> name >> token >> cups >> money >> pos;

            Player* p = new Player(name, std::string(1, token));
            p->setRollUpCups(cups);
            p->setMoney(money);
            p->moveTo(pos);

            if (pos == 10) {
                int inTims;
                in >> inTims;
                if (inTims == 1) {
                    int turns;
                    in >> turns;
                    p->setInTims(true);
                    for (int j = 0; j < turns; ++j) p->incrementTimsTurn();
                }
            }

            usedTokens.insert(token);
            controller.addPlayer(p);
            players.push_back(p);
        }

        std::string propertyName, owner;
        int improvements;
        while (in >> propertyName >> owner >> improvements) {
            auto* b = controller.getBuilding(propertyName);
            if (!b) continue;

            if (owner != "BANK") {
                for (auto* p : players) {
                    if (p->getName() == owner) {
                        p->addProperty(b->getName());
                        b->setOwnerToken(p->getToken());
                        break;
                    }
                }
            }

            if (improvements == -1) {
                controller.mortgageBuilding(nullptr, b);
            } else if (auto* ab = dynamic_cast<AcademicBuilding*>(b)) {
                ab->forceSetImprovements(improvements);
            }
        }
    } else {
        int numPlayers = 0;
        std::cout << "Enter number of players (2–8): ";
        while (numPlayers < 2 || numPlayers > 8) {
            std::cin >> numPlayers;
            if (numPlayers < 2 || numPlayers > 8)
                std::cout << "Invalid number. Please enter 2–8 players: ";
        }

        std::cout << "\nAvailable Tokens:\n";
        for (const auto& [token, name] : validTokens) {
            std::cout << "  " << name << " (" << token << ")\n";
        }

        for (int i = 0; i < numPlayers; ++i) {
            std::string name;
            char token;

            std::cout << "\nEnter name for Player " << (i + 1) << ": ";
            std::cin >> name;

            std::cout << "Choose a token (single character from list above): ";
            std::cin >> token;

            while (!validTokens.count(token) || usedTokens.count(token)) {
                std::cout << "Invalid or already chosen. Choose another: ";
                std::cin >> token;
            }

            usedTokens.insert(token);
            Player* p = new Player(name, std::string(1, token));
            controller.addPlayer(p);
            players.push_back(p);
        }
    }


    disp.printGameBoard(board, players);

    while (true) {
        if (players[current]->isBankrupt()) {
            current = (current + 1) % players.size();
            continue;
        }

        Player* p = players[current];
        std::string command;
        bool rolled = false;

        std::cout << "\nIt's " << p->getName() << "'s turn. Type command: ";

        while (true) {
            std::cin >> command;

            if (command == "roll" && !rolled) {
                if (testingMode) {
                    std::string next;
                    std::getline(std::cin, next);
                    std::istringstream iss(next);
                    int d1, d2;
                    if (iss >> d1 >> d2) {
                        controller.playTurn(p, std::pair{d1, d2});
                    } else {
                        controller.playTurn(p); // fallback to default roll
                    }
                } else {
                    controller.playTurn(p);
                }
                rolled = true;
                disp.printGameBoard(board, players);
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
                std::ofstream out(filename);
                if (!out) {
                    std::cerr << "Error writing to file: " << filename << "\n";
                    continue;
                }

                // Save players
                out << players.size() << "\n";
                for (auto* pl : players) {
                    out << pl->getName() << " "
                        << pl->getToken() << " "
                        << pl->getRollUpCups() << " "
                        << pl->getMoney() << " "
                        << pl->getPosition();

                    if (pl->getPosition() == 10 && pl->isInTims()) {
                        out << " 1 " << pl->getTimsTurns();
                    }

                    out << "\n";
                }

                // Save buildings
                for (int i = 0; i < 40; ++i) {
                    auto* b = board.getSquare(i);
                    auto* building = dynamic_cast<Building*>(b);
                    if (!building) continue;
                    if (auto* building = dynamic_cast<Building*>(b)) {
                        std::string owner = "BANK";
                        if (!building->getOwnerToken().empty()) {
                            Player* ownerPlayer = controller.getPlayer(building->getOwnerToken());
                            if (ownerPlayer) {
                                owner = ownerPlayer->getName();
                            }
                        }
                        int improvements = 0;

                        if (building->isMortgaged()) {
                            improvements = -1;
                        } else if (auto* ab = dynamic_cast<AcademicBuilding*>(building)) {
                            improvements = ab->getImprovementCount();
                        }

                        out << building->getName() << " " << owner << " " << improvements << "\n";
                    }           
                }

                std::cout << "[✓] Game saved to: " << filename << "\n";
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

        current = (current + 1) % players.size();
    }

    for (auto* p : players) delete p;
    return 0;
}
