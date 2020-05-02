#include<iostream>
#include<fstream>

using namespace std;

#include"Protocol.h"
#include"Board.h"
#include "Position.h"


using namespace Engine;

int main() {
    srand(time(NULL));
    BitEngine::Tables::initializeAttackTables();
    BitEngine::Board board;
    cout << board.toString();
    Protocol uci(board);
    uci.start();
}
