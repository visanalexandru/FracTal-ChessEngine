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
		Position dest=move.getDestination();

		pieces[dest.y][dest.x]=pieces[org.y][org.x];
		pieces[org.y][org.x]=Piece::None;


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
		current_game_state.toggleState(turnColor);
	}
	void Board::makeEnPassant(Move move){
		Position a=move.getOrigin();
		Position b=move.getDestination();

		pieces[a.y][b.x]=Piece::None;	
		pieces[b.y][b.x]=getPieceAt(a);
		pieces[a.y][a.x]=Piece::None;
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
	void Board::makeMove(Move move){
		history.push(current_game_state);
		switch(move.getType()){
			case MoveType::Normal:
				makeNormalMove(move);
				break;
			case MoveType::KingSideCastle:
				makeKingSideCastle();
				break;
			case MoveType::QueenSideCastle:
				makeQueenSideCastle();
				break;

		}	
		current_game_state.toggleState(turnColor);
		current_game_state.setLastMove(move);
	}
	void Board::undoLastMove(){
		Move last_move=current_game_state.getLastMove();
		Position org=last_move.getOrigin();
		Position dest=last_move.getDestination();
		if(last_move.getType()==MoveType::Normal){
			pieces[org.y][org.x]=pieces[dest.y][dest.x];
			pieces[dest.y][dest.x]=last_move.getTaken();
		}

		current_game_state=history.top();
		history.pop();
	}



	void Board::print() const{
		for(int i=0;i<8;i++){
			for(int k=0;k<33;k++)
				std::cout<<"#";
			std::cout<<std::endl;
			std::cout<<"# ";

			for(int k=0;k<8;k++){
				std::cout<<getChar(pieces[7-i][k])<<" # ";
			}
			std::cout<<std::endl;
		}
		for(int k=0;k<33;k++)
			std::cout<<"#";

		std::cout<<std::endl;

	}


	Color Board::getTurn() const{
		if(current_game_state.getState(turnColor))
			return Black;
		return White;
	}
	Piece Board::getPieceAt(Position pos) const{
		return pieces[pos.y][pos.x];
	}

	Move Board::createNormal(Position a,Position b) const{
		return Move(MoveType::Normal,a,b,getPieceAt(b),Piece::None);
	}
	Move Board::createPromotion(Position a,Position b,Piece promote_to) const{
		return Move(MoveType::Promote,a,b,getPieceAt(b),promote_to);
	}
	Move Board::createDoublePawnPush(Position a,Position b) const{
		return Move(MoveType::DoublePawnPush,a,b,Piece::None,Piece::None);
	}
	Move Board::createEnPassant(Position a,Position b) const{
		return Move(MoveType::EnPassant,a,b,Piece::None,Piece::None);
	}

	Move Board::createKingSideCastle() const{
		if(getTurn()==Color::White){
			return Move(MoveType::KingSideCastle,Position(4,0),
					Position(6,0),Piece::None,Piece::None);
		}
		return Move(MoveType::KingSideCastle,Position(4,7),
				Position(6,7),Piece::None,Piece::None);
	}

	Move Board::createQueenSideCastle() const{
		if(getTurn()==Color::White){
			return Move(MoveType::QueenSideCastle,Position(4,0),
					Position(2,0),Piece::None,Piece::None);
		}
		return Move(MoveType::QueenSideCastle,Position(4,7),
				Position(2,7),Piece::None,Piece::None);
	}

}
