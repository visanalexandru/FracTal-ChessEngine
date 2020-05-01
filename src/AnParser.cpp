#include "AnParser.h"

namespace Engine {
    namespace AnParser {

        BitEngine::Move getMove(const std::string &an,BitEngine::Board &board) {
            BitEngine::MoveGen movegen(board);
            std::vector<BitEngine::Move> move_list = movegen.getAllMoves();

            for (BitEngine::Move a:move_list) {
                if (a.toString() == an)
                    return a;
            }
            return BitEngine::Move();//null move;
        }

    }
}
