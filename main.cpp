#include "Board.h"
#include <iostream>
using namespace std;

int main() {
    Board board;

    while (true) {
        board.display();

        bool myTurn = board.getTurn();
        cout << (myTurn ? "White" : "Black") << "'s turn\n";

        // Warn the player if they are currently in check
        if (board.isCheck(myTurn))
            cout << "*** YOU ARE IN CHECK! Move your king or block the attack ***\n";

        int sx, sy, dx, dy;
        cout << "Enter move (row col row col): ";
        cin >> sx >> sy >> dx >> dy;

        if (board.movePiece(sx, sy, dx, dy)) {
            cout << "Moved!\n";

            bool opponent = board.getTurn();  // turn has flipped after move

            if (board.isCheckmate(opponent)) {
                board.display();
                cout << "*** CHECKMATE! " << (opponent ? "Black" : "White") << " wins! ***\n";
                break;
            }
            if (board.isStalemate(opponent)) {
                board.display();
                cout << "*** STALEMATE! It's a draw! ***\n";
                break;
            }
            if (board.isCheck(opponent))
                cout << "*** CHECK! ***\n";
        }
        else {
            cout << "Invalid move! ";
            if (board.isCheck(myTurn))
                cout << "You must resolve the check first.\n";
            else
                cout << "Try again.\n";
        }
    }

    system("pause");
    return 0;
}