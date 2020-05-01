#include<iostream>
#include<fstream>

using namespace std;

#include"Protocol.h"
#include"Board.h"
#include "Position.h"


using namespace Engine;

int main() {
    srand(time(NULL));
    BitEngine::Board board;
    cout << board.prt();
    Protocol uci(board);
    uci.start();
}
