#include<iostream>
#include<fstream>
using namespace std;

#include"Protocol.h"
#include"Board.h"
#include "Position.h"


using namespace Engine;

int main(){

	Engine::Board board;

	cout<<board.getAllMoves().size()<<endl;
	Move to_move=board.getAllMoves()[1];
	board.makeMove(to_move);
	board.print();

	cout<<to_move.toString();

	Protocol uci;
	uci.start();
}
