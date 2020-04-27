#include<iostream>
#include<fstream>
using namespace std;

#include"Protocol.h"
#include"Board.h"
#include "Position.h"


using namespace Engine;

int main(){

	Engine::Board board;
	Protocol uci(board);
	uci.start();
}
