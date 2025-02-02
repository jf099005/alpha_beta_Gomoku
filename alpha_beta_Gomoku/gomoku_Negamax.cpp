#include"gomoku_Negamax.h"

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

        inline void Negamax_agent::print_path(vector< pair<int,int> > path_rec, int color){
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
                        if( min( py-direction[dir][0],  px-direction[dir][1])>0 and 
                                max( py-direction[dir][0],  px-direction[dir][1])<= board_size and
                                    board[ py-direction[dir][0] ][ px-direction[dir][1] ] == color ){
                                        continue;
                        }
                        if( get_length_with_dir(py, px, dir, false) ==5)return 1;
                    }
                }
            }
            return 0;
        }


inline int Negamax_agent:: get_length_with_dir(int py, int px, int dir, bool reverse = 0){
    if(min(py,px)<=0 or max(py,px)> board_size or board[py][px]==0 or dir<0 or dir>=num_direction){
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

int Negamax_agent::calculate_score(int color){
    int total_score = 0;
    for(int py=1; py<=board_size; py++){
        for(int px=1; px<=board_size; px++){
            if(board[py][px]!=color)
                continue;

            for(int dir=0; dir<num_direction; dir++){
                int dy = direction[dir][0], dx = direction[dir][1];
                //don't repeat calculate score
                if( board[py-dy][px-dx] == board[py][px] )
                    continue;
                int len_pos = get_length_with_dir(py, px, dir), len_neg = get_length_with_dir(py,px,dir,1);
                int num_obstacle = ( board[py+dy*len_pos][px+dx*len_pos]!= 0 ) +
                        ( board[py-dy*len_neg][px-dx*len_neg]!= 0 );
                int total_length = len_pos+len_neg-1;
                if(total_length>5)total_length=0;
                total_score += score_table[num_obstacle][total_length];
            }
        }
    }
    return total_score;
}

inline int Negamax_agent::board_score(){
    return calculate_score(1) - calculate_score(-1);
}

int Negamax_agent::Negamax(int color, int depth, int alpha, int beta, vector< pair<int,int> > &opt_path_rec, int start_time=-1, int time_limit=1e9, bool use_gomoku_cut){
    if(depth<=0){
        return color*board_score();
    }

    if(is_win(color))return 1e9+7;
    else if(is_win(-color))return -(1e9+7);
    int opt_score = -1e9;
    // int query = recorder.query_minimax_solution(*this, color, depth);
    // if(query != query_fail and depth<opt_path_rec.size()){
    //     if(show_progress)cout<<"return by transposition table\n";
    //     return query;
    // }

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

pair<int,int> Negamax_agent:: get_opt_move(int color, int limit_time, bool show_detail){
    pair<int,int> opt_solution={-1,-1};
    for(int depth = 1; depth<5; depth++){
        
        vector< pair<int,int> > opt_path(depth);
        auto time_now = chrono::steady_clock::now();
        auto time_ms  = chrono::time_point_cast< chrono::milliseconds >( time_now );
        
        double time_start = time_ms.time_since_epoch().count();

        int opt_score = Negamax(color, depth,-1e9, 1e9, opt_path, time(0), limit_time, true);
        if( opt_path[0].first!=0 and opt_path[0].second!=0 )
            opt_solution = opt_path[0];
		//cout<<"sucessfully calling Negamax function\n";
        if(opt_score == -2e9){
            if(show_detail)cout<<"interrupted at "<<depth<<"-th layer\n";
            break;
        }
        for(int i=0;i<opt_path.size();i++){
                for(int j=0;j<board_size*board_size;j++){
                    if( opt_path[i] == visit_seq[j] ){
                        for(int k=j;k>i;k--)swap( visit_seq[k], visit_seq[k-1] );
                        break;
                    }
                }
        }
        if(show_detail){
            cout<<"curret optimal score: "<< opt_score<<endl;
            cout<<"current optimal point: ("<< opt_solution.first<<','<< opt_solution.second<<" )\n\n";
            cout<<"following is the path:\n";
            print_path(opt_path, color);
        }
    }
    return opt_solution;
}


inline int Negamax_agent:: detect_5(int color, pair<int,int> pt){//判斷下在pt是否能做�?5, if yes the return 50
    if(board[pt.first][pt.second] != color)return 0;
    int py = pt.first, px = pt.second;
    for(int dir=0 ; dir<num_direction ; dir++){
        int dy = direction[dir][0], dx = direction[dir][1];
        int len_pos = get_length_with_dir(py, px, dir, false);
        int len_neg = get_length_with_dir(py, px, dir, true);
        if(len_pos+len_neg-1 == 5) return 50;
    }
    return 0;
}

inline int Negamax_agent :: detect_4(int color, pair<int,int> pt){// return 0 if there's no 4, 4 if there's only a dead 4, 40 if there's a live 4
    if(board[pt.first][pt.second] != color)return 0;
    int res=0;//40: live 4, 4: dead 4
    int py = pt.first, px = pt.second;
    for(int dir=0 ; dir<num_direction and res!=40 ; dir++){
        int dy = direction[dir][0], dx = direction[dir][1];
        int len_pos = get_length_with_dir(py, px, dir, false);
        int len_neg = get_length_with_dir(py, px, dir, true);
        int total_len = len_pos + len_neg -1;
        if(total_len == 4){
            if( min(py+dy*len_pos, px+dx*len_pos) < 0 or max(py+dy*len_pos, px+dx*len_pos) > board_size+1 )
                cout<<"ERROR: out of bound=======================================================================\n";

            if(min(py-dy*len_neg, px-dx*len_neg)<0 or max(py-dy*len_neg, px-dx*len_neg) > board_size + 1){
                cout<<"ERROR: out of bound=======================================================================\n";
            }
            
            bool open_pos = (board[ py+dy*len_pos ][ px+dx*len_pos ] == 0);
            bool open_neg = (board[ py-dy*len_neg ][ px-dx*len_neg ] == 0);
            if( open_pos and open_neg )res = 40;
            if( open_pos or open_neg )res = max(res, 4);
        }
    }
    return res;
};

inline bool Negamax_agent::Gomoku_knowledge_cut(int color, vector< pair<int,int> >& candidate_pts){
    int state_self = 0, state_opponent = 0;
    for(int i=0;i<board_size*board_size;i++){
        pair<int,int> pt = visit_seq[i];
        if( board[ pt.first ][ pt.second ] != 0)continue;
        
        move(color, pt);
        int current_state_self     = max( detect_4(color, pt), detect_5(color, pt) );
        remove(color, pt);

        move(-color, pt);
        int current_state_opponent = max( detect_4(-color, pt), detect_5(-color, pt) );
        remove(-color, pt);
        
        if(current_state_self == 50){
            if(candidate_pts.size())candidate_pts.clear();
            candidate_pts.push_back(pt);    
            return 1;
        }
        if(current_state_self != 50 and current_state_opponent == 50){
            if(candidate_pts.size())candidate_pts.clear();
            candidate_pts.push_back(pt);
        };
        if ( state_opponent<50  and current_state_self == 40){
            if(candidate_pts.size())candidate_pts.clear();
            candidate_pts.push_back(pt);    
        };
        if( state_self != 40 and current_state_opponent == 40){
            if(state_self != 4)candidate_pts.push_back(pt);
        };

        state_self = max(state_self, current_state_self);
        state_opponent = max(state_opponent, current_state_opponent);
    }
    //cout<<"value:"<<state_self<<"/"<<state_opponent<<endl;
    return (candidate_pts.size()!=0);
};