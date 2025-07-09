#include"./H/gomoku_Negamax.h"
int main(){
    int n= 15;
    gomoku_board board(n);
    Negamax_agent agent(n, &board, true);
    int human_color = 1;
    int current_color = 1;
    while( !agent.evaluator->is_win(1) && !agent.evaluator->is_win(-1) ){
        int y,x;
        if(current_color == human_color){
            cout<<"input point: ";
            cin>>y>>x;
        }
        else{
            cout<<"computer is thinking...\n";
            y = -1;
            x = -1;
            bool res = agent.get_opt_move_with_fixed_depth(current_color, y,x , 1100000, 2, 0);
            cout<<"thinking solution: "<<res<<"\n";
            agent.print_info();
            agent.reset_record();
        }
        cout<<"color "<<current_color<<" move at ("<<y<<","<<x<<")\n";
        board.add_stone(current_color, {y,x});
        current_color = -current_color;
        board.print_board();
    }
}