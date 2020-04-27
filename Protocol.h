#ifndef PROTOCOL_H
#define PROTOCOL_H


#include<iostream>
#include<fstream>
#include<sstream>
#include"Board.h"
#include "AnParser.h"
class Protocol{
	private:
        Engine::Board&board;
		std::ofstream log;
		bool isRunning;
		void handleRequest(const std::string&req);
		void send(const std::string&to_send);
		void Log(const std::string&to_log);

	public:
		Protocol(Engine::Board&internal_board);
		void start();
};

#endif
