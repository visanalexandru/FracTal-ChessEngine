
#include "Types.h"

namespace Engine {
    namespace {
        const char pieceChar[13]{'P', 'p', 'N', 'n', 'B', 'b', 'R', 'r', 'Q', 'q', 'K', 'k', ' '};
        const int pieceValue[6]{100, 320, 330, 500, 900, 0};
    }

    char getPieceChar(Piece type) {
        return pieceChar[type];
    }

    int getPieceValue(PieceType type) {
        if (type == PieceType::None)
            return 0;
        return pieceValue[static_cast<int>(type)];
    }
}