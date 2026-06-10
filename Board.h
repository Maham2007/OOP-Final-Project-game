
#pragma once
#include "Piece.h"

class Board {
private:
private:
    Piece* grid[8][8];
    bool isSquareAttacked(int row, int col, char attackerColor) const;

public:
    Board();
    ~Board();

    void setupBoard();
    void display() const;

    Piece* getPiece(int row, int col) const;
    Piece* (*getGrid())[8] { return grid; }  // returns Piece*(*)[8] — pointer to first row
    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);

    bool isKingAlive(char color) const;
    bool isInCheck(char color) const;
    bool isCheckmate(char color);
    bool isValidMoveWithCheckProtection(int fromRow, int fromCol, int toRow, int toCol, char color);

    void clearEnPassant(char color);

    // A pawn that reached the back rank
    void promotePawn(int row, int col, char choice);

    // Stalemate detection
    bool isStalemate(char color);

    // Castling availability checks
    bool canCastleKingside(char color);
    bool canCastleQueenside(char color);
};
#pragma once
