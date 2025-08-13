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
    while(n--)
        loader.next();
    // loader.load_problem("./bug_board.txt");
    cout<< loader.current_problem() <<endl;
    board.print_board();

    // board_evaluator eval(&board);
    Negamax_agent agent(15, &board);

    int color = 1;
    while(1){
        pair<int,int> atk_pt;
        for(int i=1;i<=4;i++){
            cout<<"search depth: "<<i<<endl;
            agent.reset_record();
            vector< pair<int,int> > test(i*2);
            int score = agent.Negamax(color, i*2, 0, -MAX_BOARD_SCORE, MAX_BOARD_SCORE, {-1, -1}, test, 0, 1, 0);
            // atk_pt = eval.get_victory_move(color, i);
            atk_pt = test[0];
            if(score == MAX_BOARD_SCORE){
                cout<<"atk pt:"<<atk_pt.first<<", "<<atk_pt.second<<endl;
                // eval.attack_to_win(1, i, true);
                cout<<"score: "<<score<<endl;
                break;
            }
            else{
                cout<<"can't find solution\n";
            }

        }
        board.add_stone(color, atk_pt);
        agent.print_info();
        board.print_board();
        cout<<"input your move:\n";        
        int y,x;cin>>y>>x;
        board.add_stone(-color, {y,x});
        board.print_board();
    }
    board.print_board();
}