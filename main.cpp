#include<iostream>
#include<fstream>
using namespace std;

#include"Protocol.h"
#include"Board.h"
#include "Position.h"


using namespace Engine;

int main(){
	
	Engine::Board board;
	
	Move move(MoveType::Capture,Position(3,1),Position(3,6),Piece::None);


	cout<<board.getTurn()<<endl;
	board.makeMove(move);
	cout<<board.getTurn()<<endl;
	board.print();

	Protocol uci;
	uci.start();
}
