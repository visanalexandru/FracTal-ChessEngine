#include<iostream>
#include<fstream>

using namespace std;

#include"Protocol.h"
#include "Eval.h"
int main() {
    srand(time(NULL));
    Engine::Tables::initializeAttackTables();
    Engine::Board board;
    Engine::Eval eval(board);
    cout << board.toString();
    Protocol uci(board);
    uci.start();
}
