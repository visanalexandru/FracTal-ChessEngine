#include "Position.h"

namespace Engine{

	Position::Position(int a,int b):x(a),y(b){

	}
	bool Position::isInside(){
		return x>=0 && x<8 && y>=0 && y<8;
	}
}
