#include "chess.h"
#include <iostream>
#include <stdexcept>
#include <limits>
using namespace std;

Game::Game() : currentTurn('W') {}

void Game::parsePosition(const string& pos, int& row, int& col) {
    if (pos.size() < 2)
        throw invalid_argument("Position string too short: \"" + pos + "\"");
    col = pos[0] - 'a';
    row = 8 - (pos[1] - '0');
}

bool Game::isValidInput(const string& pos) {
    if (pos.size() != 2) return false;
    if (pos[0] < 'a' || pos[0] > 'h') return false;
    if (pos[1] < '1' || pos[1] > '8') return false;
    return true;
}

char Game::askPromotion() {
    char choice = 'Q';
    cout << "\n  *** PAWN PROMOTION! ***\n";
    cout << "  Choose promotion piece:\n";
    cout << "    Q = Queen  (recommended)\n";
    cout << "    R = Rook\n";
    cout << "    B = Bishop\n";
    cout << "    N = Knight\n";
    cout << "  Your choice: ";

    string input;
    while (true) {
        try {
            if (!(cin >> input))
                throw runtime_error("Failed to read promotion choice.");
            if (input.size() != 1)
                throw invalid_argument("Please enter exactly one letter (Q/R/B/N).");
            char c = toupper(input[0]);
            if (c != 'Q' && c != 'R' && c != 'B' && c != 'N')
                throw invalid_argument("Invalid choice. Enter Q, R, B, or N.");
            choice = c;
            break;
        }
        catch (const invalid_argument& e) {
            cout << "  [!] " << e.what() << "\n  Your choice: ";
        }
    }
    return choice;
}

bool Game::handleCastling(const string& cmd) {
    int row = (currentTurn == 'W') ? 7 : 0;

    if (cmd == "O-O" || cmd == "o-o") {
        if (!board.canCastleKingside(currentTurn))
            throw runtime_error("Kingside castling is not available right now.");
        if (!board.isValidMoveWithCheckProtection(row, 4, row, 6, currentTurn))
            throw runtime_error("Castling would leave your King in check.");
        board.movePiece(row, 4, row, 6);
        return true;
    }

    if (cmd == "O-O-O" || cmd == "o-o-o") {
        if (!board.canCastleQueenside(currentTurn))
            throw runtime_error("Queenside castling is not available right now.");
        if (!board.isValidMoveWithCheckProtection(row, 4, row, 2, currentTurn))
            throw runtime_error("Castling would leave your King in check.");
        board.movePiece(row, 4, row, 2);
        return true;
    }

    return false;
}

void Game::start() {
    cout << "=========================================================\n";
    cout << "           *********** CHESS GAME *********\n";
    cout << "=========================================================\n";
    cout << "    Made with love by Nawal and Noor \n";

    cout << "    Chess is the gymnasium of the mind  \n";
    cout << "                        ~ Blaise Pascal \n";
    cout << "=========================================================\n";
    cout << "                 GRANDMASTER'S GAMBIT\n";
    cout << "=========================================================\n";
    cout << " Commands:\n\n";
    cout << "   Move Piece      -> e2 e4\n";
    cout << "   Kingside Castle -> O-O\n";
    cout << "   Queenside Castle-> O-O-O\n";
    cout << "   Quit Game       -> quit\n\n";
    string from, to;

    while (true) {
        board.display();

        // ---- Checkmate ----
        if (board.isCheckmate(currentTurn)) {
            char winner = (currentTurn == 'W') ? 'B' : 'W';
            cout << "==========================================\n";
            cout << "  CHECKMATE! " << (winner == 'W' ? "White" : "Black") << " wins!\n";
            cout << "==========================================\n";
            break;
        }

        // ---- Stalemate ----
        if (board.isStalemate(currentTurn)) {
            cout << "==========================================\n";
            cout << "  STALEMATE! The game is a draw.\n";
            cout << "==========================================\n";
            break;
        }

        if (board.isInCheck(currentTurn))
            cout << "  !! " << (currentTurn == 'W' ? "WHITE" : "BLACK") << " IS IN CHECK !!\n\n";

        cout << (currentTurn == 'W' ? "White" : "Black") << "'s turn.\n";
        cout << "Enter move: ";

        try {
            if (!(cin >> from))
                throw runtime_error("Input stream error. Please try again.");

            // Quit
            if (from == "quit" || from == "exit") {
                cout << "Game ended by player. Goodbye!\n";
                break;
            }

            // ---- Castling shorthand ----
            if (from == "O-O" || from == "o-o" ||
                from == "O-O-O" || from == "o-o-o") {
                if (handleCastling(from)) {
                    board.clearEnPassant(currentTurn);
                    currentTurn = (currentTurn == 'W') ? 'B' : 'W';
                    continue;
                }
            }

            // ---- Normal move ----
            if (!(cin >> to))
                throw runtime_error("Failed to read destination square.");

            if (!isValidInput(from))
                throw invalid_argument(
                    "Invalid 'from' square \"" + from + "\". Use format: e2");
            if (!isValidInput(to))
                throw invalid_argument(
                    "Invalid 'to' square \"" + to + "\". Use format: e4");

            int fromRow, fromCol, toRow, toCol;
            parsePosition(from, fromRow, fromCol);
            parsePosition(to, toRow, toCol);

            Piece* piece = board.getPiece(fromRow, fromCol);
            if (piece == nullptr)
                throw runtime_error("No piece at " + from + "!");
            if (piece->getColor() != currentTurn)
                throw runtime_error("That is not your piece!");
            if (!piece->isValidMove(fromRow, fromCol, toRow, toCol, board.getGrid()))
                throw runtime_error("Invalid move for this piece!");
            if (!board.isValidMoveWithCheckProtection(fromRow, fromCol, toRow, toCol, currentTurn))
                throw runtime_error("This move would leave your King in check!");

            // Clear opponent's en passant vulnerability before moving

            board.movePiece(fromRow, fromCol, toRow, toCol);
            char opponent = (currentTurn == 'W') ? 'B' : 'W';
            board.clearEnPassant(opponent);

            // ---- Pawn Promotion ----
            Piece* movedPiece = board.getPiece(toRow, toCol);
            if (movedPiece && movedPiece->getSymbol() == 'P') {
                if ((movedPiece->getColor() == 'W' && toRow == 0) ||
                    (movedPiece->getColor() == 'B' && toRow == 7)) {
                    char promoteTo = askPromotion();
                    board.promotePawn(toRow, toCol, promoteTo);
                    cout << "  Pawn promoted to "
                        << (promoteTo == 'Q' ? "Queen" :
                            promoteTo == 'R' ? "Rook" :
                            promoteTo == 'B' ? "Bishop" : "Knight") << "!\n\n";
                }
            }

            currentTurn = (currentTurn == 'W') ? 'B' : 'W';
        }
        catch (const invalid_argument& e) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] " << e.what() << "\n\n";
        }
        catch (const runtime_error& e) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] " << e.what() << "\n\n";
        }
        catch (const out_of_range& e) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Out-of-range error: " << e.what() << "\n\n";
        }
        catch (const logic_error& e) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Logic error: " << e.what() << "\n\n";
        }
        catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] An unexpected error occurred. Please try again.\n\n";
        }
    }
}
