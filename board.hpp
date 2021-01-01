#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <stack>
#include <vector>
#include <unordered_set>

#define BOARD_SIZE 15

#define TWO 8
#define THREE 32
#define FOUR 64
#define FIVE 65536

using namespace std;

class Board {
private:
    int cells[BOARD_SIZE*BOARD_SIZE] = {0};
    int turn = 1;
    int pieceCount = 0;
    stack<int> moveHist;
    int roiCount[BOARD_SIZE*BOARD_SIZE] = {0};
    unordered_set<int> roi;

    int checkLine(vector<int> line);
    int checkRows();
    int checkCols();
    int checkDiags();
    void updateROI(int piece);
public:
    Board() = default;
    ~Board() = default;

    bool place(int linearIdx);
    bool place(int row, int col);
    int boardValue();
    void eraseLast();
    int getTurn();
    int getScore(vector<int> &tile);
    unordered_set<int> getROI() const;
    
    friend ostream & operator << (std::ostream & _os, Board const &board) {
        for (int r = 0; r < BOARD_SIZE; r++) {
            for (int c = 0; c < BOARD_SIZE; c++) {
                char symbol = '.';
                int idx = BOARD_SIZE*r+c;
                int value = board.cells[idx];
                if (value == 1) symbol = 'x';
                else if (value == -1) symbol = 'o';
                else {
                    auto roi = board.getROI();
                    if (roi.find(idx) != roi.end()) symbol = '_';
                }
                _os << symbol << " ";
            }
            _os << '\n';
        }
        return _os;
    }
};

#endif