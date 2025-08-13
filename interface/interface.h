#ifndef gomoku_Negamax_H
#define gomoku_Negamax_H
#include"gomoku_Negamax.h"
#endif

namespace agent_interface{
    Negamax_agent* get_negamax_agent(int n);
    void add_agent_stone(Negamax_agent* agent, int color, int py, int px);
    // void set_agent_stone(Negamax_agent* agent, int color, int py, int px);
    void find_opt_move_with_alpha_beta(Negamax_agent* agent,int color,int& rec_x,int& rec_y);
    void release_agent(Negamax_agent*);
}