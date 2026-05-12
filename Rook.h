#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece {
public:
	Rook(char s);
	bool isValidMove(int sx, int sy, int dx, int dy, Board* board) override;
};
#endif