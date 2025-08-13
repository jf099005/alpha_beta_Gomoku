#pragma once
#include<fstream>
#include"Gomoku_board.h"
#ifndef MAX_BOARD_SCORE
#define MAX_BOARD_SCORE (1e9+7)
#endif



#ifndef TLE_SCORE
#define TLE_SCORE (2e9+123456)
#endif

#ifndef ATTACK
#define ATTACK 1
#endif

#ifndef NOT_ATTACK
#define NOT_ATTACK -1
#endif


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
        Negamax_agent(int n, gomoku_board* board,  string lp, bool fix_search_sequence = 0);
        ~Negamax_agent();
        void print_path(vector< pair<int,int> > path_rec, int color=0);
        // get next step by calling Negamax with IDS algorithm
        // calculate the optimal move and record in the reference (y,x), return the maximal depth 
        int get_opt_move(int color, int& rec_y, int& rec_x, int limit_time = 1000, int limit_depth=10);
        bool get_opt_move_with_fixed_depth(int color, int& rec_y, int& rec_x, int limit_time, int depth, int attack_depth);
        void open_log_file();
        void close_log_file();


        int Negamax(int color, int depth, int attack_depth, int alpha, int beta, pair<int,int> prv_move, vector< pair<int,int> > &opt_path_rec,\
               bool under_attack = false, int self_strategy = 0, int opponent_strategy = 0);

        map<int, pair<int,int> > transposition_table;
        
        int* visited_node_num;
        void print_info();
        void reset_record();

        pair<int,int>* visit_seq;

        bool time_restrict = 0;

        int search_start_time;
        int search_end_time;
        // bool use_gomoku_cut;

        bool log_recursion;
        string log_path;
        ofstream log_file;


};