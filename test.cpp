#include"./H/gomoku_Negamax.h"
#include"board_loader.cpp"
#include<ctime>
using namespace std;
int main(){
    gomoku_board board(15);

    string problems = "./board/russian/";
    board_loader loader(problems, &board);
    int n;
    cout<<"input problem id:";
    cin>>n;
    // n= 2;
    while(n--)
        loader.next();

    cout<< loader.current_problem() <<endl;

    // board_evaluator eval(&board);

    Negamax_agent agent(15, &board,"./log.txt" ,  true);


    // board.add_stone(1, {12, 8});
    board.print_board();


    int color = 1;
    int depth;
    cout<<"depth: ";
    cin>>depth;

    int atk_depth;
    cout<<"attack depth:";
    cin>>atk_depth;

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
    vector<pair<int,int>> test(depth + atk_depth, {0,0});
    // board.add_stone(1, {8,12});
    // board.add_stone(-1, {7,12});
    // board.add_stone(1, {9,11});
    // board.add_stone(-1, {7,11});
    // cout<<"valid defend: "<<agent.evaluator->is_valid_defend(-1, {7,11}, {9,11})<<endl;
    // cout<<agent.evaluator->is_win(color)<<"/"<<agent.evaluator->is_win(-color)<<endl;
    
    // agent.visit_seq[0] = {6,10};
    agent.log_recursion = 1;
    cout<< agent.Negamax(color, depth, atk_depth, -MAX_BOARD_SCORE, MAX_BOARD_SCORE, {0,0}, test, 0, 0, 0);
    cout<<"/"<<MAX_BOARD_SCORE<<endl;
    for(auto v:test){
        cout<<v.first<<","<<v.second<<endl;
    }

    // agent.visit_seq[0] = {6,10};

    agent.print_info();
    // board.print_board();
    agent.print_path(test, 1);
    // board.add_stone(1, {6,10});
    cout<<"6,10 atk:"<< (agent.evaluator->is_valid_attack(1, {6,10}) != board_evaluator::STATE::none)<<endl;
}