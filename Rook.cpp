#include "Rook.h"

Rook::Rook(char s) : Piece(s) {}

bool Rook::isValidMove(int sx, int sy, int dx, int dy, Board* board) {
	return (sx == dx || sy == dy);

}