#include "King.h"
#include "Board.h"

King::King(char s) : Piece(s) {}

bool King::isValidMove(int sx, int sy, int dx, int dy, Board* board) {

    // Block moving to same square
    if (sx == dx && sy == dy) return false;

    // Normal king move: 1 square any direction
    return abs(dx - sx) <= 1 && abs(dy - sy) <= 1;
}