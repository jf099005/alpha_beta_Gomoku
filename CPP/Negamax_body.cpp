#include"../H/gomoku_Negamax.h"

const int default_attack_check_depth = 2;

Negamax_agent::Negamax_agent(int n, gomoku_board* board, bool fix_search_sequence):
    Board(board), attack_check_depth(default_attack_check_depth)
{
    //srand(time(0));
    srand(0);
    // this->Board = board;
    this->visit_seq = new pair<int,int>[board->Board_size() * board->Board_size()];


    bool flag[MAX_BOARD_SIZE+1][MAX_BOARD_SIZE +1];
    for(int i=0;i<=n;i++){
        for(int j=0;j<=n;j++)
            flag[i][j] = 0;
    }
    flag[0][0] = 1;
    for(int i=0;i<n*n;i++){
        int px=i/n, py=i%n;
        if(!fix_search_sequence){
            do{
                px = rand()%n+1, py = rand()%n+1;
            }
            while(flag[py][px]);
        }
        visit_seq[i] = {py, px};
        flag[py][px] = 1;
    }
    
    this->evaluator = new board_evaluator(Board, visit_seq);
};

void Negamax_agent::print_path(vector< pair<int,int> > path_rec, int color){
    if(color)cout<<"current player is "<<(color==1?'O':'X')<<endl;
    else cout<<"color is 0...?\n";
    cout<<"path length:"<<path_rec.size()<<endl;
    for(auto it:path_rec)cout<<"("<<it.first<<","<<it.second<<"), ";
    cout<<endl;
    for(int i=1; i<= this->Board->board_size; i++){
        for(int j=1;j<=this->Board->board_size;j++){
            bool in_path=0;
            for(int k=0;k<path_rec.size();k++){
                if( path_rec[k] == pair<int,int>(i,j) ){
                    in_path = 1;
                    cout<<setw(3)<<k;
                };
            }
            if(!in_path){
                // Board->print_board();
                cout<<setw(3)<<( Board->get(i,j)==0?'.': (Board->get(i,j)==1? 'O':'X') );
            }
        }
        cout<<endl;
    }
}

int Negamax_agent::Negamax(int color, int depth, int alpha, int beta, vector< pair<int,int> > &opt_path_rec, int start_time=-1, int time_limit=1e9, bool use_gomoku_cut){
    if(depth<=0){
        return color*(evaluator->board_score());
    }

    if(evaluator->is_win(color))return 1e9+7;
    else if(evaluator->is_win(-color))return -(1e9+7);
    int opt_score = -1e9;
    vector< pair<int,int> > path_rec = opt_path_rec;
    int path_len = path_rec.size();
///*
    if(use_gomoku_cut){
        vector< pair<int,int> > candidate_pts;
        // bool cut_occur = evaluator->Gomoku_knowledge_cut(color, candidate_pts);
        pair<int,int> atk_pt = evaluator->attack_to_win(color, attack_check_depth);
        pair<int,int> def_pt = evaluator->attack_to_win(-color, attack_check_depth);

        bool cut_occur = Board->in_board(atk_pt) || Board->in_board(def_pt);

        if(cut_occur){
            pair<int,int> opt_pt = {-1, -1};
            for(auto cut_pt: candidate_pts){
                path_rec[path_rec.size()-depth] = cut_pt;
                Board->add_stone(color, cut_pt);
                int score = - Negamax( -color, depth-1, -beta, -opt_score, path_rec, start_time, time_limit, use_gomoku_cut);
                Board->erase(color, cut_pt);
                if(score>opt_score){
                    //if(score>=beta)return score;
                    opt_score = score;
                    opt_path_rec = path_rec;
                    opt_pt = cut_pt;
                }
            }
            // recorder.record_minimax_solution(*this, color, depth, opt_score);
            return opt_score;
        }
    }
    //*/

    if(depth<=0){
        return color*evaluator->board_score();
    }
    pair<int,int>opt_pt(0,0);
    for(int i=0;i<Board->board_size*Board->board_size;i++){
        if(time(0) - start_time > time_limit)
            return -2e9;

        pair<int,int> visit_pt = visit_seq[i];
        
        if( Board->get(visit_pt)!=0 )continue;

        path_rec[path_rec.size()-depth] = visit_pt;
        Board->add_stone(color, visit_pt);
        int score = - Negamax( -color, depth-1, -beta, -opt_score, path_rec, start_time, time_limit, use_gomoku_cut);
        Board->erase(color, visit_pt);
        
        if(time(0) - start_time > time_limit){
            return -2e9;
        }
		
        if(score > opt_score){
            if( score>= beta )return score;
            opt_score = score;
            opt_path_rec = path_rec;
            opt_pt = visit_pt;
        }
    }
    return opt_score;
}

int Negamax_agent::get_opt_move(int color, int& rec_y, int& rec_x, int limit_time, int limit_depth) {
    for (int d = 2;d < limit_depth;d += 2) {
        bool search_success = get_opt_move_with_fixed_depth(color, rec_y, rec_x, limit_time, d);
        if (!search_success) {
            return d-2;
        }
    }
    return limit_depth;
}

bool Negamax_agent::get_opt_move_with_fixed_depth(int color, int& rec_y, int& rec_x, int limit_time, int depth) {
    vector< pair<int,int> > opt_path(depth);
    int opt_score = Negamax(color, depth, -1e9, 1e9, opt_path, time(0), limit_time, true);
    pair<int,int> opt_solution = opt_path[0];
		//cout<<"sucessfully calling Negamax function\n";
    if (opt_score <= -2e9 || opt_solution.first == 0) {
        return false;
    }
    for(int i=0;i<opt_path.size();i++){
        for(int j=0;j<Board->board_size*Board->board_size;j++){
        if( opt_path[i] == visit_seq[j] ){
            for(int k=j;k>i;k--)
                swap( visit_seq[k], visit_seq[k-1] );
                break;
            }
        }
    }
    rec_y = opt_solution.first;
    rec_x = opt_solution.second;
    return true;
}