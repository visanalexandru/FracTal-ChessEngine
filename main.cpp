#include<iostream>
#include<fstream>
using namespace std;

#include"Protocol.h"
#include"Board.h"
#include "Position.h"


using namespace Engine;

int main(){
	
	Engine::Board board;
	

	Move to_move=board.createNormalMove(Position(2,1),Position(2,6));
	board.makeMove(to_move);
	board.undoLastMove();
	board.print();

	cout<<to_move.toString();

	Protocol uci;
	uci.start();
}
