#include "Queen.h"

Queen::Queen(char s) : Piece(s) {}

bool Queen::isValidMove(int sx, int sy, int dx, int dy, Board* board) {
	return(sx == dx || sy == dy || abs(dx - sx) == abs(dy - sy));
}