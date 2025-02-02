#include<iostream>
#include<iomanip>

#ifndef vector
#include<vector>
#endif

#ifndef MAX_BOARD_SIZE_
#define MAX_BOARD_SIZE_
const int MAX_BOARD_SIZE = 19;
#endif

using namespace std;
class gomoku_game{
    public:
        // vector< vector<int> > get_board(){
        //     return board;
        // };
        void print_board();
        gomoku_game(int n);

        //return 1 if the step is legal
        void move(int color, pair<int,int> position);

        inline int get_board(int py,int px);
        inline int get_board(pair<int,int> pt);

        inline int get_board_size();
//    protected:
        //board is (board_size+2)*(board_size+2), where the edge colored by 10
        // the value of board[i][j] is 1 if it's black, -1 if white
        // vector< vector<int> > board;
        int board[MAX_BOARD_SIZE+2][MAX_BOARD_SIZE+2];
        int board_size;
        int current_color;// 1 if it is black's turn, -1 if it's white's
        bool remove(int color, pair<int,int> position);
        void reset(pair<int, int> position);
};
