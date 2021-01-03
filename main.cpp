#include <iostream>
#include<limits>
#include "board.hpp"

using namespace std;

int evaluate(Board &board, int depth, int alpha, int beta, int maxDepth) {
    // int winner = board.isOver();
    // if (winner != 0) return FIVE*winner;
    // else if (depth == maxDepth) return board.eval();
    int eval = board.eval();
    if (depth == maxDepth || abs(eval) >= FOUR) return eval;

    bool isMax = true;
    int bestEval = -FIVE;
    int bestMove = -1;
    if (board.getTurn() == -1) {
        isMax = false;
        bestEval = FIVE;
    }

    for (int i : board.getROI()) { // try everything
        board.place(i);
        //cout << board << endl;
        int eval = evaluate(board, depth+1, alpha, beta, maxDepth);
        if ((isMax && eval > bestEval) || (!isMax && eval < bestEval)) {
            bestEval = eval;
            bestMove = i;
        }

        board.eraseLast();
        //cout << board << endl;

        if (isMax) alpha = max(alpha, eval);
        else beta = min(beta, eval);
        if (alpha >= beta) break;
    }

    if (depth == 0) return bestMove;
    return bestEval;
}

int main(int argc, char const *argv[]) {
    Board board;

    // board.place(6, 5); board.place(7, 5);
    // board.place(7, 4); board.place(8, 5);
    // board.place(7, 7); board.place(8, 7);
    // board.place(7, 8); board.place(9, 5);
    // board.place(8, 4); board.place(9, 6);
    // board.place(8, 6); board.place(9, 8);
    // board.place(8, 9); board.place(10, 7);
    // board.place(9, 7); board.place(10, 8);
    // board.place(10, 6); board.place(10, 9);
    // board.place(11, 10); board.place(11, 8);
    // board.place(12, 9); board.place(12, 8);

    // cout << board << endl;

    // cout << board.eval() << endl;
        
    std::cout << "enter a coordinate separated by space: " << endl; 
    while (true) {
        // user
        int row, col;
        do {
            cin >> row >> col;
            while(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                std::cout<< "invalid input" <<endl;
                cin >> row >> col;
            }
        } while (board.place(row, col) == false);
        std::cout << board << endl;
        if (abs(board.boardValue()) >= FOUR) break; // has a result

        // computer
        board.place(evaluate(board, 0, -FIVE, FIVE, 5));
        std::cout << board << endl;
        if (abs(board.boardValue()) >= FOUR) break; // has a result
    }

    char winner = 'x';
    if (board.boardValue() < 0) winner = 'o';
    std::cout << "the winner is: " << winner << endl; 
    
    return 0;
}
