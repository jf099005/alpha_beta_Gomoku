#include"../H/gomoku_Negamax.h"

Negamax_agent::Negamax_agent(int n, bool fix_search_sequence):
                gomoku_game(n)
        {
            //srand(time(0));
            srand(0);
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
            
        };

void Negamax_agent::print_path(vector< pair<int,int> > path_rec, int color){
            if(color)cout<<"current player is "<<(color==1?'O':'X')<<endl;
            else cout<<"color is 0...?\n";
            cout<<"path length:"<<path_rec.size()<<endl;
            for(auto it:path_rec)cout<<"("<<it.first<<","<<it.second<<"), ";
            cout<<endl;
            for(int i=1;i<=board_size;i++){
                for(int j=1;j<=board_size;j++){
                    bool in_path=0;
                    for(int k=0;k<path_rec.size();k++){
                        if( path_rec[k] == pair<int,int>(i,j) ){
                            in_path = 1;
                            cout<<setw(3)<<k;
                        };
                    }
                    if(!in_path){
                        cout<<setw(3)<<( board[i][j]==0?'.': (board[i][j]==1? 'O':'X') );
                    }
                }
                cout<<endl;
            }
}

bool Negamax_agent::is_win(int color){
            for(int py = 0;py<=board_size;py++){
                for(int px=0;px<=board_size;px++){
                    for(int dir=0;dir<num_direction;dir++){
                        if(board[py][px]!=color)continue;
                        if( min( py-direction[dir][0],  px-direction[dir][1])>0 && 
                                max( py-direction[dir][0],  px-direction[dir][1])<= board_size &&
                                    board[ py-direction[dir][0] ][ px-direction[dir][1] ] == color ){
                                        continue;
                        }
                        if( get_length_with_dir(py, px, dir, false) ==5)return 1;
                    }
                }
            }
            return 0;
}


int Negamax_agent:: get_length_with_dir(int py, int px, int dir, bool reverse = 0){
    if(min(py,px)<=0 || max(py,px)> board_size || board[py][px]==0 || dir<0 || dir>=num_direction){
        return 0;
    };
    int len = 1;
    int dy = direction[dir][0], dx = direction[dir][1];
    if(reverse){
        dx=-dx; dy=-dy;
    }
    while( board[ py+len*dy ][ px+len*dx ] == board[py][px] ){
        len++;
        if(len>5){
            cout<<"DETECTED LENGTH >= 5 WHEN CALLING get_length_with_dir==============\n";
            return len;
        }
    }
    return len;
}

int Negamax_agent::board_score(){
    return calculate_score(1) - calculate_score(-1);
}

int Negamax_agent::Negamax(int color, int depth, int alpha, int beta, vector< pair<int,int> > &opt_path_rec, int start_time=-1, int time_limit=1e9, bool use_gomoku_cut){
    if(depth<=0){
        return color*board_score();
    }

    if(is_win(color))return 1e9+7;
    else if(is_win(-color))return -(1e9+7);
    int opt_score = -1e9;
    vector< pair<int,int> > path_rec = opt_path_rec;
    int path_len = path_rec.size();
///*
    if(use_gomoku_cut){
        vector< pair<int,int> > candidate_pts;
        bool cut_occur = Gomoku_knowledge_cut(color, candidate_pts);
        if(cut_occur){
            pair<int,int> opt_pt = {-1, -1};
            for(auto cut_pt: candidate_pts){
                path_rec[path_rec.size()-depth] = cut_pt;
                move(color, cut_pt);
                int score = - Negamax( -color, depth-1, -beta, -opt_score, path_rec, start_time, time_limit, use_gomoku_cut);
                remove(color, cut_pt);
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
        return color*board_score();
    }
    pair<int,int>opt_pt(0,0);
    for(int i=0;i<board_size*board_size;i++){
        if(time(0) - start_time > time_limit)
            return -2e9;

        pair<int,int> visit_pt = visit_seq[i];
        
        if( board[visit_pt.first][visit_pt.second]!=0 )continue;

        path_rec[path_rec.size()-depth] = visit_pt;
        move(color, visit_pt);
        int score = - Negamax( -color, depth-1, -beta, -opt_score, path_rec, start_time, time_limit, use_gomoku_cut);
        remove(color, visit_pt);
        
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
            //if (d == 2) {/*
            //    rec_y = 1;
            //    rec_x = 1;
            //}*/
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
        for(int j=0;j<board_size*board_size;j++){
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