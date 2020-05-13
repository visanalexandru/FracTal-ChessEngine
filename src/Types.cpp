
#include "Types.h"

namespace Engine{
    namespace {
        const char pieceChar[13]{'P','p','N','n','B','b','R','r','Q','q','K','k',' '};
        const int pieceValue[5]{100,320,330,500,900};
    }

    char getPieceChar(Piece type){
        return pieceChar[type];
    }
    int getPieceValue(Piece type){
        if(type==None || type==WKing || type==BKing)
            return 0;
        return pieceValue[type/2];
    }
}