#include "Protocol.h"


Protocol::Protocol():isRunning(false),
	log("chesslog"){


	}
void Protocol::send(const std::string&to_send){
	log<<"sent "<<to_send<<'\n';
	std::cout<<to_send<<'\n';
}

void Protocol::handleRequest(const std::string&req){

	log<<"received "<<req<<std::endl;
	
	if(req=="uci"){	
		send("id name StockFischer");
		send("id author Visan");
		send("uciok");
	}
	else if(req=="isready"){
		send("readyok");
	}
	else if(req=="quit"){
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


