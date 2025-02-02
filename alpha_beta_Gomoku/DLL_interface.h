//#pragma once
#ifndef INTERFACE_H
#define INTERFACE_H
#include"interface.h"
#endif

#ifndef DLL_EXPORTS
#define DLL_EXPORTS __declspec(dllexport)
#endif


namespace DLL_interface {
	extern "C" {

		DLL_EXPORTS Negamax_agent* get_negamax_agent(int n);
		DLL_EXPORTS void add_agent_stone(Negamax_agent* agent, int color, int py, int px);
		// void set_agent_stone(Negamax_agent* agent, int color, int py, int px);
		DLL_EXPORTS void find_opt_move_with_alpha_beta(Negamax_agent* agent, int color, int& rec_x, int& rec_y, int time_limit);
		DLL_EXPORTS void release_agent(Negamax_agent* agent);
		DLL_EXPORTS void print_board(Negamax_agent* agent);
	}
}