#pragma once
#include "Board.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Board::Board() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            grid[i][j] = nullptr;
    setupBoard();
}

Board::~Board() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            delete grid[i][j];
            grid[i][j] = nullptr;
        }
}

void Board::setupBoard() {
    // Black back row
    grid[0][0] = new Rook('B');
    grid[0][1] = new Knight('B');
    grid[0][2] = new Bishop('B');
    grid[0][3] = new Queen('B');
    grid[0][4] = new King('B');
    grid[0][5] = new Bishop('B');
    grid[0][6] = new Knight('B');
    grid[0][7] = new Rook('B');
    for (int j = 0; j < 8; j++)
        grid[1][j] = new Pawn('B');

    // White back row
    grid[7][0] = new Rook('W');
    grid[7][1] = new Knight('W');
    grid[7][2] = new Bishop('W');
    grid[7][3] = new Queen('W');
    grid[7][4] = new King('W');
    grid[7][5] = new Bishop('W');
    grid[7][6] = new Knight('W');
    grid[7][7] = new Rook('W');
    for (int j = 0; j < 8; j++)
        grid[6][j] = new Pawn('W');
}

void Board::display() const
{
    cout << "\n";
    cout << "=========================================================\n";
    cout << "                  GRANDMASTER'S GAMBIT\n";
    cout << "=========================================================\n\n";

    cout << "       A    B    C    D    E    F    G    H\n";
    cout << "    +----+----+----+----+----+----+----+----+\n";

    for (int i = 0; i < 8; i++)
    {
        cout << " " << (8 - i) << "  |";

        for (int j = 0; j < 8; j++)
        {
            if (grid[i][j] == nullptr)
            {
                cout << "    |";
            }
            else
            {
                cout << " "
                    << grid[i][j]->getColor()
                    << grid[i][j]->getSymbol()
                    << " |";
            }
        }

        cout << "  " << (8 - i) << "\n";
        cout << "    +----+----+----+----+----+----+----+----+\n";
    }

    cout << "       A    B    C    D    E    F    G    H\n\n";

    cout << "WK=White King   BK=Black King\n";
    cout << "WQ=White Queen  BQ=Black Queen\n";
    cout << "WR=White Rook   BR=Black Rook\n";
    cout << "WB=White Bishop BB=Black Bishop\n";
    cout << "WN=White Knight BN=Black Knight\n";
    cout << "WP=White Pawn   BP=Black Pawn\n\n";
}

Piece* Board::getPiece(int row, int col) const {
    if (row < 0 || row > 7 || col < 0 || col > 7)
        throw out_of_range("Board::getPiece — coordinates out of range.");
    return grid[row][col];
}

bool Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7)
        throw invalid_argument("Source square is out of bounds.");
    if (toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7)
        throw invalid_argument("Destination square is out of bounds.");

    Piece* piece = grid[fromRow][fromCol];
    if (piece == nullptr)
        throw invalid_argument("No piece at source square.");

    char color = piece->getColor();
    char enemy = (color == 'W') ? 'B' : 'W';

    // CASTLING: King moves two squares sideways
    if (piece->getSymbol() == 'K' &&
        fromRow == toRow &&
        abs(toCol - fromCol) == 2) {

        bool kingSide = (toCol == 6);

        if (kingSide && !canCastleKingside(color))
            throw runtime_error("Kingside castling is not available.");
        if (!kingSide && !canCastleQueenside(color))
            throw runtime_error("Queenside castling is not available.");

        int rookFromCol = kingSide ? 7 : 0;
        int rookToCol = kingSide ? 5 : 3;

        grid[toRow][toCol] = piece;
        grid[fromRow][fromCol] = nullptr;
        dynamic_cast<King*>(piece)->hasMoved = true;

        grid[toRow][rookToCol] = grid[toRow][rookFromCol];
        grid[toRow][rookFromCol] = nullptr;
        dynamic_cast<Rook*>(grid[toRow][rookToCol])->hasMoved = true;

        return true;
    }

    //  MOVE VALIDATION
    if (!piece->isValidMove(fromRow, fromCol, toRow, toCol, grid))
        return false;

    // EN PASSANT CAPTURE 
    bool enPassantCapture = false;
    if (piece->getSymbol() == 'P' &&
        fromCol != toCol &&
        grid[toRow][toCol] == nullptr) {
        Piece* captured = grid[fromRow][toCol];
        if (!captured || captured->getSymbol() != 'P' || captured->getColor() != enemy)
            return false;
        delete grid[fromRow][toCol];
        grid[fromRow][toCol] = nullptr;
        enPassantCapture = true;
    }
    if (!enPassantCapture)
        delete grid[toRow][toCol];
    grid[toRow][toCol] = piece;
    grid[fromRow][fromCol] = nullptr;

    if (piece->getSymbol() == 'K')
        dynamic_cast<King*>(piece)->hasMoved = true;
    else if (piece->getSymbol() == 'R')
        dynamic_cast<Rook*>(piece)->hasMoved = true;

    // MARK EN PASSANT VULNERABILITY 
    if (piece->getSymbol() == 'P') {
        Pawn* pawn = dynamic_cast<Pawn*>(piece);
        if (pawn) {
            if (abs(toRow - fromRow) == 2)
                pawn->enPassantVulnerable = true;
            pawn->hasMoved = true;
        }
    }
    return true;
}

bool Board::isKingAlive(char color) const {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (grid[i][j] != nullptr &&
                grid[i][j]->getColor() == color &&
                grid[i][j]->getSymbol() == 'K')
                return true;
    return false;
}

bool Board::isInCheck(char color) const {
    int kingRow = -1, kingCol = -1;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (grid[i][j] &&
                grid[i][j]->getColor() == color &&
                grid[i][j]->getSymbol() == 'K') {
                kingRow = i; kingCol = j;
            }
    if (kingRow == -1) return false;
    char enemy = (color == 'W') ? 'B' : 'W';
    return isSquareAttacked(kingRow, kingCol, enemy);
}

bool Board::isCheckmate(char color) {
    // Not checkmate if not even in check
    if (!isInCheck(color)) return false;

    // Try every possible move for every piece of this color
    // If any move results in the king no longer being in check, it's NOT checkmate
    for (int fromRow = 0; fromRow < 8; fromRow++) {
        for (int fromCol = 0; fromCol < 8; fromCol++) {
            Piece* piece = grid[fromRow][fromCol];
            if (piece == nullptr || piece->getColor() != color) continue;

            for (int toRow = 0; toRow < 8; toRow++) {
                for (int toCol = 0; toCol < 8; toCol++) {
                    if (!piece->isValidMove(fromRow, fromCol, toRow, toCol, grid)) continue;

                    // Simulate the move
                    Piece* captured = grid[toRow][toCol];
                    grid[toRow][toCol] = piece;
                    grid[fromRow][fromCol] = nullptr;

                    bool stillInCheck = isInCheck(color);

                    // Undo the move
                    grid[fromRow][fromCol] = piece;
                    grid[toRow][toCol] = captured;

                    if (!stillInCheck)
                        return false; // Found a move that escapes check
                }
            }
        }
    }

    // No escape found — it's checkmate
    return true;

}

bool Board::isValidMoveWithCheckProtection(int fromRow, int fromCol, int toRow, int toCol, char color) {
    Piece* piece = grid[fromRow][fromCol];
    if (!piece) return false;

    // Detect en passant: diagonal pawn move to empty square
    bool isEnPassant = (piece->getSymbol() == 'P' &&
        fromCol != toCol &&
        grid[toRow][toCol] == nullptr);

    Piece* captured = grid[toRow][toCol];
    Piece* epCaptured = nullptr;

    // Simulate the move
    grid[toRow][toCol] = piece;
    grid[fromRow][fromCol] = nullptr;
    if (isEnPassant) {
        epCaptured = grid[fromRow][toCol];
        grid[fromRow][toCol] = nullptr;
    }

    bool leavesKingInCheck = isInCheck(color);
    grid[fromRow][fromCol] = piece;
    grid[toRow][toCol] = captured;
    if (isEnPassant)
        grid[fromRow][toCol] = epCaptured;

    return !leavesKingInCheck;
}

bool Board::isSquareAttacked(int row, int col, char attackerColor) const {
    Piece* (&mutableGrid)[8][8] = const_cast<Piece * (&)[8][8]>(grid);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            Piece* p = mutableGrid[i][j];
            if (p && p->getColor() == attackerColor)
                if (p->isValidMove(i, j, row, col, mutableGrid))
                    return true;
        }
    return false;
}

void Board::clearEnPassant(char color) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            Piece* p = grid[i][j];
            if (p && p->getColor() == color && p->getSymbol() == 'P') {
                Pawn* pawn = dynamic_cast<Pawn*>(p);
                if (pawn) pawn->enPassantVulnerable = false;
            }
        }
}

void Board::promotePawn(int row, int col, char choice) {
    if (row < 0 || row > 7 || col < 0 || col > 7)
        throw out_of_range("promotePawn — coordinates out of range.");

    Piece* p = grid[row][col];
    if (!p || p->getSymbol() != 'P')
        throw logic_error("promotePawn — no pawn at specified square.");

    char color = p->getColor();
    delete grid[row][col];

    switch (choice) {
    case 'Q': grid[row][col] = new Queen(color);  break;
    case 'R': grid[row][col] = new Rook(color);   break;
    case 'B': grid[row][col] = new Bishop(color); break;
    case 'N': grid[row][col] = new Knight(color); break;
    default:  grid[row][col] = new Queen(color);  break;
    }
}

bool Board::isStalemate(char color) {
    if (isInCheck(color)) return false;

    for (int fromRow = 0; fromRow < 8; fromRow++)
        for (int fromCol = 0; fromCol < 8; fromCol++) {
            Piece* piece = grid[fromRow][fromCol];
            if (!piece || piece->getColor() != color) continue;
            for (int toRow = 0; toRow < 8; toRow++)
                for (int toCol = 0; toCol < 8; toCol++) {
                    if (!piece->isValidMove(fromRow, fromCol, toRow, toCol, grid)) continue;
                    if (isValidMoveWithCheckProtection(fromRow, fromCol, toRow, toCol, color))
                        return false;
                }
        }
    return true;
}

bool Board::canCastleKingside(char color) {
    int row = (color == 'W') ? 7 : 0;
    char enemy = (color == 'W') ? 'B' : 'W';

    Piece* king = grid[row][4];
    Piece* rook = grid[row][7];
    if (!king || king->getSymbol() != 'K' || king->getColor() != color) return false;
    if (!rook || rook->getSymbol() != 'R' || rook->getColor() != color) return false;

    King* k = dynamic_cast<King*>(king);
    Rook* r = dynamic_cast<Rook*>(rook);
    if (!k || k->hasMoved) return false;
    if (!r || r->hasMoved) return false;

    if (grid[row][5] || grid[row][6]) return false;

    if (isSquareAttacked(row, 4, enemy)) return false;
    if (isSquareAttacked(row, 5, enemy)) return false;
    if (isSquareAttacked(row, 6, enemy)) return false;

    return true;
}

bool Board::canCastleQueenside(char color) {
    int row = (color == 'W') ? 7 : 0;
    char enemy = (color == 'W') ? 'B' : 'W';

    Piece* king = grid[row][4];
    Piece* rook = grid[row][0];
    if (!king || king->getSymbol() != 'K' || king->getColor() != color) return false;
    if (!rook || rook->getSymbol() != 'R' || rook->getColor() != color) return false;

    King* k = dynamic_cast<King*>(king);
    Rook* r = dynamic_cast<Rook*>(rook);
    if (!k || k->hasMoved) return false;
    if (!r || r->hasMoved) return false;

    if (grid[row][1] || grid[row][2] || grid[row][3]) return false;

    if (isSquareAttacked(row, 4, enemy)) return false;
    if (isSquareAttacked(row, 3, enemy)) return false;
    if (isSquareAttacked(row, 2, enemy)) return false;

    return true;
}



