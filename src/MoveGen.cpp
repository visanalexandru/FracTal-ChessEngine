#include "MoveGen.h"

namespace Engine {

    MoveGen::MoveGen(Engine::Board&internal_board) : board(internal_board) {

    }

    Move MoveGen::createNormal(Position a, Position b) const {
        return Move(MoveType::Normal, a, b, board.getPieceAt(a), board.getPieceAt(b), None);
    }

    Move MoveGen::createPromotion(Position a, Position b, uint8_t promote_to) const {
        return Move(MoveType::Promote, a, b, board.getPieceAt(a), board.getPieceAt(b), promote_to);
    }

    Move MoveGen::createDoublePawnPush(Position a, Position b) const {
        return Move(MoveType::DoublePawnPush, a, b, board.getPieceAt(a), None, None);
    }

    Move MoveGen::createEnPassant(Position a, Position b) const {
        uint8_t taken = board.getPieceAt(Position(b.x, a.y));
        return Move(MoveType::EnPassant, a, b, board.getPieceAt(a), taken, None);
    }

    Move MoveGen::createKingSideCastle() const {
        if (board.getTurn() == Color::White) {
            return Move(MoveType::KingSideCastle, Position(4, 0),
                        Position(6, 0), WhiteKing, None, None);
        }
        return Move(MoveType::KingSideCastle, Position(4, 7),
                    Position(6, 7), BlackKing, None, None);
    }

    Move MoveGen::createQueenSideCastle() const {
        if (board.getTurn() == Color::White) {
            return Move(MoveType::QueenSideCastle, Position(4, 0),
                        Position(2, 0), WhiteKing, None, None);
        }
        return Move(MoveType::QueenSideCastle, Position(4, 7),
                    Position(2, 7), BlackKing, None, None);
    }


    void MoveGen::generatePawnMoves(Position a,std::vector<Move>&moves) const{

        Position l,r,f,ff;
        Color turn_color=board.getTurn();

        if(turn_color==Color::White){
            f=Position(a.x,a.y+1);
            ff=Position(a.x,a.y+2);
            l=Position(a.x-1,a.y+1);
            r=Position(a.x+1,a.y+1);

            if(a.y==1 && board.getPieceAt(f)==None && board.getPieceAt(ff)==None)
                moves.push_back(createDoublePawnPush(a,ff));
        }
        else {
            f=Position(a.x,a.y-1);
            ff=Position(a.x,a.y-2);
            l=Position(a.x-1,a.y-1);
            r=Position(a.x+1,a.y-1);
            if(a.y==6 && board.getPieceAt(f)==None && board.getPieceAt(ff)==None)
                moves.push_back(createDoublePawnPush(a,ff));
        }

        Position can_go[4];
        int possible=0;
        uint8_t piece_front=board.getPieceAt(f),piece_left=board.getPieceAt(l),piece_right=board.getPieceAt(r);

        if(f.isInside() && piece_front==None) {
            can_go[possible++]=f;
        }
        if(l.isInside() && piece_left!=None && getPieceColor(piece_left) !=turn_color )
            can_go[possible++]=l;

        if(r.isInside() && piece_right!=None &&getPieceColor(piece_right) !=turn_color)
            can_go[possible++]=r;

        Position pos;

        for(int i=0;i<possible;i++){
            pos=can_go[i];
            if(turn_color==White && pos.y==7){
                moves.push_back(createPromotion(a,pos,WhiteQueen));
                moves.push_back(createPromotion(a,pos,WhiteKnight));
                moves.push_back(createPromotion(a,pos,WhiteBishop));
                moves.push_back(createPromotion(a,pos,WhiteRook));
            }
            else if(turn_color==Black && pos.y==0){
                moves.push_back(createPromotion(a,pos,BlackQueen));
                moves.push_back(createPromotion(a,pos,BlackKnight));
                moves.push_back(createPromotion(a,pos,BlackBishop));
                moves.push_back(createPromotion(a,pos,BlackRook));
            }
            else moves.push_back(createNormal(a,pos));
        }


        if(board.getGamestate().getLastMove().getType()==MoveType::DoublePawnPush){
            Position pawnpushpos=board.getGamestate().getLastMove().getDestination();
            int x=pawnpushpos.x;
            if(abs(x-a.x)==1 && pawnpushpos.y==a.y){//is neighbour
                if(turn_color==Color::White){
                    moves.push_back(createEnPassant(a,pawnpushpos+Position(0,1)));
                }
                else{
                    moves.push_back(createEnPassant(a,pawnpushpos-Position(0,1)));
                }
            }
        }
    }


    void MoveGen::generateKingMoves(Engine::Position a, std::vector<Move> &moves) const {

        for(int i=-1;i<=1;i++){
            for(int k=-1;k<=1;k++){

                if(k!=0 || i!=0){
                    Position neighbour=a+Position(i,k);

                    if(neighbour.isInside()){
                        uint8_t pc=board.getPieceAt(neighbour);
                        if(getPieceType(pc)==Piece::None || getPieceColor(pc)!=board.getTurn())
                        moves.push_back(createNormal(a,neighbour));
                    }
                }

            }
        }
    }


    void MoveGen::generateKnightMoves(Engine::Position a, std::vector<Move> &moves) const {
        Position offsets[8]{Position(1,2),Position(2,1),Position(2,-1),
                            Position(1,-2),Position(-1,-2),
                            Position(-2,-1),Position(-2,1),Position(-1,2)};
        for(int i=0;i<8;i++){
            Position there=a+offsets[i];
            if(there.isInside()){
                uint8_t pc=board.getPieceAt(there);
                if(getPieceType(pc)==Piece::None || getPieceColor(pc)!=board.getTurn()){
                    moves.push_back(createNormal(a,there));
                }
            }

        }
    }
    void MoveGen::generateBishopMoves(Engine::Position a, std::vector<Move> &moves) const {

        Position offsets[4]{Position(1, 1), Position(1, -1), Position(-1, 1), Position(-1, -1)};
        for (int i = 0; i < 4; i++) {
            Position now = a + offsets[i];

            while (now.isInside()) {
                uint8_t here =board.getPieceAt(now);
                if (here == None) {
                    moves.push_back(createNormal(a, now));
                } else {
                    if (getPieceColor(here) != board.getTurn())
                        moves.push_back(createNormal(a, now));
                    break;
                }
                now += offsets[i];
            }
        }
    }

    void MoveGen::generateRookMoves(Engine::Position a, std::vector<Move> &moves) const {

        Position offsets[4]{Position(1, 0), Position(-1, 0), Position(0, 1), Position(0, -1)};
        for (int i = 0; i < 4; i++) {
            Position now = a + offsets[i];
            while (now.isInside()) {
                uint8_t here = board.getPieceAt(now);
                if (here == None) {
                    moves.push_back(createNormal(a, now));
                } else {
                    if (getPieceColor(here) != board.getTurn())
                        moves.push_back(createNormal(a, now));
                    break;
                }
                now += offsets[i];
            }
        }
    }
    void MoveGen::generateQueenMoves(Engine::Position a, std::vector<Move> &moves) const {
        generateRookMoves(a,moves);
        generateBishopMoves(a,moves);
    }

    std::vector<Move> MoveGen::getAllMoves() const{
        std::vector<Move> to_return;
        to_return.reserve(300);
        for(int i=0;i<8;i++){
            for(int k=0;k<8;k++){
                Position pos(i,k);
                uint8_t there=board.getPieceAt(pos);

                if(getPieceColor(there)==board.getTurn()){

                    switch (getPieceType(there)){
                        case Piece::Pawn:
                            generatePawnMoves(pos,to_return);
                            break;
                        case Piece::Bishop:
                            generateBishopMoves(pos,to_return);
                            break;
                        case Piece::Rook:
                            generateRookMoves(pos,to_return);
                            break;
                        case Piece::Queen:
                            generateQueenMoves(pos,to_return);
                            break;
                        case Piece::King:
                            generateKingMoves(pos,to_return);
                            break;
                        case Piece::Knight:
                            generateKnightMoves(pos,to_return);
                            break;
                    }
                }
            }
        }
        return to_return;
    }
}