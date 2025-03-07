#include"./H/gomoku_Negamax.h"
using namespace std;
int main(){
    Negamax_agent agent(15);
    // agent.move(1, {1,1});
    // agent.move(1, {1,2});
    // agent.move(1, {1,3});
    agent.move(1, {3,5});
    agent.move(1, {4,5});
    // agent.move(1, {6,5});
    agent.move(1, {3,4});
    agent.move(1, {4,3});
    // agent.move(1, {5,2});
    // agent.move(-1, {1, 5});
    agent.move(-1, {7,5});
    // agent.move(-1, {1,6});
    int nx,ny;

    agent.move(1, {1,10});
    agent.move(-1, {1, 11});
    agent.move(-1, {1, 12});
    agent.move(-1, {1, 13});
    // agent.move(-1, {1,15});
    
    agent.print_board();
    int depth = agent.get_opt_move_with_fixed_depth(1, ny,nx, 1000, 4);
    cout<<nx<<","<<nx<<endl;
    cout<<"depth:"<<depth<<endl;
    // vector< pair<int,int> > candidate_pt;
    // // agent.move(1, {1,5});
    // agent.print_board();

    // cout<<agent.shape_cmp(1, "OOO.O", {1,5}, 0, 1);
    // cout<<agent.shape_cmp(1, "OOO.O", {1,5}, 1, 0);
    // cout<<agent.shape_cmp(1, "O.OOO", {1,5}, 0, 1);
    // cout<<agent.shape_cmp(1, "O.OOO", {1,5}, 1, 0);

    // bool is_cut = agent.Termination_state_cut(1, candidate_pt);
    // if(is_cut){
    //     cout<<"cut occur\n";
    //     cout<<candidate_pt[0].first<<","<<candidate_pt[0].second<<endl;
    // }
    // else{
    //     cout<<"cut does not occur\n";
    // }
    // agent.move(1, {1,5});
    // cout<<agent.detect_4(1, {1,5}, 0);
    // cout<<agent.detect_4(1, {1,5}, 1);
    // cout<<agent.detect_4(1, {1,5}, 2);
    // cout<<agent.detect_4(1, {1,5}, 3);
}