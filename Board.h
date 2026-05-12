#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Position.h"

class Board {
private:
    Piece* grid[8][8];
    bool whiteturn;

public:
    Board();
    ~Board();
    void initialize();
    void display();

    bool movePiece(int sx, int sy, int dx, int dy);
    bool isPathClear(int sx, int sy, int dx, int dy);

    bool isCheck(bool white);
    bool isCheckmate(bool white);
    bool isStalemate(bool white);
    bool canEscapeCheck(bool white);

    Position findKing(bool white);
    Piece* getPiece(int x, int y);
    bool getTurn();
};

#endif