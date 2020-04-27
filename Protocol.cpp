#include "Protocol.h"


Protocol::Protocol(Engine::Board&internal_board):isRunning(false),
	log("chesslog"),
	board(internal_board){


	}
void Protocol::send(const std::string&to_send){
	log<<"sent "<<to_send<<'\n';
	std::cout<<to_send<<'\n';
}

void Protocol::handleRequest(const std::string&req){

	log<<"received "<<req<<std::endl;

	std::stringstream sstream;
	sstream<<req;

	std::string cmmd;
	sstream>>cmmd;

	if(cmmd=="uci"){
		send("id name StockFischer");
		send("id author Visan");
		send("uciok");
	}
	else if(cmmd=="isready"){
		send("readyok");
	}
	else if(cmmd=="go"){
        std::vector<Engine::Move> moves=board.getAllMoves();
        Engine::Move bestmove=moves[rand()%(moves.size())];
        send("bestmove "+bestmove.toString());
        board.makeMove(bestmove);
    }
	else if(cmmd=="quit"){
		isRunning=false;
	}
}

void Protocol::start(){
	std::string input;
	isRunning=true;
	while(isRunning){
		std::getline(std::cin,input);
		handleRequest(input);
	}
}


