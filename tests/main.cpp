#include<iostream>
#include"../src/Board.h"
#include "../src/MoveGen.h"
#include "../src/BitBoard/Board.h"
#include "../src/Types.h"
#include "../src/BitBoard/MoveGen.h"
#include <string.h>

using namespace std;

BitEngine::Board board;
BitEngine::MoveGen movegen(board);


int perft_captures = 0, perft_nodes = 0, perft_ep = 0;

bool perft(int depth, bool capture, bool enPassant) {
    //check for illegal king moves
    vector<BitEngine::Move> moves = movegen.getAllMoves();

    for (BitEngine::Move a:moves) {
        if (a.getTaken() == BitEngine::WKing || a.getTaken() == BitEngine::BKing)
            return false;
    }

    if (depth == 0) {
        perft_nodes++;
        if (enPassant)
            perft_ep++;
        if (capture)
            perft_captures++;

        return true;
    }

    for (const BitEngine::Move &move:moves) {
        board.makeMove(move);
        perft(depth - 1, move.getTaken() != BitEngine::PieceType::None,
              move.getType() == BitEngine::MoveType::EnPassant);
        board.undoLastMove();
    }
}


void testBitBoards() {

    BitEngine::Board bt;
    BitEngine::Tables::initializeAttackTables();
    for (int i = 0; i < 64; i++) {
        uint64_t attacks = 0;


        for (int k = 0; k < 8; k++)
            attacks |= BitEngine::Tables::AttackTables[i][k];
        BitEngine::printBitboard(attacks);

    }
    BitEngine::MoveGen movegen(bt);
    movegen.getAllMoves();


}

int main() {

    BitEngine::Tables::initializeAttackTables();
    float a = clock();
    cout << board.toString();
    perft(6, false, false);
    cout << "perf took " << (clock() - a) / CLOCKS_PER_SEC << endl;

    cout << "perf nodes " << perft_nodes << endl;
    cout << "perf captures " << perft_captures << endl;
    cout << "perf en passant " << perft_ep << endl;

    return 0;
}