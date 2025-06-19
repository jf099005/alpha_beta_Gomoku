#include<iostream>
#include<iomanip>
#include<vector>
#include<cassert>
#include<cstring>

#ifndef MAX_BOARD_SIZE_
#define MAX_BOARD_SIZE_
const int MAX_BOARD_SIZE = 19;
#endif

#ifndef GOMOKU
#define GOMOKU
#include"../H/Gomoku_board.h"
#endif

using namespace std;

// ostream &operator <<(ostream &ofs, pair<int,int> pt){
//     ofs<<"("<<pt.first<<","<<pt.second<<")";
//     return ofs;
// }
void gomoku_board::print_board(){
    cout<<"==========\n";
    cout<<setw(3)<<"\\";
    for(int i=0;i<=board_size;i++)cout<<setw(3)<<i;
    cout<<endl;
    for(int i=0; i<=board_size+1; i++){
        cout<<setw(3)<<i;
        for(int j=0; j<=board_size+1; j++){
            cout<<setw(3)<<( _board[i][j]==0?'.': (_board[i][j]==1? 'O':_board[i][j] == -1?'X':'?') );
        }
        cout<<"\n";
    }
    cout<<"==========\n";

};
gomoku_board::gomoku_board(int n):
    board_size(n)
{
    memset(_board,0,sizeof(_board));
    for(int i=0;i<=n+1;i++){
        _board[i][0] = 10;
        _board[0][i] = 10;
        _board[n+1][i] = 10;
        _board[i][n+1] = 10;
    }
};

//return 1 if the step is legal
void gomoku_board::add_stone(int color, pair<int,int> position){
    int py = position.first, px = position.second;
    if( min(px,py)<=0 || max(px,py)>board_size )
        return;
    else if(_board[py][px] != 0)
        return;
    else{
        _board[py][px] = color;
        return;
    }
};

int gomoku_board::get(int py,int px){
    return _board[py][px];
};

int gomoku_board::get(pair<int,int> pt){
    return _board[pt.first][pt.second];
};

int gomoku_board::Board_size(){
    return board_size;
};
//    protected:
//board is (board_size+2)*(board_size+2), where the edge colored by 10
// the value of board[i][j] is 1 if it's black, -1 if white
// vector< vector<int> > board;

bool gomoku_board::erase(int color, pair<int,int> position){
    int py = position.first, px = position.second;
    assert(_board[py][px] == color);
    if( min(px,py)<=0 || max(px,py)>board_size )
        return 0;
    else if(_board[py][px] != color)
        return 0;
    else{
        _board[py][px] = 0;
        return 1;
    }
};


bool gomoku_board::in_board(pair<int,int> pt){
    return in_board(pt.first, pt.second);
}
bool gomoku_board::in_board(int y,int x){
    return (y>0 && y<=this->board_size) &&\
        (x>0 && x<=this->board_size);
}

bool gomoku_board::is_valid_move(int y, int x){
    return in_board(y,x) && _board[y][x] == 0;
}

bool gomoku_board::is_valid_move(pair<int,int> pt){
    return is_valid_move(pt.first, pt.second);
}

void gomoku_board::reset_all(){
    for(int i=1; i<=board_size; i++){
        for(int j=1; j<=board_size; j++){
            _board[i][j] = 0;
        }
    }
}