#pragma once
#include"Gomoku_board.h"
#include<map>
class board_evaluator{
    public:
        gomoku_board *Board;
        pair<int,int>* visit_seq;

        enum class STATE{
            one_step_draw = 5,
            one_step = 4,
            two_step = 3
        };

        //return the maximal length of the position at board[py][px] with direction[dir]
        
        board_evaluator(gomoku_board* board, pair<int,int>* visit_seq = NULL);
        ~board_evaluator();
        
        int get_length_with_dir(int py, int px, int dir, bool reverse);
        int calculate_score(int color);
        int board_score();//score(Black) - score(White)
        bool shape_compare(int color, string shape, pair<int,int> pt, int dy, int dx);
        bool shape_match(int color, string shape, pair<int,int> pt, int dy, int dx);
        
        int detect_5(int color, pair<int,int> pt);// return 50 if player[color] can win when ze put stone on pt
        int detect_4(int color, pair<int,int> pt);// return 0 if there's no 4, -4 if there's only a dead 4, 4 if there's a live 4 when player put stone on pt
        bool match_attack(int color, pair<int,int> pt, STATE attack_type, bool show_detail);// return 0 if there's no 4, -4 if there's only a dead 4, 4 if there's a live 4 when player put stone on pt
        
        bool is_win(int color);
        bool attack_to_win(int attacker, int depth, bool show_detail = false);
        bool can_defend(int defender, int depth, pair<int,int> atk_pt);
        //only when attack_to_win is true
        pair<int,int> get_victory_move(int attacker, int depth);
        

        int* attack_to_win_calls;
        int* can_defend_calls;
        void reset_info();
        void print_info();

    private:
        static const int num_direction;
        static const int direction[4][2];
        

        static const vector<string> one_step_draw_shape;
        static const vector<string> one_step_shape;
        static const vector<string> two_step_shape;

        static const map<string, vector<int> > defend_positions;

        static const int score_table[3][6];

        map<STATE, vector<string> >state_map;
};