#include<iostream>
#include<fstream>

using namespace std;

#include"Protocol.h"
int main() {
    srand(time(NULL));
    Engine::Tables::initializeAttackTables();
    Engine::Board board;
    cout << board.toString();
    Protocol uci(board);
    uci.start();
}
