#include<iostream>
#include"../src/Board.h"
#include "../src/MoveGen.h"
#include <string.h>

using namespace std;

Engine::Board board;
Engine::MoveGen movegen(board);


int perft_captures = 0, perft_nodes = 0, perft_ep = 0;

void perft(int depth, bool capture, bool enPassant) {
    //check for illegal king moves
    vector<Engine::Move> moves = movegen.getAllMoves();
    for (const Engine::Move &move:moves)
        if (Engine::getPieceType(move.getTaken()) == Engine::Piece::King)
            return;


    if (depth == 0) {
        perft_nodes++;
        if (capture)
            perft_captures++;
        if (enPassant)
            perft_ep++;
        return;
    }

    for (const Engine::Move &move:moves) {
        board.makeMove(move);
        perft(depth - 1, Engine::getPieceType(move.getTaken()) != Engine::Piece::None,
              move.getType() == Engine::MoveType::EnPassant);
        board.undoLastMove();
    }
}


int main() {
    float a = clock();
    perft(5, false, false);
    cout << "perf took " << (clock() - a) / CLOCKS_PER_SEC << endl;

    cout << "perf nodes " << perft_nodes << endl;
    cout << "perf captures " << perft_captures << endl;
    cout << "perf en passant " << perft_ep << endl;
    cout << board.print();

    return 0;
}