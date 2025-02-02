#ifndef GOMOKU
#define GOMOKU
#include"Gomoku_board.h"
#endif

// #ifndef Memorize
// #define Memorize
// #include"memorize_search.h"
// #endif

#ifndef iostream
#include<iostream>
#endif

#ifndef chrono
#include <chrono>
#endif

#ifndef algorithm
#include<algorithm>
#endif

#ifndef time
#include<ctime>
#endif
#include<map>

using namespace std;
class Negamax_agent: public gomoku_game{
    public:
        Negamax_agent(int n, bool fix_search_sequence = 0);

        inline void print_path(vector< pair<int,int> > path_rec, int color=0);
        // get next step by calling Negamax with IDS algorithm
        pair<int,int> get_opt_move(int color, int expect_time = 1000, bool show_detail=false);

        bool is_win(int color);

        int num_direction = 4;
        int direction[4][2] = { {1,0},{0,1},{1,1},{1,-1}};
        int score_table[3][6] = {
            {0,20,100,500,5000,100000},
			{0,0,20,100,500,100000},
			{0,0,0,0,0,100000}
        };
        
        //return the maximal length of the position at board[py][px] with direction[dir]
        inline int get_length_with_dir(int py, int px, int dir, bool reverse);
        inline int calculate_score(int color);
        inline int board_score();//score(Black) - score(White)

        pair<int,int> visit_seq[20*20];
        vector< pair<int,int> > current_opt_path;

        int Negamax(int color,int depth, int alpha, int beta, vector< pair<int,int> > &opt_path_rec, int start_time, int time_limit, bool use_gomoku_cut = false);        
        inline int detect_5(int color, pair<int,int> pt);// return 50 if player[color] can win when ze put stone on pt
        inline int detect_4(int color, pair<int,int> pt);// return 0 if there's no 4, -4 if there's only a dead 4, 4 if there's a live 4 when player put stone on pt
        inline bool Gomoku_knowledge_cut(int color, vector< pair<int,int> >& candidate_pts);

        map<int, pair<int,int> > transposition_table;
};