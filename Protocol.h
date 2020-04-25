#ifndef PROTOCOL_H
#define PROTOCOL_H


#include<iostream>
#include<fstream>
class Protocol{
	private:
		std::ofstream log;
		bool isRunning;
		void handleRequest(const std::string&req);
		void send(const std::string&to_send);

	public:
		Protocol();
		void start();
};

#endif
