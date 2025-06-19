#include"H/Gomoku_board.h"
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<filesystem>
#include <filesystem>
#include<random>
namespace fs = std::filesystem;
using namespace std;

class board_loader{
    private:
        string folder_dir;
        vector<string> problems_path;
        int problem_idx;
        gomoku_board *Board;
    public:
        board_loader(string folder_dir, gomoku_board *board):
            Board(board),
            folder_dir(folder_dir), problem_idx(0), problems_path(0)
        {
            srand(0);
            for (const auto& entry : fs::directory_iterator(folder_dir)) {
                problems_path.push_back( entry.path().string() );
            }
            if(problems_path.size() == 0){
                cout<<"Invalid path: no file\n";
                exit(1);
            }
            cout<<"successfully load the following problems:\n";
            for(auto s: problems_path){
                cout<<"\t"<<s<<endl;
            }
            load_problem();
        }


        void random_problem(){
            problem_idx = rand() % problems_path.size();
            load_problem( problems_path[ problem_idx ] );
        }

        void next(){
            problem_idx++;
            problem_idx %= problems_path.size();
            load_problem( problems_path[ problem_idx ] );
        }

        void load_problem(string problem_path = ""){

            
            Board->reset_all();
            if(problem_path.length() == 0)
                problem_path = problems_path[ problem_idx ];
            ifstream ifs;
            ifs.open(problem_path);
            if(ifs.fail()){
                cout<<"================ invalid problem path:"<< problem_path<<"====="<<endl;
                exit(1);
            }
            int board_size;
            ifs>>board_size;
            assert(Board->board_size == board_size);
            for(int i=1; i<=board_size; i++){
                for(int j=1; j<=board_size; j++){
                    int stone;
                    ifs >> stone;
                    Board->add_stone(stone, {i,j});
                }
            }
        }

        string current_problem(){
            return problems_path[problem_idx];
        }
};