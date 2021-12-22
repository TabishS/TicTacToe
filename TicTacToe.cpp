/* This version of the program is an initial rough version. It works by creating a 
   tree of all possible Tic-Tac-Toe games and board states. While this version does 
   work, a  major change to make is that it does not treat game board states as unique 
   copies, and instead has multiple copies of the same game board states in separate 
   branches. Ideally, the graph of states would be optimized to get rid of duplicates.

   Another area for optimization is setting up an easier way to count the number of 
   possible future states, that come from a given initial state. Currently, the closest
   thing to this is counting all the descendants of the current state's node in a tree
   and counting the number of possible game ending paths from a current state.         */

// clang++ -Wall -std=c++14 TicTacToe.cpp -o TicTacToe

#include <vector>
#include <iostream>
#include <math.h>
using std::vector;
using std::cout; using std::endl;

struct Board {
    vector<char> state;
    vector<Board *> nextStates;
    bool turn;
    bool done;

    Board();
    Board(const Board & board);
    ~Board();
    Board(vector<char> State, int Turn) : state(State), turn(Turn) {};
    bool operator!=(const Board & board);
    void getNextStates();
    void printBoard();
    int countChildStates();
    int countChildPaths();
};

Board::Board() {
    state.resize(9);
    nextStates.resize(0);
    for(int i = 0; i < 9; ++i)
        state[i] = '_';
    turn = 0;
    done = 0;
}

Board::Board(const Board & board) {
    state.resize(board.state.size());
    for(int i = 0; i < state.size(); ++i)
        state[i] = board.state[i];
    nextStates.resize(board.nextStates.size());
    for(int i = 0; i < nextStates.size(); ++i)
        nextStates[i] = board.nextStates[i];
    turn = board.turn; // More efficient to check conditions each time? Or nah
    done = board.done;
}

bool Board::operator!=(const Board & board) {
    if(nextStates.size() != board.nextStates.size() || turn != board.turn || done != board.done)
        return 1;
    for(int i = 0; i < state.size(); ++i)
        if(state[i] != board.state[i])
            return 1;
    for(int i = 0; i < nextStates.size(); ++i)
        if(nextStates[i] != board.nextStates[i])
            return 1;
    return 0;
}

void Board::getNextStates() {
    nextStates.resize(0);
    char prev = turn ? 'X' : 'O';
    {
        bool Zero = (state[0] == prev), One = (state[1] == prev), Two = (state[2] == prev),
        Three = (state[3] == prev), Four = (state[4] == prev), Five = (state[5] == prev),
        Six = (state[6] == prev), Seven = (state[7] == prev), Eight = (state[8] == prev);
        if((Zero && Three && Six) || (One && Four && Seven) || (Two && Five && Eight) || (Zero && One && Two) ||
            (Three && Four && Five) || (Six && Seven && Eight) || (Zero && Four && Eight) || (Two && Four && Six)) {
            done = 1;
            return;
        }
    }
    turn = !turn;
    char cur = turn ? 'X' : 'O';
    for(int i = 0; i < state.size(); ++i)
        if(state[i] == '_') {
            Board * newBoard = new Board(*this);
            newBoard->state[i] = cur;
            nextStates.push_back(newBoard);
            newBoard->getNextStates();
        }
}

void Board::printBoard() {
    int i = 0;
    for(int I = 0; I < sqrt(state.size()); ++I) {
        for(int J = 0; J < sqrt(state.size()); ++J, ++i)
            cout << state[i] << " ";
        cout << endl;
    }
}

int Board::countChildStates() {
    int count = nextStates.size();
    for(int i = 0; i < nextStates.size(); ++i) {
        count += nextStates[i]->countChildStates();
    }
    return count;
}

int Board::countChildPaths() {
    if(!nextStates.size())
        return 1;
    int count = 0;
    for(int i = 0; i < nextStates.size(); ++i)
        count += nextStates[i]->countChildPaths();
    return count;
}

int main() {
    Board * board = new Board();
    board->getNextStates();
    cout << "From an empty Tic-Tac-Toe board, there are " << board->countChildPaths() << " possible games" << endl;

    Board * testBoard = new Board({'X', 'O', 'X', 'O', 'X', 'O', '_', 'X', '_'}, 1);
    testBoard->getNextStates();
    testBoard->printBoard();
    cout << "The board has " << testBoard->countChildStates() << " possible subsequent boards" << endl;
    cout << "The board has " << testBoard->countChildPaths() << " possible subsequent paths" << endl;
    return 0;
}