#include"../H/gomoku_Negamax.h"

vector<string> one_step_draw_shape = { ".OOOO.", "OOO.O.OOO", "OO.OO.OO", "O.OOO.O" };
vector<string> one_step_shape = { ".OOOO",  "O.OOO", "OO.OO" };
vector<string> two_step_draw_shape = { ".OOO.", ".OO.O.", ".O.O.O.O.", "OOO...OOO" };

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
        if(len_pos+len_neg-1 == 5) return player_state::win;
    }
    return 0;
}
bool Negamax_agent::shape_cmp(int color, string shape, pair<int,int> pt, int dy, int dx){
    for(int i = 0; i < shape.length(); i++){
        int ref_y = pt.first - i*dy;
        int ref_x = pt.second - i*dx;
        if(this->out_of_bound(ref_y, ref_x))
            continue;
        bool cmp_result = true;
        // cout<<"start at "<<ref_y<<","<<ref_x<<endl;
        for(int j=0;j<shape.length() && cmp_result;j++){
            int ny = ref_y + j*dy;
            int nx = ref_x + j*dx;
            if(this->out_of_bound(ny,nx))
                cmp_result = false;

            if( shape[j] == 'O' && board[ny][nx] != color ){
                cmp_result = false;
            }

            if(shape[j] == '.' && board[ny][nx] != 0){
                cmp_result = false;
            }
            // if(!cmp_result){
            //     cout<<"\t break at "<<ny<<","<<nx<<endl;
            // }
        }
        if(cmp_result)
            return true;
    }
    return false;
}

int Negamax_agent :: detect_4(int color, pair<int,int> pt, int dir){// return 0 if there's no 4, 4 if there's only a dead 4, 40 if there's a live 4
    if(board[pt.first][pt.second] != color)return 0;
    int res=0;//40: live 4, 4: dead 4
    int py = pt.first, px = pt.second;
    for(auto shape: one_step_draw_shape){
            int dy = direction[dir][0], dx = direction[dir][1];
            for(int d=1;d>=-1;d-=2){
                dy *= d;
                dx *= d;
                if(shape_cmp(color, shape, pt, dy, dx))
                    return player_state::one_step_to_win;
            }
    };

    for(auto shape: one_step_shape){
            int dy = direction[dir][0], dx = direction[dir][1];
            for(int d=1;d>=-1;d-=2){
                dy *= d;
                dx *= d;
                if(shape_cmp(color, shape, pt, dy, dx))
                    return player_state::one_step_draw;
            }
    };
    return res;
}

bool Negamax_agent::Gomoku_knowledge_cut(int color, vector< pair<int,int> >& candidate_pts){
    player_state state_self = none, state_opponent = none;
    for(int i=0;i<board_size*board_size;i++){
        pair<int,int> pt = visit_seq[i];
        player_state current_state_self = player_state::none;
        player_state current_state_opponent = player_state::none;
        
        if( board[ pt.first ][ pt.second ] != 0)continue;
        
        // if(pt.first != 1 or pt.second != 5)
        //     continue;

        for(int dir=0;dir<num_direction;dir++){
            
            // cout<<"dir:"<<direction[dir][0]<<","<<direction[dir][1]<<endl;
            move(color, pt);
            player_state current_state_self_dir     = (player_state)max( detect_4(color, pt, dir), detect_5(color, pt) );
            remove(color, pt);

            move(-color, pt);
            player_state current_state_opponent_dir = (player_state)max( detect_4(-color, pt, dir), detect_5(-color, pt) );
            remove(-color, pt);
            
            if(current_state_self_dir == one_step_draw and current_state_self==one_step_draw){
                current_state_self = one_step_to_win;
            }

            if(current_state_opponent == one_step_draw and current_state_opponent == one_step_draw){
                current_state_opponent = one_step_to_win;
            }
            // cout<<"\t state:"<<current_state_self_dir<<endl;
            current_state_self = max(current_state_self, current_state_self_dir);
            current_state_opponent = max(current_state_opponent, current_state_opponent_dir);
        }


        if(current_state_self >= state_opponent && current_state_self > state_self){
            if(candidate_pts.size())candidate_pts.clear();
            candidate_pts.push_back(pt);    
        }
        if(current_state_opponent > state_self && current_state_opponent > state_opponent){
            if(candidate_pts.size())candidate_pts.clear();
            candidate_pts.push_back(pt);    
        }

        state_self = (player_state)max((int)state_self, (int)current_state_self);
        state_opponent = (player_state)max((int)state_opponent, (int)current_state_opponent);
        
    }
    //cout<<"value:"<<state_self<<"/"<<state_opponent<<endl;
    return (candidate_pts.size()!=0);

}