#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece {
public:
	Pawn(char s);

	bool isValidMove(int sx, int sy, int dx, int dy, Board* board) override;

};
#endif