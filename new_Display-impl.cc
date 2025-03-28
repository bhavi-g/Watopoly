module new_Display;

import Board;
import Square;
import Player;
import AcademicBuilding;

import <iostream>;
import <iomanip>;
import <string>;
import <vector>;

using namespace std;

new_Display::new_Display() {}
new_Display::~new_Display() {}

std::string getPlayerString(int squareIndex, const std::vector<Player*> &players) {
    std::string result = "";
    for (Player* p : players) {
        if (p->getPosition() == squareIndex) {
            result += p->getToken();  // Assuming getToken() returns char/string representation
        }
    }

    while (result.length() < 7) {
        result += ' ';
    }

    return result;
}

std::string getImprovementString(const Square *sq) {
    auto *ac = dynamic_cast<const AcademicBuilding *>(sq);
    if (!ac) return "       ";
    int count = ac->getImprovementCount();
    return std::string(count, 'I') + std::string(7 - count, ' ');
}

void new_Display::printGameBoard(const Board &board, const std::vector<Player*> &players) {


    int improv = 0;
    int nspace = 0;


    // line 1
    cout << "-----------------------------------------------------------------------------------------" << endl;

    // line 2
    for (int i = 20; i <= 30; ++i) {
        const std::string &name = board.getSquare(i)->getName();
        if (name == "Goose Nesting") cout << "|Goose  ";
        else if (name == "NEEDLES HALL") cout << "|NEEDLES";
        else if (name == "V1") cout << "|V1     ";
        else if (name == "CIF") cout << "|CIF    ";
        else if (name == "GO TO TIMS") cout << "|GO TO  ";
        else cout << "|" << getImprovementString(board.getSquare(i));
    }
    cout << "|" << endl;


    // line 3
    for (int i = 20; i <= 30; ++i) {
        const std::string &name = board.getSquare(i)->getName();
        if (name == "Goose Nesting") cout << "|Nesting";
        else if (name == "NEEDLES HALL") cout << "|HALL   ";
        else if (name == "V1" || name == "CIF") cout << "|       ";
        else if (name == "GO TO TIMS") cout << "|TIMS   ";
        else cout << "|-------";
    }
    cout << "|" << endl;

    // line 4
    for (int i = 20; i <= 30; ++i) {
    const std::string &name = board.getSquare(i)->getName();
        if (name == "Goose Nesting" || name == "NEEDLES HALL" ||
            name == "V1" || name == "CIF" || name == "GO TO TIMS") {
            cout << "|       ";
        } else {
            cout << "|" << left << setw(7) << setfill(' ') << name;
        }
    }
    cout << "|" << endl;


    // line 5: players
    for (int i = 20; i <= 30; ++i) {
        cout << "|" << getPlayerString(i, players);
    }
    cout << "|" << endl;

    // line 6: underscores
    for (int i = 0; i < 11; ++i) {
        cout << "|_______";
    }
    cout << "|" << endl;

    // line 7
    cout << "|";
    const Square *sqLeft = board.getSquare(19);
    const AcademicBuilding *ac1 = dynamic_cast<const AcademicBuilding *>(sqLeft);
    if (ac1) {
        improv = ac1->getImprovementCount();
        for (int j = 0; j < improv; ++j) cout << "I";
        nspace = 7 - improv;
        for (int x = 0; x < nspace; ++x) cout << " ";
    } else {
        cout << "       ";
    }

    cout << "|                                                                       |";

    const Square *sqRight = board.getSquare(31);
    const AcademicBuilding *ac2 = dynamic_cast<const AcademicBuilding *>(sqRight);
    if (ac2) {
        improv = ac2->getImprovementCount();
        for (int j = 0; j < improv; ++j) cout << "I";
        nspace = 7 - improv;
        for (int x = 0; x < nspace; ++x) cout << " ";
    } else {
        cout << "       ";
    }

    cout << "|" << endl;
    // line 8
    cout << "|-------|                                                                       |-------|" << endl;

    // line 9
    cout << "|OPT    |                                                                       |EIT    |" << endl;
    // line 10 : display players board[19], board[31]
    cout << "|" << getPlayerString(19, players)
    << "|                                                                       |"
    << getPlayerString(31, players) << "|" << endl;

    // line 11
    cout << "|_______|                                                                       |_______|" << endl;

    // line 12
    cout << "|";
    const Square *sqLeft12 = board.getSquare(18);
    const AcademicBuilding *ac3 = dynamic_cast<const AcademicBuilding *>(sqLeft12);
    if (ac3) {
        improv = ac3->getImprovementCount();
        for (int j = 0; j < improv; ++j) cout << "I";
        nspace = 7 - improv;
        for (int x = 0; x < nspace; ++x) cout << " ";
    } else {
        cout << "       ";
    }

    cout << "|                                                                       |";

    const Square *sqRight12 = board.getSquare(32);
    const AcademicBuilding *ac4 = dynamic_cast<const AcademicBuilding *>(sqRight12);
    if (ac4) {
        improv = ac4->getImprovementCount();
        for (int j = 0; j < improv; ++j) cout << "I";
        nspace = 7 - improv;
        for (int x = 0; x < nspace; ++x) cout << " ";
    } else {
        cout << "       ";
    }

    cout << "|" << endl;

    // line 13
   cout << "|-------|                                                                       |-------|" << endl;
   // line 14
   cout << "|BMH    |                                                                       |ESC    |" << endl;
   

   // line 15
    cout << "|" << getPlayerString(18, players)
        << "|                                                                       |"
        << getPlayerString(32, players) << "|" << endl;

    // line 16
    cout << "|_______|                                                                       |_______|" << endl;
    // line 17
    cout << "|SLC    |                                                                       |SLC    |" << endl;
    // line 18 
    cout << "|       |                                                                       |       |" << endl;
    // line 19
    cout << "|       |                                                                       |       |" << endl;


    // line 30: display players on board[17], board[33]
    cout << "|" << getPlayerString(17, players)
        << "|                                                                       |"
        << getPlayerString(33, players) << "|" << endl;

    // line 31
    cout << "|_______|                                                                       |_______|" << endl;

    // line 32: display improvements on board[16], board[34]
    cout << "|";
    const Square *sqLeft32 = board.getSquare(16);
    const AcademicBuilding *ac5 = dynamic_cast<const AcademicBuilding *>(sqLeft32);
    if (ac5) {
        improv = ac5->getImprovementCount();
        for (int j = 0; j < improv; ++j) cout << "I";
        nspace = 7 - improv;
        for (int x = 0; x < nspace; ++x) cout << " ";
    } else {
        cout << "       ";
    }

    cout << "|                                                                       |";

    const Square *sqRight32 = board.getSquare(34);
    const AcademicBuilding *ac6 = dynamic_cast<const AcademicBuilding *>(sqRight32);
    if (ac6) {
        improv = ac6->getImprovementCount();
        for (int j = 0; j < improv; ++j) cout << "I";
        nspace = 7 - improv;
        for (int x = 0; x < nspace; ++x) cout << " ";
    } else {
        cout << "       ";
    }

    cout << "|" << endl;


    // line 33
    cout << "|-------|                                                                       |-------|" << endl;
    // line 34
    cout << "|LHI    |                                                                       |C2     |" << endl;

    // line 35: display players on board[16], board[34]
    cout << "|" << getPlayerString(16, players)
        << "|             ---------------------------------------------             |"
        << getPlayerString(34, players) << "|" << endl;

    // line 31
    cout << "|_______|            |                                             |            |_______|" << endl;
    // line 32
    cout << "|UWP    |            | #   #  ##  #####  ###  ###   ###  #   #   # |            |REV    |" << endl;
    // line 33
    cout << "|       |            | #   # #  #   #   #   # #  # #   # #   #   # |            |       |" << endl;
    // line 34
    cout << "|       |            | # # # ####   #   #   # ###  #   # #    # #  |            |       |" << endl;

    // line 35: display players on board[15], board[35]
    cout << "|" << getPlayerString(15, players)
     << "|            | # # # #  #   #   #   # #    #   # #     #   |            |"
     << getPlayerString(35, players) << "|" << endl;

    // line 36
    cout << "|_______|            | ##### #  #   #    ###  #     ###  ####  #   |            |_______|" << endl;

    // line 37
    cout << "|";
    const Square *sqLeft37 = board.getSquare(14);
    const AcademicBuilding *ac7 = dynamic_cast<const AcademicBuilding *>(sqLeft37);
    if (ac7) {
        improv = ac7->getImprovementCount();
        for (int j = 0; j < improv; ++j) cout << "I";
        nspace = 7 - improv;
        for (int x = 0; x < nspace; ++x) cout << " ";
    } else {
        cout << "       ";
    }

    cout << "|            |_____________________________________________|            |NEEDLES|" << endl;


    //line 38
    cout << "|-------|                                                                       |HALL   |" << endl;
    // line 39
    cout << "|CPH    |                                                                       |       |" << endl;

    // line 40: display players on board[14], board[36]
    cout << "|" << getPlayerString(14, players)
        << "|                                                                       |"
        << getPlayerString(36, players) << "|" << endl;

    // line 41
    cout << "|_______|                                                                       |_______|" << endl;


    // line 42
    cout << "|";
    const Square *sqLeft42 = board.getSquare(13);
    const AcademicBuilding *ac8 = dynamic_cast<const AcademicBuilding *>(sqLeft42);
    if (ac8) {
        improv = ac8->getImprovementCount();
        for (int j = 0; j < improv; ++j) cout << "I";
        nspace = 7 - improv;
        for (int x = 0; x < nspace; ++x) cout << " ";
    } else {
        cout << "       ";
    }

    cout << "|                                                                       |";

    const Square *sqRight42 = board.getSquare(37);
    const AcademicBuilding *ac9 = dynamic_cast<const AcademicBuilding *>(sqRight42);
    if (ac9) {
        improv = ac9->getImprovementCount();
        for (int j = 0; j < improv; ++j) cout << "I";
        nspace = 7 - improv;
        for (int x = 0; x < nspace; ++x) cout << " ";
    } else {
        cout << "       ";
    }

    cout << "|" << endl;

    // line 43 
    cout << "|-------|                                                                       |-------|" << endl;
    // line 44
    cout << "|DWE    |                                                                       |MC     |" << endl;


    // line 45: display players on board[13], board[37]
    cout << "|" << getPlayerString(13, players)
        << "|                                                                       |"
        << getPlayerString(37, players) << "|" << endl;

    // line 46
    cout << "|_______|                                                                       |_______|" << endl;
    // line 47
    cout << "|PAC    |                                                                       |COOP   |" << endl;
    // line 48
    cout << "|       |                                                                       |FEE    |" << endl;
    // line 49
    cout << "|       |                                                                       |       |" << endl;

    // line 50: display players on board[12], board[38]
    cout << "|" << getPlayerString(12, players)
        << "|                                                                       |"
        << getPlayerString(38, players) << "|" << endl;


    // line 51
   cout << "|_______|                                                                       |_______|" << endl;



   // line 52
    cout << "|";
    const Square *sqLeft52 = board.getSquare(11);
    const AcademicBuilding *ac10 = dynamic_cast<const AcademicBuilding *>(sqLeft52);
    if (ac10) {
        improv = ac10->getImprovementCount();
        for (int j = 0; j < improv; ++j) cout << "I";
        nspace = 7 - improv;
        for (int x = 0; x < nspace; ++x) cout << " ";
    } else {
        cout << "       ";
    }

    cout << "|                                                                       |";

    const Square *sqRight52 = board.getSquare(39);
    const AcademicBuilding *ac11 = dynamic_cast<const AcademicBuilding *>(sqRight52);
    if (ac11) {
        improv = ac11->getImprovementCount();
        for (int j = 0; j < improv; ++j) cout << "I";
        nspace = 7 - improv;
        for (int x = 0; x < nspace; ++x) cout << " ";
    } else {
        cout << "       ";
    }

    cout << "|" << endl;

    // line 53
    cout << "|-------|                                                                       |-------|" << endl;
    // line 54
    cout << "|RCH    |                                                                       |DC     |" << endl;

    // line 55: display players on board[11], board[39]
    cout << "|" << getPlayerString(11, players)
        << "|                                                                       |"
        << getPlayerString(39, players) << "|" << endl;


    // line 56
    cout << "|_______|_______________________________________________________________________|_______|" << endl;


    // line 57
    for (int i = 10; i >= 0; --i) {
        const Square *sq = board.getSquare(i);
        const std::string &name = sq->getName();

        if (name == "DC Tims Line") {
            cout << "|DC Tims";
        } else if (name == "NEEDLES HALL") {
            cout << "|NEEDLES";
        } else if (name == "MKV") {
            cout << "|MKV    ";
        } else if (name == "TUITION") {
            cout << "|TUITION";
        } else if (name == "SLC") {
            cout << "|SLC    ";
        } else if (name == "COLLECT OSAP") {
            cout << "|COLLECT";
        } else {
            cout << "|";
            const AcademicBuilding *ac = dynamic_cast<const AcademicBuilding *>(sq);
            if (ac) {
                improv = ac->getImprovementCount();
                for (int j = 0; j < improv; ++j) cout << "I";
                nspace = 7 - improv;
                for (int x = 0; x < nspace; ++x) cout << " ";
            } else {
                cout << "       ";
            }
        }
    }
    cout << "|" << endl;

    // line 58
    for (int i = 10; i >= 0; --i) {
        const std::string &name = board.getSquare(i)->getName();

        if (name == "DC Tims Line") {
            cout << "|Line   ";
        } else if (name == "NEEDLES HALL") {
            cout << "|HALL   ";
        } else if (name == "MKV" || name == "TUITION" || name == "SLC") {
            cout << "|       ";
        } else if (name == "COLLECT OSAP") {
            cout << "|OSAP   ";
        } else {
            cout << "|-------";
        }
    }
    cout << "|" << endl;

    // line 59
    for (int i = 10; i >= 0; --i) {
        const std::string &name = board.getSquare(i)->getName();

        if (name != "DC Tims Line" &&
            name != "NEEDLES HALL" &&
            name != "MKV" &&
            name != "TUITION" &&
            name != "SLC" &&
            name != "COLLECT OSAP") {
            cout << "|" << left << setw(7) << setfill(' ') << name;
        } else {
            cout << "|       ";
        }
    }
    cout << "|" << endl;

    // line 60
    for (int i = 10; i >= 0; --i) {
        cout << "|" << getPlayerString(i, players);
    }
    cout << "|" << endl;


    // line 61
   for(int i = 0; i < 11;++i){
      cout << "|_______";
   }
   cout << "|" << endl;



}
