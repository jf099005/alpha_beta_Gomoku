#include"./H/gomoku_Negamax.h"
#include"board_loader.cpp"
#include<ctime>
using namespace std;
int main(){
    gomoku_board board(15);

    string problems = "./board/Nakamura/";
    board_loader loader(problems, &board);
    int n = 1;
    while(n--)
        loader.next();
    loader.load_problem("./bug_board.txt");
    cout<< loader.current_problem() <<endl;
    board.print_board();

    board_evaluator eval(&board);

    Negamax_agent agent(15, &board, true);
    int depth = 2, attack_depth = 0;
    vector<pair<int,int>> test(depth);
    int current_color = -1;
    int y=-1, x=-1;
    // board.add_stone(-1, {8,4});
    // board.add_stone(1, {8,7});
    // cout<<eval.attack_to_win(-1, 4, false);
    // cout<<eval.attack_to_win(1, 4);
    // cout<<agent.Negamax(current_color, 0, 0, -1e9, 1e9, test, time(0), 1100000, true)<<endl;
    // cout<<"test with pure negamax\n";

    // cout<<agent.evaluator->attack_to_win(1, 2)<<"/"<<agent.evaluator->attack_to_win(-1, 2)<<endl;
    // cout<<"test of atk-to-win ended\n";
    current_color = -1;
    depth = 2;
    // board.add_stone(-1, {8,4});


    vector< pair<int,int> > opt_path(depth + attack_depth);
    bool res = agent.get_opt_move_with_fixed_depth(current_color, y,x , 1100000, 2,  0);
    

    cout<<"test of get_opt_move_with_fixed_depth ended\n";
    // bool res = agent.get_opt_move_with_fixed_depth(current_color, y,x , 1100000, 2,  0);
    // cout<<"thinking solution: "<<res<<"\n";

    cout<<y<<","<<x<<endl;
    // cout<<test[0].first<<","<<test[0].second<<"\n";
    // pair<int,int> atk_pt;

    // int color = -1;
    // board.add_stone(-1, {5,7});
    // board.add_stone(1, {6, 7});
    // board.add_stone(1, {10, 9});
    // board.add_stone(-1, {11,10});
    // board.add_stone(1, {9,10});
    // board.add_stone(-1, {7,9});
    // for(int y=1;y<=15;y++){
    //     for(int x=1;x<=15;x++){
    //         board.add_stone(-1, )
    //         for(int i=1;i<=4;i++){
    //                 cout<<"search depth: "<<i<<endl;
    //                 atk_pt = eval.get_victory_move(color, i);
    //                 if(board.in_board(atk_pt)){
    //                     cout<<"atk pt:"<<atk_pt.first<<", "<<atk_pt.second<<endl;
    //                     // eval.attack_to_win(1, i, true);
    //                     break;
    //                 }
    //             }
    //         }
    // }
    board.print_board();
}