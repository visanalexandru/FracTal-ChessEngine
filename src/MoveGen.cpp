#include "MoveGen.h"

namespace Engine {

    MoveGen::MoveGen(Engine::Board&internal_board) : board(internal_board) {

    }

    Move MoveGen::createNormal(Position a, Position b) const {
        return Move(MoveType::Normal, a, b, board.getPieceAt(a), board.getPieceAt(b), Piece::None);
    }

    Move MoveGen::createPromotion(Position a, Position b, Piece promote_to) const {
        return Move(MoveType::Promote, a, b, board.getPieceAt(a), board.getPieceAt(b), promote_to);
    }

    Move MoveGen::createDoublePawnPush(Position a, Position b) const {
        return Move(MoveType::DoublePawnPush, a, b, board.getPieceAt(a), Piece::None, Piece::None);
    }

    Move MoveGen::createEnPassant(Position a, Position b) const {
        Piece taken = board.getPieceAt(Position(b.x, a.y));
        return Move(MoveType::EnPassant, a, b, board.getPieceAt(a), taken, Piece::None);
    }

    Move MoveGen::createKingSideCastle() const {
        if (board.getTurn() == Color::White) {
            return Move(MoveType::KingSideCastle, Position(4, 0),
                        Position(6, 0), Piece::WhiteKing, Piece::None, Piece::None);
        }
        return Move(MoveType::KingSideCastle, Position(4, 7),
                    Position(6, 7), Piece::BlackKing, Piece::None, Piece::None);
    }

    Move MoveGen::createQueenSideCastle() const {
        if (board.getTurn() == Color::White) {
            return Move(MoveType::QueenSideCastle, Position(4, 0),
                        Position(2, 0), Piece::WhiteKing, Piece::None, Piece::None);
        }
        return Move(MoveType::QueenSideCastle, Position(4, 7),
                    Position(2, 7), Piece::BlackKing, Piece::None, Piece::None);
    }


    void MoveGen::generatePawnMoves(Position a,std::vector<Move>&moves) const{

        Position l,r,f,ff;
        Color turn_color=board.getTurn();

        if(turn_color==Color::White){
            f=Position(a.x,a.y+1);
            ff=Position(a.x,a.y+2);
            l=Position(a.x-1,a.y+1);
            r=Position(a.x+1,a.y+1);

            if(a.y==1 && board.getPieceAt(f)==Piece::None && board.getPieceAt(ff)==Piece::None)
                moves.push_back(createDoublePawnPush(a,ff));
        }
        else {
            f=Position(a.x,a.y-1);
            ff=Position(a.x,a.y-2);
            l=Position(a.x-1,a.y-1);
            r=Position(a.x+1,a.y-1);
            if(a.y==6 && board.getPieceAt(f)==Piece::None && board.getPieceAt(ff)==Piece::None)
                moves.push_back(createDoublePawnPush(a,ff));
        }

        std::vector<Position> can_go;
        Piece piece_front=board.getPieceAt(f),piece_left=board.getPieceAt(l),piece_right=board.getPieceAt(r);

        if(f.isInside() && piece_front==Piece::None) {
            can_go.push_back(f);
        }
        if(l.isInside() && piece_left!=Piece::None && getColor(piece_left) !=turn_color )
            can_go.push_back(l);

        if(r.isInside() && piece_right!=Piece::None &&getColor(piece_right) !=turn_color)
            can_go.push_back(r);



        for(Position pos:can_go){
            if(turn_color==White && pos.y==7){
                moves.push_back(createPromotion(a,pos,Piece::WhiteQueen));
                moves.push_back(createPromotion(a,pos,Piece::WhiteKnight));
                moves.push_back(createPromotion(a,pos,Piece::WhiteBishop));
                moves.push_back(createPromotion(a,pos,Piece::WhiteRook));
            }
            else if(turn_color==Black && pos.y==0){
                moves.push_back(createPromotion(a,pos,Piece::BlackQueen));
                moves.push_back(createPromotion(a,pos,Piece::BlackKnight));
                moves.push_back(createPromotion(a,pos,Piece::BlackBishop));
                moves.push_back(createPromotion(a,pos,Piece::BlackRook));
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

                    if(neighbour.isInside() && getColor(board.getPieceAt(neighbour))!=board.getTurn()){
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
            if(there.isInside() && getColor(board.getPieceAt(there))!=board.getTurn())
                moves.push_back(createNormal(a,there));
        }
    }
    void MoveGen::generateBishopMoves(Engine::Position a, std::vector<Move> &moves) const {

        Position offsets[4]{Position(1, 1), Position(1, -1), Position(-1, 1), Position(-1, -1)};
        for (int i = 0; i < 4; i++) {
            Position now = a + offsets[i];

            while (now.isInside()) {
                Piece here =board.getPieceAt(now);
                if (here == Piece::None) {
                    moves.push_back(createNormal(a, now));
                } else {
                    if (getColor(here) != board.getTurn())
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
                Piece here = board.getPieceAt(now);
                if (here == Piece::None) {
                    moves.push_back(createNormal(a, now));
                } else {
                    if (getColor(here) != board.getTurn())
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
        for(int i=0;i<8;i++){
            for(int k=0;k<8;k++){
                Position pos(i,k);
                Piece there=board.getPieceAt(pos);

                if(getColor(there)==board.getTurn()){
                    if(there==Piece::BlackPawn || there==Piece::WhitePawn)
                        generatePawnMoves(pos,to_return);
                    else if(there==Piece::BlackKnight || there==Piece::WhiteKnight)
                        generateKnightMoves(pos,to_return);
                    else if(there==Piece::BlackBishop || there==Piece::WhiteBishop){
                        generateBishopMoves(pos,to_return);
                    }
                    else if(there==Piece::BlackRook||there==Piece::WhiteRook){
                        generateRookMoves(pos,to_return);
                    }
                    else if(there==Piece::BlackQueen || there==Piece::WhiteQueen){
                        generateQueenMoves(pos,to_return);
                    }
                    else if(there==Piece::WhiteKing || there==Piece::BlackKing)
                        generateKingMoves(pos,to_return);
                }
            }
        }
        return to_return;
    }
}