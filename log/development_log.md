# Gomoku AI (ABG) game algorithm development log

## current algorithm structure
- Board
basic Go board, implement all function of a Go board in the real world
    - add/erase stones from board
    - judge if a axis in/out of the board

- Negamax-agent
search algorithm, using Negamax(a variant of Minimax, merge min and max layer) algorithm
    - running Negamax algorithm
    - doing the search-sequence permutation

- board_evaluator
evaluate the board situation, use Gomoku knowledge
    - win/lose detect
    - board-score calculate
    - attack-move or defend-move detect

## Running
use Gomoku problems as benchmark to enhance board_evaluator


## ref.
- https://587.renju.org.tw/
