#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"
class Queen : public Piece {
public:
	Queen(char s);

	bool isValidMove(int sx, int sy, int dx, int dy, Board* board)override;

};
#endif