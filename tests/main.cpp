#include<iostream>
#include"../src/Board.h"
#include "../src/MoveGen.h"
#include <string.h>
using namespace std;

Engine::Board board;
Engine::MoveGen movegen(board);

int perft(int depth){
    if(depth==0)
        return 1;
    int nodes=0;
    vector<Engine::Move> moves=movegen.getAllMoves();
    for(const Engine::Move&move:moves){
        board.makeMove(move);
        nodes+=perft(depth-1);
        board.undoLastMove();
    }
    return  nodes;

}


int main(){
    float a=clock();
    cout<<perft(6)<<endl;
    cout<<"perf took "<<(clock()-a)/CLOCKS_PER_SEC;
    return 0;
}