#include<iostream>
#include "../src/Board.h"
#include "../src/MoveGen.h"
#include <string.h>

using namespace std;

BitEngine::Board board;
BitEngine::MoveGen movegen(board);


long long perft_captures = 0, perft_nodes = 0, perft_ep = 0;

void perft(int depth) {
    //check for illegal king moves
    vector<BitEngine::Move> moves = movegen.getAllMoves();

    if (depth == 1) {
        perft_nodes+=moves.size();
        return;
    }

    for (const BitEngine::Move &move:moves) {
        board.makeMove(move);
        perft(depth - 1);
        board.undoLastMove();
    }
}

string fenConfigurations[]{
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ",
"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ",
"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
"8/PPP4k/8/8/8/8/4Kppp/8 w - - 0 1",
"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1"//for promotion bugs
};


vector<vector<long long >> perftResults{
        {44,1486,62379},
        {48,2039,97862,4085603,193690690},
        {20,400,8902,197281,4865609,119060324},
        {14,191,2812,43238,674624,11030083,178633661},
        {46,2079,89890,3894594,164075551},
        {18,290,5044,89363,1745545,34336777,749660761},
        {24,496,9483,182838,3605103,71179139}
};


void perftTest() {
    for(int i=0;i<perftResults.size();i++){
        string to_test=fenConfigurations[i];
        board.loadFen(to_test);
        cout<<"configuration "<<i+1<<endl;

        for(int k=0;k<perftResults[i].size();k++){
            perft_nodes=0;
            float elapsed =clock();
            perft(k+1);
            elapsed=(clock()-elapsed)/CLOCKS_PER_SEC;
            if(perft_nodes==perftResults[i][k]){
                cout<<"OK "<<perft_nodes<<" in "<<elapsed<<endl;
            }
            else{
                cout<<"ERR generated"<<perft_nodes<< ",correct"<<perftResults[i][k]<<" in "<<elapsed<<endl;
            }
        }
    }


}

int main() {
    BitEngine::Tables::initializeAttackTables();
    perftTest();

    return 0;
}