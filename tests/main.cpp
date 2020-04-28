#include<iostream>
#include"../src/Board.h"
#include "../src/MoveGen.h"
#include <string.h>
using namespace std;

Engine::Board board;
Engine::MoveGen movegen(board);

int perft(int depth){
    vector<Engine::Move> moves=movegen.getAllMoves();
    if(depth==1)
        return moves.size();
    int to_return=0;

    for(const Engine::Move&move:moves){
        board.makeMove(move);
        to_return+=perft(depth-1);
        board.undoLastMove();
    }
    return  to_return;

}


int main(){
    float a=clock();
    cout<<perft(6)<<endl;
    cout<<"perf took "<<(clock()-a)/CLOCKS_PER_SEC;
    return 0;
}