#include"./H/gomoku_Negamax.h"
using namespace std;
int main(){
    Negamax_agent agent(15);
    agent.move(1, {1,1});
    agent.move(1, {1,2});
    agent.move(1, {1,3});
    agent.move(1, {3,5});
    agent.move(1, {4,5});
    agent.move(1, {5,5});
    int nx,ny;
    // agent.print_board();
    // int depth = agent.get_opt_move(1, ny,nx);
    // cout<<nx<<","<<ny<<endl;
    // cout<<"depth: "<<depth<<endl;
    agent.move(1, {1, 5});
    cout<<agent.detect_4(1, {1,5});
}