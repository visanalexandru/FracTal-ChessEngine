#include "Position.h"

namespace Engine{

	Position::Position(int a,int b):x(a),y(b){

	}
	bool Position::isInside(){
		return x>=0 && x<8 && y>=0 && y<8;
	}
	Position Position::operator+(const Position&other ) const{
		return Position(x+other.x,y+other.y);
	}
	Position Position::operator-(const Position&other ) const {
		return Position(x-other.x,y-other.y);
	}

	void Position::operator+=(const Engine::Position &other) {
	    x+=other.x;
	    y+=other.y;
	}
	void Position::operator-=(const Engine::Position &other) {
	    x-=other.x;
	    y-=other.y;
	}
}
