#pragma once
#include<iostream>
#include<iomanip>
#include<vector>

#ifndef MAX_BOARD_SIZE_
#define MAX_BOARD_SIZE_
const int MAX_BOARD_SIZE = 19;
#endif

using namespace std;
class gomoku_board{
    public:
        // vector< vector<int> > get_board(){
        //     return board;
        // };
        void print_board();
        gomoku_board(int n);


        //return the stone at corresponding position
        int get(int py,int px);
        int get(pair<int,int> pt);
        //erase the stone at corresponding position
        //return 1 if the step is legal
        void add_stone(int color, pair<int,int> position);
        bool erase(int color, pair<int,int> position);

        int Board_size();
        bool in_board(int y, int x);
        bool in_board(pair<int,int> pt);
        bool is_valid_move(int y, int x);
        bool is_valid_move(pair<int,int> pt);
        const int board_size;
        void reset_all();

    protected:
        //board is (board_size+2)*(board_size+2), where the edge colored by 10
        // the value of board[i][j] is 1 if it's black, -1 if white
        // vector< vector<int> > board;
        int _board[MAX_BOARD_SIZE+2][MAX_BOARD_SIZE+2];
};
