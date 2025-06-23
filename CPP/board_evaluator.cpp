#include"../H/board_evaluator.h"
#include<cassert>
#include<cstdlib>
#include <string>
#include <vector>

const vector<std::string> board_evaluator::one_step_draw_shape = { ".OOOO.", "OOO.O.OOO", "OO.OO.OO", "O.OOO.O" };
const vector<std::string> board_evaluator::one_step_shape = { ".OOOO", "O.OOO", "OO.OO" };
const vector<std::string> board_evaluator::two_step_shape = { ".OOO..", ".OO.O.", ".O.O.O.O.", "OOO...OOO" };

const int board_evaluator:: direction[4][2] = { {1,0},{0,1},{1,1},{1,-1}};
const int board_evaluator::score_table[3][6] = {
    {0,20,100,500,5000,100000},
    {0,0,20,100,500,100000},
    {0,0,0,0,0,100000}
};



const map<string, vector<int> > board_evaluator::defend_positions = {
        {".OOOO", 
            {0}
        },
        
        {"O.OOO",
            {1}
        },

        {"OO.OO",
            {2}
        },

        {".OOO..",
            {0, 4, 5}
        },

        {".OO.O.", 
            {0, 3, 5}
        },

        { ".O.O.O.O.", 
            {2, 4, 6}
        },

        {"OOO...OOO",
            {3, 4, 5}
        }

    };

board_evaluator::board_evaluator(gomoku_board *board, pair<int,int> *seq):
    visit_seq(seq), Board(board)
{
    state_map = {
        {STATE::one_step_draw, one_step_draw_shape},
        {STATE::one_step,   one_step_shape},
        {STATE::two_step, two_step_shape}
    };

    if(visit_seq == NULL){
        cout<<"randomized initialize evaluator visit sequence\n";
        visit_seq = new pair<int,int>[ board->board_size * board->board_size ];
        for(int i=0;i< board->board_size * board->board_size; i++){
            visit_seq[i] = pair<int,int>(i/board->board_size, i%board->board_size);
        }
    }
}

int board_evaluator::get_length_with_dir(int py, int px, int dir, bool reverse = 0){
    if(min(py,px)<=0 || max(py,px)> Board->board_size || Board->get(py,px)==0 || dir<0 || dir>=num_direction){
        return 0;
    };
    int len = 1;
    int dy = direction[dir][0], dx = direction[dir][1];
    if(reverse){
        dx=-dx; dy=-dy;
    }
    while( Board->get( py+len*dy, px+len*dx) == Board->get(py, px) ){
        len++;
        // assert(len<=5);
        if(len>5){
            cout<<"DETECTED LENGTH >= 5 WHEN CALLING get_length_with_dir==============\n";
            Board->print_board();
            return len;
        }
    }
    return len;
}

int board_evaluator::calculate_score(int color){
    int total_score = 0;
    for(int py=1; py<=Board->board_size; py++){
        for(int px=1; px<=Board->board_size; px++){
            if(Board->get(py, px)!=color)
                continue;

            for(int dir=0; dir<num_direction; dir++){
                int dy = direction[dir][0], dx = direction[dir][1];
                //don't repeat calculate score
                if( Board->get(py-dy, px-dx) == Board->get(py, px) )
                    continue;
                int len_pos = get_length_with_dir(py, px, dir, 0), len_neg = get_length_with_dir(py, px, dir, 1);
                int num_obstacle = ( Board->get(py+dy*len_pos, px+dx*len_pos)!= 0 ) +
                        ( Board->get(py-dy*len_neg, px-dx*len_neg)!= 0 );
                int total_length = len_pos+len_neg-1;
                if(total_length>5)total_length=0;
                total_score += score_table[num_obstacle][total_length];
            }
        }
    }
    return total_score;
}

int board_evaluator::board_score(){
    return calculate_score(1) - calculate_score(-1);
}



int board_evaluator:: detect_5(int color, pair<int,int> pt){//judge if is can make 5 when put stone at pt, if yes the return 50
    if(Board->get(pt) != color)return 0;
    int py = pt.first, px = pt.second;
    for(int dir=0 ; dir<num_direction ; dir++){
        int dy = direction[dir][0], dx = direction[dir][1];
        int len_pos = get_length_with_dir(py, px, dir, false);
        int len_neg = get_length_with_dir(py, px, dir, true);
        if(len_pos+len_neg-1 >= 5) return 50;
    }
    return 0;
}
bool board_evaluator::shape_compare(int color, string shape, pair<int,int> pt, int dy, int dx){
    assert( (dx == 1 || dx == -1 || dx == 0) && (dy==1 ||dy == -1 || dy == 0) );
    assert( dx !=0 || dy != 0 );
    assert(this->Board->in_board(pt.first, pt.second));
    for(int j=0;j<shape.length();j++){
        int ny = pt.first + j*dy;
        int nx = pt.second + j*dx;
        if(! this->Board->in_board(ny, nx)){
            return false;
        }
        else if( shape[j] == 'O' && Board->get(ny, nx) != color ){
            return false;
        }
        else if(shape[j] == '.' && Board->get(ny, nx) != 0){
            return false;
            // cout<<"\t\t break at "<<ny<<","<<nx<<endl;
        }
    }
    return true;
}


bool board_evaluator::shape_match(int color, string shape, pair<int,int> pt, int dy, int dx){
    assert( (dx == 1 || dx == -1 || dx == 0) && (dy==1 ||dy == -1 || dy == 0) );
    assert( dx !=0 || dy != 0 );
    // assert(dx == 0 || dy == 0);
    // assert(Board->get(pt) != 0);
    assert(this->Board->in_board(pt.first, pt.second));
    // cout<<"match start\n";
    // cout<<"shape length:"<<shape.length()<<endl;
    for(int i= 0; i<shape.length() ; i++){
        // cout<<"i:"<<i<<endl;
        int ref_y = pt.first - i*dy;
        int ref_x = pt.second - i*dx;
        
        // cout<<"ref:"<<ref_y<<","<<ref_x<<endl;

        if(!(this->Board->in_board(ref_y, ref_x)))
            continue;
        bool cmp_result = true;
        for(int j=0;j<shape.length() && cmp_result;j++){
            int ny = ref_y + j*dy;
            int nx = ref_x + j*dx;
            if(! this->Board->in_board(ny, nx)){
                cmp_result = false;
                break;
            }
            else if( shape[j] == 'O' && Board->get(ny, nx) != color ){
                cmp_result = false;
            }
            else if(shape[j] == '.' && Board->get(ny, nx) != 0){
                cmp_result = false;
                // cout<<"\t\t break at "<<ny<<","<<nx<<endl;
            }
        }
        if(cmp_result)
            return true;
    }
    return false;
}
int board_evaluator :: detect_4(int color, pair<int,int> pt){// return 0 if there's no 4, 4 if there's only a dead 4, 40 if there's a live 4
    if(Board->get(pt) != color)return 0;
    int res=0;//40: live 4, 4: dead 4
    int py = pt.first, px = pt.second;
    for(auto shape: one_step_draw_shape){
        for(int dir=0 ; dir<num_direction && res!=40 ; dir++){
            int dy = direction[dir][0], dx = direction[dir][1];
            for(int d=1;d>=-1;d-=2){
                dy *= d;
                dx *= d;
                if(shape_match(color, shape, pt, dy, dx))
                    return 40;
            }
        }
    };

    for(auto shape: one_step_shape){
        for(int dir=0 ; dir<num_direction && res!=40 ; dir++){
            int dy = direction[dir][0], dx = direction[dir][1];
            for(int d=1;d>=-1;d-=2){
                dy *= d;
                dx *= d;
                if(shape_match(color, shape, pt, dy, dx))
                    return 4;
            }
        }
    };
    return res;
}

bool board_evaluator :: match_attack(int color, pair<int,int> pt, STATE attack_type, bool show_detail = false){// return 0 if there's no 4, 4 if there's only a dead 4, 40 if there's a live 4
    // if(Board->get(pt) != color)return false;
    int py = pt.first, px = pt.second;
    vector<string> &shapes = state_map[attack_type];
    for(auto shape: shapes){
        
        if(show_detail)cout<<"check shape "<<shape<<endl;

        for(int dir=0 ; dir<num_direction; dir++){
            int dy = direction[dir][0], dx = direction[dir][1];
            for(int d=1;d>=-1;d-=2){
                dy *= d;
                dx *= d;
                if(shape_match(color, shape, pt, dy, dx)){
                    if(show_detail)cout<<"matched "<<shape<<" of "<<pt.first<<","<<pt.second<<"  "<<dy<<"/"<<dx <<endl;
                    return true;
                }
            }
        }
    };
    return false;
}


bool board_evaluator::Gomoku_knowledge_cut(int color, vector< pair<int,int> >& candidate_pts){
    int state_self = 0, state_opponent = 0;
    for(int i=0;i<Board->board_size*Board->board_size;i++){
        pair<int,int> pt = visit_seq[i];
        if( Board->get(pt) != 0)continue;
        
        Board->add_stone(color, pt);
        int current_state_self     = max( detect_4(color, pt), detect_5(color, pt) );
        Board->erase(color, pt);

        Board->add_stone(-color, pt);
        int current_state_opponent = max( detect_4(-color, pt), detect_5(-color, pt) );
        Board->erase(-color, pt);
        
        // if(pt.first == 1 && pt.second == 5)
        //    cout<<"checking "<<pt.first<<","<<pt.second<<":  "<<current_state_self<<" / "<<current_state_opponent<<endl;

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
}

bool board_evaluator::is_win(int color){
    for(int py = 0;py<=Board->board_size;py++){
        for(int px=0;px<=Board->board_size;px++){
            for(int dir=0;dir<num_direction;dir++){
                if(Board->get(py,px)!=color)continue;
                if( min( py-direction[dir][0],  px-direction[dir][1])>0 && 
                        max( py-direction[dir][0],  px-direction[dir][1])<= Board->board_size &&
                            Board->get( py-direction[dir][0], px-direction[dir][1] ) == color ){
                                continue;
                }
                if( get_length_with_dir(py, px, dir, false) ==5)return 1;
            }
        }
    }
    return 0;
}



bool board_evaluator::attack_to_win(int attacker, int depth, bool show_detail){
    assert(attacker == 1 || attacker == -1);
    assert(depth >= 0);
    
    if(show_detail){
        cout<<"evaluate:\n";
        Board->print_board();
        cout<<"\t depth:"<<depth<<endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
    }
    if(depth == 0)
        return false;
    
    for(int i=0; i<Board->board_size * Board->board_size; i++){
        pair<int,int> atk_pt = visit_seq[i];//cur_pt: current visited point
        if(!Board->is_valid_move(atk_pt))
            continue;
        Board->add_stone(attacker, atk_pt);
        // int valid_attack = detect_4(attacker, atk_pt);

        bool one_step_draw_attack = match_attack(attacker, atk_pt, STATE::one_step_draw);
        bool one_step_attack = match_attack(attacker, atk_pt, STATE::one_step);
        bool two_step_attack = match_attack(attacker, atk_pt, STATE::two_step);

        int winning = detect_5(attacker, atk_pt);
        if(show_detail){
            cout<<"search point for "<<atk_pt.first<<","<<atk_pt.second<<",layer "<<depth<<endl;
            cout<<"\t result:"<<one_step_attack<<"/"<<two_step_attack<<endl;
        }
        if(one_step_attack && show_detail)match_attack(attacker, atk_pt, STATE::one_step, true);
        if(two_step_attack && show_detail)match_attack(attacker, atk_pt, STATE::two_step, true);
        // std::cout << "Press Enter to continue...";
        // std::cin.get();

        if(winning || one_step_draw_attack){
            // cout<<"return\n";
            Board->erase(attacker, atk_pt);
            return true;
        }
        
        bool attack_success = false;
        if(two_step_attack || one_step_attack){
            attack_success = true;


            for(int j=0; j<Board->board_size * Board->board_size && attack_success; j++){
                pair<int,int> def_pt = visit_seq[j];// move to defend the attack
                if(!Board->is_valid_move(def_pt))
                    continue;

                Board->add_stone(-attacker, def_pt);
                if(detect_5(-attacker, def_pt)){
                    attack_success = false;
                    Board->erase(-attacker, def_pt);
                    continue;
                }
                bool one_step_attack_after = match_attack(attacker, atk_pt, STATE::one_step);
                bool two_step_attack_after = match_attack(attacker, atk_pt, STATE::two_step);

                bool defend_success = !(one_step_attack_after || two_step_attack_after);
                defend_success |= (!one_step_attack_after) && match_attack(-attacker, def_pt, STATE::one_step);
                
                // if(def_pt.first == 7 && def_pt.second == 10){
                if(show_detail){
                    cout<<"\ttry defend on "<<def_pt.first<<","<<def_pt.second<<" of depth "<<depth<<"\n";
                    Board->print_board();
                    cout<<"\t"<<one_step_attack_after<<"/"<<two_step_attack_after<<endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.get();

                }

                if(defend_success){
                    defend_success &= attack_to_win(attacker, depth-1);
                    // defend_success &= !(Board->in_board(nx_atk));
                }
                Board->erase(-attacker, def_pt);
                attack_success &= (!defend_success);
            }
        }
        Board->erase(attacker, atk_pt);
        if(attack_success)
            return true;
    }
    return false;
}

pair<int,int> board_evaluator::get_victory_move(int attacker, int depth){
    assert(attacker == 1 || attacker == -1);
    assert(depth >= 0);
    
    if(depth == 0)
        return {-1, -1};
    
    for(int i=0; i<Board->board_size * Board->board_size; i++){
        pair<int,int> atk_pt = visit_seq[i];//cur_pt: current visited point
        if(!Board->is_valid_move(atk_pt))
            continue;
        Board->add_stone(attacker, atk_pt);
        // int valid_attack = detect_4(attacker, atk_pt);

        bool one_step_draw_attack = match_attack(attacker, atk_pt, STATE::one_step_draw);
        bool one_step_attack = match_attack(attacker, atk_pt, STATE::one_step);
        bool two_step_attack = match_attack(attacker, atk_pt, STATE::two_step);

        int winning = detect_5(attacker, atk_pt);
        
        if(winning || one_step_draw_attack){
            // cout<<"return\n";
            Board->erase(attacker, atk_pt);
            return {0,0};
        }
        
        bool attack_success = false;
        if(two_step_attack || one_step_attack){
            attack_success = true;


            for(int j=0; j<Board->board_size * Board->board_size && attack_success; j++){
                pair<int,int> def_pt = visit_seq[j];// move to defend the attack
                if(!Board->is_valid_move(def_pt))
                    continue;

                Board->add_stone(-attacker, def_pt);
                if(detect_5(-attacker, def_pt)){
                    attack_success = false;
                    Board->erase(-attacker, def_pt);
                    continue;
                }
                bool one_step_attack_after = match_attack(attacker, atk_pt, STATE::one_step);
                bool two_step_attack_after = match_attack(attacker, atk_pt, STATE::two_step);

                bool defend_success = !(one_step_attack_after || two_step_attack_after);
                defend_success |= (!one_step_attack_after) && match_attack(-attacker, def_pt, STATE::one_step);

                if(defend_success){
                    defend_success &= attack_to_win(attacker, depth-1);
                    // defend_success &= !(Board->in_board(nx_atk));
                }
                Board->erase(-attacker, def_pt);
                attack_success &= (!defend_success);
            }
        }
        Board->erase(attacker, atk_pt);
        if(attack_success)
            return atk_pt;
    }
    return {-1, -1};
}

