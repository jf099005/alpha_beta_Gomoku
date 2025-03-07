#include"DLL_interface.h"

Negamax_agent* DLL_interface::get_negamax_agent(int n){
    //Negamax_agent* agent = (Negamax_agent*)(malloc(sizeof(Negamax_agent)));
    //*agent = Negamax_agent(n);
    Negamax_agent* agent = new Negamax_agent(n);
    return agent;
}

void DLL_interface::add_agent_stone(Negamax_agent* agent, int color, int py,int px){
    agent->move(color, {py,px});
}

int DLL_interface::find_opt_move_with_alpha_beta(Negamax_agent* agent, int color, int& rec_y, int& rec_x, int time_limit){
    //pair<int,int> opt = agent->get_opt_move(color);
    int ret_depth = agent->get_opt_move(color, rec_y, rec_x, time_limit);
    return ret_depth;
}

void DLL_interface::release_agent(Negamax_agent* agent){
    free(agent);
}

void DLL_interface::print_board(Negamax_agent* agent) {
	agent->print_board();
}