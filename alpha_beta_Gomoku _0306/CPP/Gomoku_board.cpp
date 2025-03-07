#include<iostream>
#include<iomanip>
#ifndef vector
#include<vector>
#endif

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
        void gomoku_game::print_board(){
            cout<<"==========\n";
            cout<<setw(3)<<"\\";
            for(int i=0;i<=board_size;i++)cout<<setw(3)<<i;
            cout<<endl;
            for(int i=0; i<=board_size+1; i++){
                cout<<setw(3)<<i;
                for(int j=0; j<=board_size+1; j++){
                    cout<<setw(3)<<( board[i][j]==0?'.': (board[i][j]==1? 'O':board[i][j] == -1?'X':'?') );
                }
                cout<<"\n";
            }
            cout<<"==========\n";

        };
        gomoku_game::gomoku_game(int n):
            board_size(n), current_color(0)
        {
            for(int i=0;i<=n+1;i++){
                for(int j=0;j<=n+1;j++)
                    board[i][j] = 0;
            }
            for(int i=0;i<=n+1;i++){
                board[i][0] = 10;
                board[0][i] = 10;
                board[n+1][i] = 10;
                board[i][n+1] = 10;
            }
        };

        //return 1 if the step is legal
        void gomoku_game::move(int color, pair<int,int> position){
            int py = position.first, px = position.second;
            if( min(px,py)<=0 || max(px,py)>board_size )
                return;
            else if(board[py][px] != 0)
                return;
            else{
                board[py][px] = color;
                return;
            }
        };

        inline int gomoku_game::get_board(int py,int px){
            return board[py][px];
        };
        inline int gomoku_game::get_board(pair<int,int> pt){
            return board[pt.first][pt.second];
        };

        inline int gomoku_game::get_board_size(){
            return board_size;
        };
//    protected:
        //board is (board_size+2)*(board_size+2), where the edge colored by 10
        // the value of board[i][j] is 1 if it's black, -1 if white
        // vector< vector<int> > board;
        
        bool gomoku_game::remove(int color, pair<int,int> position){
            int py = position.first, px = position.second;
            if( min(px,py)<=0 || max(px,py)>board_size )
                return 0;
            else if(board[py][px] != color)
                return 0;
            else{
                board[py][px] = 0;
                return 1;
            }
        };

        bool gomoku_game::out_of_bound(int y,int x){
            return min(y,x)<=0 or max(y,x)>(this->board_size);
        }

        void gomoku_game::reset(pair<int, int> position) {
            int py = position.first, px = position.second;
            if (min(px, py) <= 0 || max(px, py) > board_size)
                return;
            board[py][px] = 0;
        }