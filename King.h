#ifndef KING_H
#define KING_H

#include "Piece.h"
class King : public Piece {
public:
	King(char s);

	bool isValidMove(int sx, int sy, int dx, int dy, Board* board)override;

};
#endif