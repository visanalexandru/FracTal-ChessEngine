#include<iostream>
#include<fstream>
using namespace std;

#include"Protocol.h"
#include"Board.h"
#include "Position.h"


using namespace Engine;

int main(){
	
	Engine::Board board;
	
	Move move(MoveType::Quiet,Position(3,1),Position(3,6),Piece::BlackPawn);
	Move move2(MoveType::Quiet,Position(4,1),Position(4,3),Piece::None);


	board.makeMove(move2);
	board.makeMove(move);
	board.undoLastMove();
	board.undoLastMove();
	board.print();

	Protocol uci;
	uci.start();
}
