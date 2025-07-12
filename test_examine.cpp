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
    // cin>>n;`
    n = 2;
    while(n--)
        loader.next();
    // loader.load_problem("./bug_board.txt");
    cout<< loader.current_problem() <<endl;
    board.add_stone(1, {2,10});
    board.add_stone(-1, {3,9});
    board.add_stone(1, {3,11});
    board.add_stone(-1, {5,13});
    board.add_stone(1, {3,14});
    board.print_board();


    board_evaluator eval(&board);

    cout<<"valid defend:";

    board.add_stone(-1, {4,13});
    cout<<eval.is_valid_defend(-1, {4,13}, {3,14})<<endl;
    board.erase(-1, {4,13});
    
    cout<<"can defend: ";
    // board.add_stone(-1, {4,13});

    cout<<eval.can_defend(-1, 2, {3,14})<<endl;
    
    board.add_stone(-1, {4,13});
    cout<<"can attack: "<<eval.attack_to_win(1, 1)<<endl;
    auto atk = eval.get_victory_move(1, 1);
    cout<<"atk_move: "<<atk.first<<","<<atk.second<<endl;

    board.add_stone(1, {3,13});
    cout<<"defend of 2nd\n ";
    cout<<eval.can_defend(-1, 1, {3,13});
    // int color = 1;
    // while(1){
    //     pair<int,int> atk_pt;
    //     for(int i=1;i<=4;i++){
    //         cout<<"search depth: "<<i<<endl;
    //         eval.reset_info();
    //         atk_pt = eval.get_victory_move(color, i);
    //         if(board.in_board(atk_pt)){
    //             cout<<"atk pt:"<<atk_pt.first<<", "<<atk_pt.second<<endl;
    //             // eval.attack_to_win(1, i, true);
    //             break;
    //         }

    //     }
    //     board.add_stone(color, atk_pt);
    //     eval.print_info();
    //     board.print_board();
    //     cout<<"input your move:\n";        
    //     int y,x;cin>>y>>x;
    //     board.add_stone(-color, {y,x});
    //     board.print_board();
    // }
    board.print_board();
}