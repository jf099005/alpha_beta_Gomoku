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
    agent.move(-1, {6,5});
    agent.print_board();
    int nx,ny;
    int depth = agent.get_opt_move_with_fixed_depth(1, ny,nx, 1000, 4);
    cout<<nx<<","<<nx<<endl;
    cout<<"depth:"<<depth<<endl;
    // vector< pair<int,int> > candidate_pt;
    // bool is_cut = agent.Gomoku_knowledge_cut(1, candidate_pt);
    // if(is_cut){
    //     cout<<"cut occur\n";
    //     cout<<candidate_pt[0].first<<","<<candidate_pt[0].second<<endl;
    // }
    // else{
    //     cout<<"cut does not occur\n";
    // }
}