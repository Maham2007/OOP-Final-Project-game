#include "Bishop.h"

Bishop::Bishop(char s) : Piece(s) {}

bool Bishop::isValidMove(int sx, int sy, int dx, int dy, Board* board) {
	return (abs(dx - sx) == abs(dy - sy));
}