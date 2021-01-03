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
    int isOver();
    int quickScore(int streak, int blocked);
    int eval();
    
    friend ostream & operator << (std::ostream & _os, Board const &board) {
        _os << "   ";
        int n = 0;
        for (int c = 0; c < BOARD_SIZE; c++) {
            if ((n++) % 2 == 0) _os << "\033[0;" << "43m" << c << "\033[0m ";
            else _os << "\033[0;" << "44m" << c << "\033[0m ";
            if (c <= 9) _os << ' '; // add another
        }
        _os << '\n';
        
        n = 0;
        for (int r = 0; r < BOARD_SIZE; r++) {
            if ((n++) % 2 == 0) _os << "\033[0;" << "43m" << r << "\033[0m ";
            else _os << "\033[0;" << "44m" << r << "\033[0m ";
            if (r <= 9) _os << ' '; // add another
            
            for (int c = 0; c < BOARD_SIZE; c++) {
                string symbol = ".";
                int idx = BOARD_SIZE*r+c;
                int value = board.cells[idx];
                if (value == 1) symbol = "\033[1;41mx\033[0m";
                else if (value == -1) symbol = "\033[1;42mo\033[0m";
                else {
                    auto roi = board.getROI();
                    if (roi.find(idx) != roi.end()) symbol = "_";
                }
                _os << symbol << "  ";
            }
            _os << '\n';
        }
        return _os;
    }
};

#endif