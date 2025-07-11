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
    cin>>n;
    while(n--)
        loader.next();
    // loader.load_problem("./bug_board.txt");
    cout<< loader.current_problem() <<endl;
    board.print_board();

    board_evaluator eval(&board);

    int color = 1;
    while(1){
        pair<int,int> atk_pt;
        for(int i=1;i<=4;i++){
            cout<<"search depth: "<<i<<endl;
            eval.reset_info();
            atk_pt = eval.get_victory_move(color, i);
            if(board.in_board(atk_pt)){
                cout<<"atk pt:"<<atk_pt.first<<", "<<atk_pt.second<<endl;
                // eval.attack_to_win(1, i, true);
                break;
            }

        }
        board.add_stone(color, atk_pt);
        eval.print_info();
        board.print_board();
        cout<<"input your move:\n";        
        int y,x;cin>>y>>x;
        board.add_stone(-color, {y,x});
        board.print_board();
    }
    board.print_board();
}