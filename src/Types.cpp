
#include "Types.h"

namespace Engine{
    namespace {
        const char pieceChar[13]{'P','p','N','n','B','b','R','r','Q','q','K','k',' '};
    }

    char getPieceChar(Piece type){
        return pieceChar[type];
    }
}