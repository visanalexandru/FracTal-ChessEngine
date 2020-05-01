#include "Protocol.h"


Protocol::Protocol(BitEngine::Board &internal_board) : isRunning(false),
                                                    log("chesslog"),
                                                    board(internal_board) {


}

void Protocol::send(const std::string &to_send) {
    Log("sent " + to_send);
    std::cout << to_send << '\n';
}

void Protocol::Log(const std::string &to_log) {
    log << to_log << std::endl;
}

bool Protocol::compare(BitEngine::Move a, BitEngine::Move b) {
    return a.getType() < b.getType();
}

void Protocol::handleRequest(const std::string &req) {

    log << "received " << req << std::endl;

    std::stringstream sstream;
    sstream << req;

    std::string cmmd;
    sstream >> cmmd;

    if (cmmd == "uci") {
        send("id name StockFischer");
        send("id author Visan");
        send("uciok");
    } else if (cmmd == "isready") {
        send("readyok");
    } else if (cmmd == "position") {
        std::string aux;
        sstream >> aux;//the first string is a fen string or "startpos"

        while (!sstream.eof())
            sstream >> aux;

        BitEngine::Move server_move = Engine::AnParser::getMove(aux, board);
        if (server_move.getType() == BitEngine::MoveType::Null) {
            Log("invalid move" + server_move.toString());
            isRunning = false;
        } else {
            board.makeMove(Engine::AnParser::getMove(aux, board));
            Log(board.prt());
        }
    } else if (cmmd == "go") {
        BitEngine::MoveGen movegen(board);
        std::vector<BitEngine::Move> moves = movegen.getAllMoves();
        std::sort(moves.begin(),moves.end(),compare);
        BitEngine::Move bestmove = moves[moves.size()-1];
        send("bestmove " + bestmove.toString());
        board.makeMove(bestmove);
        Log(board.prt());
    } else if (cmmd == "quit") {
        isRunning = false;
    }
}

void Protocol::start() {
    std::string input;
    isRunning = true;
    while (isRunning) {
        std::getline(std::cin, input);
        handleRequest(input);
    }
}


