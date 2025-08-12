#include"./H/gomoku_Negamax.h"
#include"board_loader.cpp"
#include<ctime>
using namespace std;
int main(){
    gomoku_board board(15);

    string problems = "./board/Nakamura/";
    board_loader loader(problems, &board);
    int n;
    cout<<"input problem id:";
    // cin>>n;
    n= 2;
    while(n--)
        loader.next();

    cout<< loader.current_problem() <<endl;

    // board_evaluator eval(&board);

    Negamax_agent agent(15, &board, true);


    // board.add_stone(1, {12, 8});
    board.print_board();


    int color = 1;
    int current_color = 1;
    int depth = 8;

    // board.add_stone(-1, {9,4});
    // board.add_stone(1, {9,6});
    // board.add_stone(-1, {5,7});
    // board.add_stone(1, {6,7});

    // board.add_stone(-1 ,{11,10});
    // board.add_stone(1, {12,11});
    // cout<<"is valid defend: "<<agent.evaluator->is_valid_defend(1, {9,6}, {9,4})<<endl;
    // board.erase(1, {9,6});
    // cout<<"input search depth:\n";
    // cin>>depth;
    vector<pair<int,int>> test(depth);
    // board.add_stone(1, {8,12});
    // board.add_stone(-1, {7,12});
    // board.add_stone(1, {9,11});
    // board.add_stone(-1, {7,11});
    // cout<<"valid defend: "<<agent.evaluator->is_valid_defend(-1, {7,11}, {9,11})<<endl;
    // cout<<agent.evaluator->is_win(color)<<"/"<<agent.evaluator->is_win(-color)<<endl;

    cout<< agent.Negamax(color, depth, 0, -MAX_BOARD_SCORE, MAX_BOARD_SCORE, {0,0}, test, time(0), 100000000,
             true, 0, 1, 0);
    cout<<"/"<<MAX_BOARD_SCORE<<endl;
    for(auto v:test){
        cout<<v.first<<","<<v.second<<endl;
    }
    agent.print_info();
    board.print_board();
    agent.print_path(test, 1);
}