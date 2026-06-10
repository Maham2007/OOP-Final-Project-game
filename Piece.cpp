#include "piece.h"
#include <cstdlib>
#include <stdexcept>
Piece::Piece(char c, char s) : color(c), symbol(s) {}
char Piece::getColor() const { return color; }
char Piece::getSymbol() const { return symbol; }
bool Piece::isEnemy(Piece* other) const {
    if (other == nullptr) return false;
    return other->getColor() != color;
}
bool Piece::isPathClear(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) const {
    int rowStep = 0, colStep = 0;
    if (toRow > fromRow) rowStep = 1;
    else if (toRow < fromRow) rowStep = -1;
    if (toCol > fromCol) colStep = 1;
    else if (toCol < fromCol) colStep = -1;
    int r = fromRow + rowStep;
    int c = fromCol + colStep;
    while (r != toRow || c != toCol) {
        if (board[r][c] != nullptr) return false;
        r += rowStep;
        c += colStep;
    }
    return true;
}
Pawn::Pawn(char c) : Piece(c, 'P'), hasMoved(false), enPassantVulnerable(false)
{
}
bool Pawn::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) {
    // Bounds check
    if (toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7)
        return false;

    int dir = (color == 'W') ? -1 : 1;
    int startRow = (color == 'W') ? 6 : 1;

    // Single step forward
    if (toCol == fromCol && toRow == fromRow + dir && board[toRow][toCol] == nullptr)
        return true;

    // Double step forward from starting row
    if (toCol == fromCol && fromRow == startRow &&
        toRow == fromRow + 2 * dir &&
        board[fromRow + dir][fromCol] == nullptr &&
        board[toRow][toCol] == nullptr)
        return true;

    // Diagonal capture of a real enemy piece
    if ((toCol == fromCol + 1 || toCol == fromCol - 1) &&
        toRow == fromRow + dir &&
        board[toRow][toCol] != nullptr &&
        isEnemy(board[toRow][toCol]))
        return true;

    // EN PASSANT: diagonal move to an EMPTY square.
    // The captured pawn sits beside us on the same row.
    // Board::movePiece handles physically removing the captured pawn.
    if ((toCol == fromCol + 1 || toCol == fromCol - 1) &&
        toRow == fromRow + dir &&
        board[toRow][toCol] == nullptr) {
        Piece* adjacent = board[fromRow][toCol];
        if (adjacent != nullptr &&
            adjacent->getSymbol() == 'P' &&
            isEnemy(adjacent)) {
            Pawn* adjacentPawn = dynamic_cast<Pawn*>(adjacent);
            if (adjacentPawn != nullptr && adjacentPawn->enPassantVulnerable)
                return true;
        }
    }

    return false;
}
Rook::Rook(char c) : Piece(c, 'R'), hasMoved(false) {}
bool Rook::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) {
    if (fromRow != toRow && fromCol != toCol) return false;
    if (!isPathClear(fromRow, fromCol, toRow, toCol, board)) return false;
    if (board[toRow][toCol] != nullptr && !isEnemy(board[toRow][toCol])) return false;
    return true;
}
Knight::Knight(char c) : Piece(c, 'N') {}
bool Knight::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) {
    int dr = abs(toRow - fromRow);
    int dc = abs(toCol - fromCol);
    if (!((dr == 2 && dc == 1) || (dr == 1 && dc == 2))) return false;
    if (board[toRow][toCol] != nullptr && !isEnemy(board[toRow][toCol])) return false;
    return true;
}
Bishop::Bishop(char c) : Piece(c, 'B') {}
bool Bishop::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) {
    int dr = abs(toRow - fromRow);
    int dc = abs(toCol - fromCol);
    if (dr != dc) return false;
    if (!isPathClear(fromRow, fromCol, toRow, toCol, board)) return false;
    if (board[toRow][toCol] != nullptr && !isEnemy(board[toRow][toCol])) return false;
    return true;
}
Queen::Queen(char c) : Piece(c, 'Q') {}
bool Queen::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) {
    int dr = abs(toRow - fromRow);
    int dc = abs(toCol - fromCol);
    bool straight = (fromRow == toRow || fromCol == toCol);
    bool diagonal = (dr == dc);
    if (!straight && !diagonal) return false;
    if (!isPathClear(fromRow, fromCol, toRow, toCol, board)) return false;
    if (board[toRow][toCol] != nullptr && !isEnemy(board[toRow][toCol])) return false;
    return true;
}
King::King(char c) : Piece(c, 'K'), hasMoved(false) {}
bool King::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) {
    int dr = abs(toRow - fromRow);
    int dc = abs(toCol - fromCol);
    if (dr > 1 || dc > 1) return false;
    if (board[toRow][toCol] != nullptr && !isEnemy(board[toRow][toCol])) return false;
    return true;
}

#pragma once
