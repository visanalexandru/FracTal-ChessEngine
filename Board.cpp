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
	void Board::makeMove(Move move){
		history.push(current_game_state);
		Position org=move.getOrigin();
		Position dest=move.getDestination();

		if(move.getType()==MoveType::Capture){
			pieces[dest.y][dest.x]=pieces[org.y][org.x];
			pieces[org.y][org.x]=Piece::None;
		}
		current_game_state.toggleState(turnColor);
	}
	void Board::undoLastMove(){
		
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

}
