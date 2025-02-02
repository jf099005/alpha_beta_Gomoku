#ifndef DLL_INTERFACE_H
#define DLL_INTERFACE_H
#include"DLL_interface.h"
#endif

#ifndef INTERFACE_H
#define INTERFACE_H
#include"interface.h"
#endif

Negamax_agent* DLL_interface::get_negamax_agent(int n) {
	Negamax_agent* agent = agent_interface::get_negamax_agent(n);
	return agent;
}

void DLL_interface::add_agent_stone(Negamax_agent* agent, int color, int py, int px) {
	agent_interface::add_agent_stone(agent, color, py, px);
}

void DLL_interface::find_opt_move_with_alpha_beta(Negamax_agent* agent, int color, int& rec_x, int& rec_y) {
	agent_interface::find_opt_move_with_alpha_beta(agent, color, rec_x, rec_y);
}

void DLL_interface::release_agent(Negamax_agent* agent) {
	agent_interface::release_agent(agent);
}