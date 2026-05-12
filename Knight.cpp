#include "Knight.h"

Knight::Knight(char s) : Piece(s) {}

bool Knight::isValidMove(int sx, int sy, int dx, int dy, Board* board) {
	return (abs(sx - dx) == 2 && abs(dy - sy) == 1) || (abs(dx - sx) == 1 && abs(dy - sy) == 2);
}