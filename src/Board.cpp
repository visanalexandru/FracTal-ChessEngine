#include <linux/videodev2.h>
#include "Board.h"

namespace Engine{
	Board::Board(){
		initBoard();
	}
	void Board::initBoard(){


		current_game_state.setState(canCastleQueenSideBlack);
		current_game_state.setState(canCastleKingSideBlack);
		current_game_state.setState(canCastleQueenSideWhite);
		current_game_state.setState(canCastleKingSideWhite);

		for(int i=0;i<8;i++){
			for(int k=0;k<8;k++)
				pieces[i][k]=Piece::None;
		}

		for(int i=0;i<8;i++){
			pieces[1][i]=Piece::WhitePawn;
			pieces[6][i]=Piece::BlackPawn;
		}

		pieces[0][0]=Piece::WhiteRook;
		pieces[0][1]=Piece::WhiteKnight;
		pieces[0][2]=Piece::WhiteBishop;
		pieces[0][3]=Piece::WhiteQueen;
		pieces[0][4]=Piece::WhiteKing;
		pieces[0][5]=Piece::WhiteBishop;
		pieces[0][6]=Piece::WhiteKnight;
		pieces[0][7]=Piece::WhiteRook;

		pieces[7][0]=Piece::BlackRook;
		pieces[7][1]=Piece::BlackKnight;
		pieces[7][2]=Piece::BlackBishop;
		pieces[7][3]=Piece::BlackQueen;
		pieces[7][4]=Piece::BlackKing;
		pieces[7][5]=Piece::BlackBishop;
		pieces[7][6]=Piece::BlackKnight;
		pieces[7][7]=Piece::BlackRook;

	}
	void Board::makeNormalMove(Move move){

		Position org=move.getOrigin();
		setPieceAt(move.getDestination(),getPieceAt(org));
        setPieceAt(org,Piece::None);


		if(getTurn()==Color::White){
			if(org.y==0 && org.x==4){
				current_game_state.unsetState(canCastleKingSideWhite);
				current_game_state.unsetState(canCastleQueenSideWhite);
			}
			else if (org.y==0 && org.x==0){
				current_game_state.unsetState(canCastleQueenSideWhite);
			}
			else if(org.y==0 && org.x==7){
				current_game_state.unsetState(canCastleKingSideWhite);
			}
		}
		else{

			if(org.y==7 && org.x==4){
				current_game_state.unsetState(canCastleKingSideBlack);
				current_game_state.unsetState(canCastleQueenSideBlack);
			}
			else if (org.y==7 && org.x==0){
				current_game_state.unsetState(canCastleQueenSideBlack);
			}
			else if(org.y==7 && org.x==7){
				current_game_state.unsetState(canCastleKingSideBlack);
			}

		}
	}
	void Board::undoNormalMove(Move move) {
        setPieceAt(move.getOrigin(),move.getMoved());
        setPieceAt(move.getDestination(),move.getTaken());
	}
	void Board::makePromotion(Move move) {
        setPieceAt(move.getDestination(),move.getPromotion());
        setPieceAt(move.getOrigin(),Piece::None);
	}
	void Board::makeEnPassant(Move move){
		Position a=move.getOrigin();
		Position b=move.getDestination();
		setPieceAt(Position(b.x,a.y),Piece::None);
		setPieceAt(b,getPieceAt(a));
		setPieceAt(a,Piece::None);
	}
	void Board::undoEnPassant(Move move) {
        Position a=move.getOrigin();
        Position b=move.getDestination();
        setPieceAt(move.getOrigin(),move.getMoved());
        setPieceAt(b,Piece::None);
        setPieceAt(Position(b.x,a.y),move.getTaken());
	}
	void Board::makeKingSideCastle(){
		if(getTurn()==Color::White){
			pieces[0][6]=Piece::WhiteKing;
			pieces[0][4]=Piece::None;
			pieces[0][5]=Piece::WhiteRook;
			pieces[0][7]=Piece::None;

			current_game_state.unsetState(canCastleKingSideWhite);
			current_game_state.unsetState(canCastleQueenSideWhite);
		}
		else{
			pieces[7][6]=Piece::BlackKing;
			pieces[7][4]=Piece::None;
			pieces[7][5]=Piece::BlackRook;
			pieces[7][7]=Piece::None;
			current_game_state.unsetState(canCastleKingSideBlack);
			current_game_state.unsetState(canCastleQueenSideBlack);

		}
	}
	void Board::undoKingSideCastle(Engine::Move move) {

	    if(move.getMoved()==Piece::WhiteKing){
            setPieceAt(Position(4,0),Piece::WhiteKing);
            setPieceAt(Position(6,0),Piece::None);
            setPieceAt(Position(5,0),Piece::None);
            setPieceAt(Position(7,0),Piece::WhiteRook);
	    }
	    else{
            setPieceAt(Position(4,7),Piece::BlackKing);
            setPieceAt(Position(6,7),Piece::None);
            setPieceAt(Position(5,7),Piece::None);
            setPieceAt(Position(7,7),Piece::BlackRook);
	    }

	}
	void Board::makeQueenSideCastle(){
		if(getTurn()==Color::White){
			pieces[0][2]=Piece::WhiteKing;
			pieces[0][4]=Piece::None;
			pieces[0][3]=Piece::WhiteRook;
			pieces[0][0]=Piece::None;
			current_game_state.unsetState(canCastleKingSideWhite);
			current_game_state.unsetState(canCastleQueenSideWhite);

		}
		else{
			pieces[7][2]=Piece::BlackKing;
			pieces[7][4]=Piece::None;
			pieces[7][3]=Piece::BlackRook;
			pieces[7][0]=Piece::None;
			current_game_state.unsetState(canCastleKingSideBlack);
			current_game_state.unsetState(canCastleQueenSideBlack);
		}
	}

	void Board::undoQueenSideCastle(Engine::Move move) {
        if(move.getMoved()==Piece::WhiteKing){
            setPieceAt(Position(4,0),Piece::WhiteKing);
            setPieceAt(Position(3,0),Piece::None);
            setPieceAt(Position(2,0),Piece::None);
            setPieceAt(Position(1,0),Piece::None);
            setPieceAt(Position(0,0),Piece::WhiteRook);
        }
        else{
            setPieceAt(Position(4,7),Piece::BlackKing);
            setPieceAt(Position(3,7),Piece::None);
            setPieceAt(Position(2,7),Piece::None);
            setPieceAt(Position(1,7),Piece::None);
            setPieceAt(Position(0,7),Piece::BlackRook);
        }
	}
	void Board::makeMove(Move move){
		history.push(current_game_state);
		MoveType type=move.getType();
		if(type==MoveType::Normal || type==MoveType::DoublePawnPush)
			makeNormalMove(move);
		else if(type==MoveType::EnPassant)
		    makeEnPassant(move);
		else if(type==MoveType::KingSideCastle)
			makeKingSideCastle();
		else if(type==MoveType::QueenSideCastle)
			makeQueenSideCastle();
		else if(type==MoveType::Promote)
		    makePromotion(move);

		current_game_state.toggleState(turnColor);
		current_game_state.setLastMove(move);
	}
	void Board::undoLastMove(){
		Move last_move=current_game_state.getLastMove();
		MoveType  type=last_move.getType();
		if(type==MoveType::Normal ||type==MoveType::DoublePawnPush || type==MoveType::Promote){
            undoNormalMove(last_move);
		}
		else if(type==MoveType::EnPassant){
		    undoEnPassant(last_move);
		}
		else if(type==MoveType::KingSideCastle){
		    undoKingSideCastle(last_move);
		}
		else if(type==MoveType::QueenSideCastle){
		    undoQueenSideCastle(last_move);
		}

		current_game_state=history.top();
		history.pop();
	}



	std::string Board::print() const{
	    std::string to_return;
		for(int i=0;i<8;i++){
			for(int k=0;k<33;k++)
				to_return+='#';
			to_return+='\n';
			to_return+="# ";

			for(int k=0;k<8;k++){
				to_return+=getChar(pieces[7-i][k]);
				to_return+=" # ";
			}
			to_return+='\n';
		}
		for(int k=0;k<33;k++)
			to_return+='#';

		to_return+='\n';
        return to_return;
	}


	Color Board::getTurn() const{
		if(current_game_state.getState(turnColor)==1)
			return Black;
		return White;
	}
	GameState Board::getGamestate() const {
	    return current_game_state;
	}
	void Board::setPieceAt(Position pos, Piece piece) {
	    pieces[pos.y][pos.x]=piece;
	}
	Piece Board::getPieceAt(Position pos) const{
		return pieces[pos.y][pos.x];
	}
}
