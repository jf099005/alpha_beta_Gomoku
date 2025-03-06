#include"../H/gomoku_Negamax.h"
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
                int len_pos = get_length_with_dir(py, px, dir, 0), len_neg = get_length_with_dir(py, px, dir, 1);
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
int Negamax_agent:: detect_5(int color, pair<int,int> pt){//判斷下在pt是否能做�?5, if yes the return 50
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

int Negamax_agent :: detect_4(int color, pair<int,int> pt){// return 0 if there's no 4, 4 if there's only a dead 4, 40 if there's a live 4
    if(board[pt.first][pt.second] != color)return 0;
    int res=0;//40: live 4, 4: dead 4
    int py = pt.first, px = pt.second;
    for(int dir=0 ; dir<num_direction && res!=40 ; dir++){
        int dy = direction[dir][0], dx = direction[dir][1];
        int len_pos = get_length_with_dir(py, px, dir, false);
        int len_neg = get_length_with_dir(py, px, dir, true);
        int total_len = len_pos + len_neg -1;
        if(total_len == 4){
            if( min(py+dy*len_pos, px+dx*len_pos) < 0 || max(py+dy*len_pos, px+dx*len_pos) > board_size+1 )
                cout<<"ERROR: out of bound=======================================================================\n";

            if(min(py-dy*len_neg, px-dx*len_neg)<0 || max(py-dy*len_neg, px-dx*len_neg) > board_size + 1){
                cout<<"ERROR: out of bound=======================================================================\n";
            }
            
            bool open_pos = (board[ py+dy*len_pos ][ px+dx*len_pos ] == 0);
            bool open_neg = (board[ py-dy*len_neg ][ px-dx*len_neg ] == 0);
            if( open_pos && open_neg )res = 40;
            if( open_pos || open_neg )res = max(res, 4);
        }
    }
    return res;
};

bool Negamax_agent::Gomoku_knowledge_cut(int color, vector< pair<int,int> >& candidate_pts){
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
        if(current_state_self != 50 && current_state_opponent == 50){
            if(candidate_pts.size())candidate_pts.clear();
            candidate_pts.push_back(pt);
        };
        if ( state_opponent<50  && current_state_self == 40){
            if(candidate_pts.size())candidate_pts.clear();
            candidate_pts.push_back(pt);    
        };
        if( state_self != 40 && current_state_opponent == 40){
            if(state_self != 4)candidate_pts.push_back(pt);
        };

        state_self = max(state_self, current_state_self);
        state_opponent = max(state_opponent, current_state_opponent);
    }
    //cout<<"value:"<<state_self<<"/"<<state_opponent<<endl;
    return (candidate_pts.size()!=0);
};