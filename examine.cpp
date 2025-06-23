#include"./H/gomoku_Negamax.h"
#include"board_loader.cpp"
using namespace std;
int main(){
    gomoku_board board(15);

    string problems = "./board/Nakamura/";
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

    int color = -1;
    // board.add_stone(1, {8,11});
    // board.add_stone(-1, {8, 10});
    // board.add_stone(1, {10, 9});
    // board.add_stone(-1, {11,10});
    // board.add_stone(1, {9,10});
    // board.add_stone(-1, {7,9});

    for(int i=1;i<=4;i++){
        cout<<"search depth: "<<i<<endl;
        atk_pt = eval.attack_to_win(color, i);
        if(board.in_board(atk_pt)){
            cout<<"atk pt:"<<atk_pt.first<<", "<<atk_pt.second<<endl;
            // eval.attack_to_win(1, i, true);
            break;
        }
    }
    board.print_board();
}