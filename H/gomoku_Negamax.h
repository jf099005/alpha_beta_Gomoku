#pragma once
#include"Gomoku_board.h"

// #ifndef Memorize
// #define Memorize
// #include"memorize_search.h"
// #endif

// #ifndef BOARD_EVALUATOR
// #define BOARD_EVALUATOR
#include"board_evaluator.h"
// #endif

#include<iostream>
#include <chrono>
#include<algorithm>
#include<ctime>
#include<map>

using namespace std;
class Negamax_agent{
    public:
        gomoku_board *Board;
        board_evaluator *evaluator;
        Negamax_agent(int n, gomoku_board* board,  bool fix_search_sequence = 0);
        ~Negamax_agent();
        void print_path(vector< pair<int,int> > path_rec, int color=0);
        // get next step by calling Negamax with IDS algorithm
        // calculate the optimal move and record in the reference (y,x), return the maximal depth 
        int get_opt_move(int color, int& rec_y, int& rec_x, int limit_time = 1000, int limit_depth=10);
        bool get_opt_move_with_fixed_depth(int color, int& rec_y, int& rec_x, int limit_time, int depth, int attack_depth);
        pair<int,int>* visit_seq;
        vector< pair<int,int> > current_opt_path;

        int Negamax(int color,int depth, int attack_depth, int alpha, int beta, vector< pair<int,int> > &opt_path_rec, int start_time, int time_limit, bool use_gomoku_cut = false);                
        map<int, pair<int,int> > transposition_table;

        int attack_check_depth;
        
        int* visited_node_num;
        void print_info();
        void reset_record();
};