#include<iostream>
#include<fstream>

using namespace std;

#include"Protocol.h"
#include "Eval.h"

int main() {
    srand(time(NULL));
    Engine::Tables::initializeAttackTables();
    Engine::Zobrist::initZobrist();
    Engine::Board board;
    Protocol uci(board);
    uci.start();
    return 0;
}
