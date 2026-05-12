#ifndef Bishop_H
#define Bishop_H

#include "Piece.h"
class Bishop : public Piece {
public:
	Bishop(char s);

	bool isValidMove(int sx, int sy, int dx, int dy, Board* board)override;

};
#endif