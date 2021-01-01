#include "board.hpp"

void Board::updateROI(int idx) {
    int row = idx/BOARD_SIZE;
    int col = idx%BOARD_SIZE;
    for (int r = max(0, row-2); r < min(row+3, BOARD_SIZE); r++) {
        for (int c = max(0, col-2); c < min(col+3, BOARD_SIZE); c++) {
            int i = BOARD_SIZE*r+c;
            roiCount[i]++;
            if (cells[i] == 0) roi.insert(i); // add if not occupied
            else roi.erase(i); // erase if occupied
        }
    }
}

unordered_set<int> Board::getROI() const {
    return roi;
}

bool Board::place(int linearIdx) {
    if (abs(linearIdx) >= BOARD_SIZE*BOARD_SIZE || cells[linearIdx] != 0) return false;
    cells[linearIdx] = turn;
    turn *= -1;
    pieceCount++;
    moveHist.push(linearIdx);
    updateROI(linearIdx); // update region of interest
    return true;
}

bool Board::place(int row, int col) {
    return place(BOARD_SIZE*row+col);
}

int Board::getScore(vector<int> &tile) {
    if (tile.size() <= 3) {
        tile.clear();
        return 0;
    }

    int color = 0;
    int length = 0;
    for (int i = 1; i < tile.size()-1; i++) {
        if (tile[i] != 0) {
            color = tile[i];
            length++;
        }
    }
    if (color == 0 || length < 2) {
        tile.clear();
        return 0;
    } else if (length >= 5) {
        tile.clear();
        return color*FIVE; 
    } // short circuit

    int score = 0;
    if (length == 2) score = TWO;
    if (length == 3) score = THREE;
    else if (length == 4) score = FOUR;

    int blocked = abs(*tile.begin()) + abs(*(tile.end()-1));
    if (blocked == 1) score /= 2;
    else if (blocked == 2) score = length;
    
    // for (int itm : tile) {
    //     if (itm == 1) cout << "x ";
    //     else if (itm == -1) cout << "o ";
    //     else cout << ". ";
    // }
    // cout << "length: " << length << " color: " << color << " blocked: " << blocked << endl;

    tile.clear();
    return color*score;
}

int Board::checkLine(vector<int> line) {
    // for (int itm : line) {
    //     if (itm == 1) cout << "x ";
    //     else if (itm == -1) cout << "o ";
    //     else cout << ". ";
    // }
    // cout << endl;

    int score = 0;
    int lastPiece = -line[0];
    vector<int> tile;
    tile.push_back(lastPiece);
    for (int i = 0; i < line.size(); i++) {
        int thisPiece = line[i];
        tile.push_back(thisPiece); // add to tile

        if (thisPiece == 0) {
            int nextPiece = -thisPiece;
            if (i+1 < line.size()) nextPiece = line[i+1];
            if (nextPiece != lastPiece) {
                score += getScore(tile);
                tile.push_back(0);
            }
        } else if (abs(thisPiece-lastPiece) == 2) { // color swap
            score += getScore(tile); // get score
            tile.push_back(lastPiece);
            tile.push_back(thisPiece); // color swap so new streak
        }
        lastPiece = thisPiece;
    }
    tile.push_back(-lastPiece);
    score += getScore(tile); // end of board
    return score;
}

int Board::checkRows() {
    int score = 0;
    for (int r = 0; r < BOARD_SIZE; r++) {
        vector<int> line(BOARD_SIZE);
        for (int c = 0; c < BOARD_SIZE; c++) {
            line[c] = cells[r*BOARD_SIZE+c];
        }
        score += checkLine(line);
    }
    return score;
}

int Board::checkCols() {
    int score = 0;
    for (int c = 0; c < BOARD_SIZE; c++) {
        vector<int> line(BOARD_SIZE);
        for (int r = 0; r < BOARD_SIZE; r++) {
            line[r] = cells[r*BOARD_SIZE+c];
        }
        score += checkLine(line);
    }
    return score;
}

int Board::checkDiags() {
    int score = 0;
    for (int k = 0; k < BOARD_SIZE*2-1; k++) {
        int i = max(0, k-BOARD_SIZE+1), j = min(k, BOARD_SIZE-1);

        vector<int> line(abs(j-i)+1);
        int n = 0;
        while (n < line.size()) {
            //cout << i << " " << j << endl;
            line[n++] = cells[(i++)*BOARD_SIZE + (j--)];
        }
        score += checkLine(line);
    }

    for (int k = 0; k < BOARD_SIZE*2-1; k++) {
        int i = max(0, k-BOARD_SIZE+1), j = min(k, BOARD_SIZE-1);

        vector<int> line(abs(j-i)+1);
        int n = 0;
        while (n < line.size()) {
            //cout << (BOARD_SIZE-1-i) << " " << j << endl;
            line[n++] = cells[(BOARD_SIZE-1-(i++))*BOARD_SIZE + (j--)];
        }
        //cout << endl;
        score += checkLine(line);
    }

    return score;
}

int Board::boardValue() {
    if (pieceCount >= BOARD_SIZE*BOARD_SIZE) return 0; // draw

    int score = 0;

    // check rows
    score += checkRows();
    score += checkCols();
    score += checkDiags();

    return score;
}

void Board::eraseLast() {
    if (moveHist.size() == 0) return;
    int idx = moveHist.top(); moveHist.pop();
    cells[idx] = 0;
    turn *= -1;
    pieceCount--;

    int row = idx/BOARD_SIZE;
    int col = idx%BOARD_SIZE;
    for (int r = max(0, row-2); r < min(row+3, BOARD_SIZE); r++) {
        for (int c = max(0, col-2); c < min(col+3, BOARD_SIZE); c++) {
            int i = BOARD_SIZE*r+c;
            roiCount[i]--;
            if (roiCount[i] == 0) roi.erase(i);
            else if (cells[i] == 0) roi.insert(i);
        }
    }
}

int Board::getTurn() {
    return turn;
}
