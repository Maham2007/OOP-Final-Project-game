#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"
class Knight : public Piece {
public:
	Knight(char s);

	bool isValidMove(int sx, int sy, int dx, int dy, Board* board)override;

};
#endif