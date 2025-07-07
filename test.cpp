#include"./H/gomoku_Negamax.h"
#include"board_loader.cpp"
using namespace std;
int main(){
    gomoku_board board(15);

    string problems = ".\\board\\russian";
    board_loader loader(problems, &board);
    int n = 1;
    while(n--)
        loader.next();
    // loader.load_problem("./question_board2.txt");
    cout<< loader.current_problem() <<endl;
    board.print_board();

    board_evaluator eval(&board);

    vector<pair<int,int>> test;
    pair<int,int> atk_pt;

    int color = 1;
    // board.add_stone(-1, {5,9});
    // cout<<"can defend:\n";
    // cout<<eval.can_defend(1, 1, {5,9});
    // board.add_stone(1, {7,7});
    // cout<<eval.match_attack(-1, {5,9}, board_evaluator::STATE::two_step, 1);
    // board.add_stone(1, {10, 9});
    // board.add_stone(-1, {11,10});
    // board.add_stone(1, {9,10});
    // board.add_stone(-1, {7,9});
    while(1){
        for(int i=1;i<=4;i++){
            cout<<"search depth: "<<i<<endl;
            atk_pt = eval.get_victory_move(color, i);
            if(board.in_board(atk_pt)){
                cout<<"atk pt:"<<atk_pt.first<<", "<<atk_pt.second<<endl;
                
                cout<< eval.attack_to_win(color, i, true)<<endl;
                break;
            }
        }
        board.add_stone(color, atk_pt);
        board.print_board();
        cout<<"input your point:\n";
        int y,x;cin>>y>>x;
        board.add_stone(-color, {y,x});    
        board.print_board();
    }
    board.print_board();
}