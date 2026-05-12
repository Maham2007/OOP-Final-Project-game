#include "Board.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include <iostream>

using namespace std;

Board::Board() {
    whiteturn = true;
    initialize();
}

Board::~Board() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            delete grid[i][j];
            grid[i][j] = nullptr;
        }
}

void Board::initialize() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            grid[i][j] = nullptr;

    for (int i = 0; i < 8; i++) {
        grid[6][i] = new Pawn('P');
        grid[1][i] = new Pawn('p');
    }

    grid[7][0] = new Rook('R');   grid[7][7] = new Rook('R');
    grid[0][0] = new Rook('r');   grid[0][7] = new Rook('r');

    grid[7][1] = new Knight('N'); grid[7][6] = new Knight('N');
    grid[0][1] = new Knight('n'); grid[0][6] = new Knight('n');

    grid[7][2] = new Bishop('B'); grid[7][5] = new Bishop('B');
    grid[0][2] = new Bishop('b'); grid[0][5] = new Bishop('b');

    grid[7][3] = new Queen('Q');  grid[0][3] = new Queen('q');
    grid[7][4] = new King('K');   grid[0][4] = new King('k');
}

void Board::display() {
    cout << "\n    1 2 3 4 5 6 7 8\n";
    cout << "  +-----------------+\n";
    for (int i = 0; i < 8; i++) {
        int label = i + 1;   // row 0 = label 1, row 7 = label 8
        cout << label << " | ";
        for (int j = 0; j < 8; j++) {
            if (grid[i][j])
                cout << grid[i][j]->getSymbol() << " ";
            else
                cout << ". ";
        }
        cout << "| " << label << "\n";
    }
    cout << "  +-----------------+\n";
    cout << "    1 2 3 4 5 6 7 8\n\n";
}

bool Board::movePiece(int sx, int sy, int dx, int dy) {

    Piece* p = grid[sx][sy];
    if (!p) return false;

    // Turn enforcement
    char s = p->getSymbol();
    if (whiteturn && s >= 'a') return false;
    if (!whiteturn && s <= 'Z') return false;

    if (!p->isValidMove(sx, sy, dx, dy, this))
        return false;

    Piece* dest = grid[dx][dy];

    // Prevent capturing own piece
    if (dest) {
        if (whiteturn && dest->getSymbol() <= 'Z') return false;
        if (!whiteturn && dest->getSymbol() >= 'a') return false;
    }

    // Path check for sliding pieces
    if (dynamic_cast<Rook*>(p) ||
        dynamic_cast<Bishop*>(p) ||
        dynamic_cast<Queen*>(p)) {
        if (!isPathClear(sx, sy, dx, dy))
            return false;
    }

    // Simulate move — reject if own king still in check
    grid[dx][dy] = p;
    grid[sx][sy] = nullptr;

    bool inCheck = isCheck(whiteturn);

    if (inCheck) {
        // Undo
        grid[sx][sy] = p;
        grid[dx][dy] = dest;
        return false;
    }

    whiteturn = !whiteturn;
    return true;
}

bool Board::isPathClear(int sx, int sy, int dx, int dy) {
    int stepX = (dx > sx) ? 1 : (dx < sx ? -1 : 0);
    int stepY = (dy > sy) ? 1 : (dy < sy ? -1 : 0);
    int x = sx + stepX, y = sy + stepY;
    while (x != dx || y != dy) {
        if (grid[x][y] != nullptr) return false;
        x += stepX; y += stepY;
    }
    return true;
}

bool Board::isCheck(bool white) {
    Position king = findKing(white);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if (!grid[i][j]) continue;
            char s = grid[i][j]->getSymbol();
            if ((white && s >= 'a') || (!white && s <= 'Z'))
                if (grid[i][j]->isValidMove(i, j, king.getX(), king.getY(), this))
                    return true;
        }
    return false;
}

// Tries every possible move for the side in trouble.
// Returns true if at least one move exists that gets out of check.
// This covers: king moving away, blocking with another piece, capturing the attacker.
bool Board::canEscapeCheck(bool white) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if (!grid[i][j]) continue;
            char s = grid[i][j]->getSymbol();

            // Only look at pieces belonging to the side that needs to escape
            if (white && s > 'Z') continue;
            if (!white && s <= 'Z') continue;

            for (int x = 0; x < 8; x++)
                for (int y = 0; y < 8; y++) {

                    if (!grid[i][j]->isValidMove(i, j, x, y, this)) continue;

                    // Sliding pieces need a clear path
                    if ((dynamic_cast<Rook*>(grid[i][j]) ||
                        dynamic_cast<Bishop*>(grid[i][j]) ||
                        dynamic_cast<Queen*>(grid[i][j])) &&
                        !isPathClear(i, j, x, y))
                        continue;

                    // Can't capture own piece
                    if (grid[x][y]) {
                        char t = grid[x][y]->getSymbol();
                        if (white && t <= 'Z') continue;
                        if (!white && t >= 'a') continue;
                    }

                    // Simulate the move
                    Piece* temp = grid[x][y];
                    Piece* moving = grid[i][j];
                    grid[x][y] = moving;
                    grid[i][j] = nullptr;

                    bool stillInCheck = isCheck(white);

                    // Undo simulation
                    grid[i][j] = moving;
                    grid[x][y] = temp;

                    if (!stillInCheck) return true;  // this move escapes check
                }
        }
    return false;  // no move found — either checkmate or stalemate
}

bool Board::isCheckmate(bool white) {
    if (!isCheck(white)) return false;   // not even in check
    return !canEscapeCheck(white);       // in check + no escape = checkmate
}

bool Board::isStalemate(bool white) {
    if (isCheck(white)) return false;    // in check = not stalemate
    return !canEscapeCheck(white);       // not in check + no legal move = stalemate
}

Position Board::findKing(bool white) {
    char king = white ? 'K' : 'k';
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (grid[i][j] && grid[i][j]->getSymbol() == king)
                return Position(i, j);
    return Position(-1, -1);
}

Piece* Board::getPiece(int x, int y) { return grid[x][y]; }
bool   Board::getTurn() { return whiteturn; }