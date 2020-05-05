#include "AnParser.h"

namespace Engine {
    namespace AnParser {

        Engine::Move getMove(const std::string &an, Engine::Board &board) {
            Engine::MoveGen movegen(board);
            std::vector<Engine::Move> move_list = movegen.getAllMoves();

            for (Engine::Move a:move_list) {
                if (a.toString() == an)
                    return a;
            }
            return Engine::Move();//null move;
        }

    }
}
