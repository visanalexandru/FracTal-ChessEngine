#include<iostream>
#include"../src/Board.h"
#include "../src/MoveGen.h"
#include "../src/BitBoard/Board.h"
#include "../src/Types.h"
#include "../src/BitBoard/MoveGen.h"
#include <string.h>

using namespace std;

Engine::Board board;
Engine::MoveGen movegen(board);


int perft_captures = 0, perft_nodes = 0, perft_ep = 0;

void perft(int depth, bool capture, bool enPassant) {
    //check for illegal king moves
    vector<Engine::Move> moves = movegen.getAllMoves();

    if (depth == 1) {
        perft_nodes += moves.size();
        return;
    }

    for (const Engine::Move &move:moves) {
        board.makeMove(move);
        perft(depth - 1, Engine::getPieceType(move.getTaken()) != Engine::Piece::None,
              move.getType() == Engine::MoveType::EnPassant);
        board.undoLastMove();
    }
}


void testBitBoards() {

    BitEngine::Board bt;
    BitEngine::Tables::initializeAttackTables();
    for (int i = 0; i <64; i++) {
        uint64_t  attacks=0;


        for(int k=0;k<8;k++)
        attacks|= BitEngine::Tables::AttackTables[i][k];
        BitEngine::printBitboard(attacks);

    }
    BitEngine::MoveGen movegen(bt);
    movegen.getAllMoves();


}

int main() {


    testBitBoards();
    return 0;

    float a = clock();
    perft(6, false, false);
    cout << "perf took " << (clock() - a) / CLOCKS_PER_SEC << endl;

    cout << "perf nodes " << perft_nodes << endl;
    cout << "perf captures " << perft_captures << endl;
    cout << "perf en passant " << perft_ep << endl;
    cout << board.print();

    return 0;
}