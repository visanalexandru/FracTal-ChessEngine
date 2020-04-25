#include<iostream>
#include<fstream>
using namespace std;

#include"Protocol.h"
#include"Board.h"
#include "Position.h"


using namespace Engine;

int main(){
	
	Engine::Board board;
	
	Move move(Capture,Position(3,1),Position(3,6),0,Piece::Empty);



	board.makeMove(move);

	board.undoMove(move);
	board.print();

	Protocol uci;
	uci.start();
}
