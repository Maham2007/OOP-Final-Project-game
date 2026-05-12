#ifndef PIECE_H
#define PIECE_H

#include <cmath>
using namespace std;

class Board; // forward declaration

class Piece {
protected:
    char symbol;

public:
    Piece(char s) : symbol(s) {}

    virtual ~Piece() {}

    char getSymbol() {
        return symbol;
    }

    virtual bool isValidMove(int sx, int sy, int dx, int dy, Board* board) = 0;
};

#endif