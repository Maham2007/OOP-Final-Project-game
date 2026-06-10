#pragma once
#include <vector>
#include <string>
using namespace std;
class Piece {
protected:
    char color; // 'W' or 'B'
    char symbol;

public:
    Piece(char c, char s);
    virtual ~Piece() {}

    char getColor() const;
    char getSymbol() const;

    virtual bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) = 0;

    bool isEnemy(Piece* other) const;
    bool isPathClear(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) const;
};
// ---- Derived Piece Classes ----

class Pawn : public Piece {
public:
    bool hasMoved;             // tracks first move (for 2-square advance)
    bool enPassantVulnerable;  // true for exactly one turn after a 2-square advance

    Pawn(char c);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) override;
};

class Rook : public Piece {
public:
    bool hasMoved;  // needed for castling legality

    Rook(char c);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) override;
};

class Knight : public Piece {
public:
    Knight(char c);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) override;
};

class Bishop : public Piece {
public:
    Bishop(char c);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) override;
};

class Queen : public Piece {
public:
    Queen(char c);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) override;
};

class King : public Piece {
public:
    bool hasMoved;  // needed for castling legality

    King(char c);
    // Normal 1-square moves only; castling is handled in Board::movePiece
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece* board[8][8]) override;
};
