
#include "Types.h"

namespace BitEngine{
    namespace {
        const char pieceChar[13]{'P','p','N','n','B','b','R','r','Q','q','K','k',' '};
    }

    char getPieceChar(Piece type){
        return pieceChar[type];
    }

    Color getOpposite(Color color){
        return static_cast<Color>(1-color);
    }
}