#include "Pawn.h"
#include "Board.h"

Pawn::Pawn(char s) : Piece(s) {}

bool Pawn::isValidMove(int sx, int sy, int dx, int dy, Board* board) {

    if (symbol == 'P') {  // White pawn — moves upward (row DECREASES now)

        if (dx == sx - 1 && dy == sy && board->getPiece(dx, dy) == nullptr)
            return true;

        // First move from row 6
        if (sx == 6 && dx == sx - 2 && dy == sy
            && board->getPiece(sx - 1, sy) == nullptr
            && board->getPiece(dx, dy) == nullptr)
            return true;

        if (dx == sx - 1 && abs(dy - sy) == 1 && board->getPiece(dx, dy) != nullptr)
            return true;
    }
    else {  // Black pawn — moves downward (row INCREASES now)

        if (dx == sx + 1 && dy == sy && board->getPiece(dx, dy) == nullptr)
            return true;

        // First move from row 1
        if (sx == 1 && dx == sx + 2 && dy == sy
            && board->getPiece(sx + 1, sy) == nullptr
            && board->getPiece(dx, dy) == nullptr)
            return true;

        if (dx == sx + 1 && abs(dy - sy) == 1 && board->getPiece(dx, dy) != nullptr)
            return true;
    }

    return false;
}
