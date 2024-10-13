#include <map>
#include <algorithm>
#include <climits>
#include "Action.hpp"
#include "Block.hpp"
#include "BlockCluster.hpp"
#include "Level.hpp"
#include "Result.hpp"
#include "Session.hpp"

class Session;

Level::~Level() {}
Level2::~Level2() {}
Level1::~Level1() {}
Level0::~Level0() {}
Level3::~Level3() {}
Level4::~Level4() {}

int Level::get_type_id() const {
  return _type_id;
}

std::vector<int> Level::get_full_rows(int starting_row) {
    std::vector<int> full_rows;

    for (int y = starting_row; y < _board.size(); ++y) {
        int empty = true;
        int full = true;
        for (int x = 0; x < _board.at(y).size(); x++) {
            if (_board.at(y).at(x) != nullptr) {
                empty = false;
            }
            else {
                full = false;
            }
            if (!empty && !full) {
                break;
            }
        }
        // if the row is clear, we don't need to continue searching up
        if (empty) {
            break;
        }
        if (full) {
            full_rows.push_back(y);
        }
    }
    return full_rows;
}

int Level::clear_rows(int starting_row) {
    std::vector<int> full_rows = get_full_rows(starting_row);
    if (full_rows.size() == 0) {
        return 0;
    }
    
    _score += (_type_id + full_rows.size()) * (_type_id + full_rows.size());
    
    // clear rows
    for (auto const& row: full_rows) { 
        for (int x = 0; x < _board.at(row).size(); ++x) {
            _board.at(row).at(x) = nullptr;
        }

        for(int i = _live_blocks.size() - 1; i >= 0; --i) {
            if (_live_blocks.at(i) -> remove_blocks(row)) {
                _score += _live_blocks.at(i) -> get_removal_score();
                _live_blocks.erase(_live_blocks.begin() + i);
            }
        }
    }

    int y_offset = 0;
    for (auto const& row: full_rows) { 
         // y_offset required since each line clear shifts the indices
        for (int y = row; y < _board.size() - 1; ++y) {
            for (int x = 0; x < _board.at(y).size(); ++x) {
                _board.at(y + y_offset).at(x) = _board.at(y + y_offset + 1).at(x);
                if (_board.at(y + y_offset).at(x) != nullptr) {
                    _board.at(y + y_offset).at(x) -> move_to(x, y + y_offset);
                }
            }
        }
        --y_offset;
    }
    return static_cast<int>(full_rows.size());
}

bool Level::out_of_bounds(std::shared_ptr<BlockCluster> block_cluster) {
    for (auto const& block: block_cluster -> get_blocks()) {
        std::pair<int, int> coordinates = block -> get_coordinates();
        if (coordinates.first < 0 || coordinates.first >= 11 ||
            coordinates.second < 0 || coordinates.second >= 18) {
            return true;
        }
    }
    return false;
}


bool Level::collides(std::shared_ptr<BlockCluster> block_cluster) {
    for (auto const& block: block_cluster -> get_blocks()) {
        std::pair<int, int> coordinates = block -> get_coordinates();
        if (_board.at(coordinates.second).at(coordinates.first) != nullptr) {
            return true;
        }
    }
    return false;
}

bool Level::can_move(std::shared_ptr<BlockCluster> block_cluster, std::pair<int, int> change) {
    // check for blocks and for border 
    for (auto const& block: block_cluster -> get_blocks()) {
        std::pair<int, int> coordinates = block -> get_coordinates();
        if (coordinates.first + change.first < 0 || coordinates.first + change.first >= 11 ||
            coordinates.second + change.second < 0 || coordinates.second + change.second >= 18 ||
            _board.at(coordinates.second + change.second).at(coordinates.first + change.first) != nullptr) {
            return false;
        }
    }
    return true;
}

bool Level::move(std::shared_ptr<BlockCluster> block_cluster, std::pair<int, int> change) {
    for (auto const& block: block_cluster -> get_blocks()) {
        std::pair<int, int> coordinates = block -> get_coordinates();
        block -> move_to(coordinates.first + change.first, coordinates.second + change.second);
    }
    return true;
}

bool Level::on_block(std::shared_ptr<BlockCluster> block_cluster, std::pair<int, int> change) {
    for (auto const& block: block_cluster -> get_blocks()) {
        std::pair<int, int> coordinates = block -> get_coordinates();
        if (coordinates.second + change.second == 0 || _board.at(coordinates.second + change.second - 1).at(coordinates.first + change.first) != nullptr) {
            return true;
        }
    }
    return false;
}

bool Level::clear_above(std::shared_ptr<BlockCluster> block_cluster, std::pair<int, int> change) {
    for (auto const& block: block_cluster -> get_blocks()) {
        std::pair<int, int> coordinates = block -> get_coordinates();
        for (int y = coordinates.second + change.second; y < 15; y++) {
            if (_board.at(y).at(coordinates.first + change.first) != nullptr) {
                return false;
            }
        }
    }
    return true;
}


int Level::on_drop() {
    std::vector<int> cleared_rows;
    std::vector<std::shared_ptr<Block>> _falling_blocks = _falling_block -> get_blocks();

    // get the smallest y-value of any block in _falling_blocks
    int smallest_y = INT_MAX;
    for (auto const& block: _falling_blocks){
        std::pair<int, int> coordinates = block -> get_coordinates();
        if (coordinates.second < smallest_y) {
            smallest_y = coordinates.second;
        }
    }

    // decrease y by 1 until any collision_blocks come in contact with another block or the bottom of the board
    int decrease_y_by = smallest_y;
    for (int i = 1; i <= smallest_y; i++) {
        if (!can_move(_falling_block, std::make_pair(0, -i))) {
            break;
        }
        decrease_y_by = i;
    }

    for (auto const& block: _falling_blocks) {
        std::pair<int, int> coordinates = block -> get_coordinates();
        if (coordinates.second - decrease_y_by > 17) {
            return - 1;
        }
        block -> move_to(coordinates.first, coordinates.second - decrease_y_by);
        _board.at(coordinates.second - decrease_y_by).at(coordinates.first) = block;
    }

    _live_blocks.push_back(_falling_block);
    _falling_block = nullptr;

    // check if any rows are cleared, from the smallest y value of the newly dropped blocks,
    // until we reach a row with no blocks or the top of the board
    return clear_rows(smallest_y - decrease_y_by);
}

void Level::spawn_bc(char block_type) {
  _falling_block = std::shared_ptr<BlockCluster>(_constants -> get_bcs_table().find(block_type) -> second -> create_cluster((_type_id + 1) * (_type_id + 1), _type_id));
}

void Level::spawn_bc() {
  std::vector<char> shapes;
  for(auto it = _weights.cbegin(); it != _weights.cend(); ++it) {
    shapes.push_back(it->first);
  }
  std::sort(shapes.begin(), shapes.end());
  std::vector<int> weights;
  for(auto it = shapes.cbegin(); it != shapes.cend(); ++it) {
    weights.push_back(_weights[*it]);
  }
  Result<char, std::string> choose_result = _bcs_selector->choose(shapes, weights);
  if(choose_result.is_error()) {
    std::cout << choose_result.get_error_value() << std::endl;
  } else {
    char block_type = choose_result.get_success_value();
    spawn_bc(block_type);
  }
}

void Level::ensure_bc() {
  if(_falling_block == nullptr) {
    spawn_bc();
  }
}

void Level::on_action_end() {
  ensure_bc();
}

bool Level::handle_action(Action action) {
    if (action.get_type() == ActionType::MOVE_DOWN) {
        if (can_move(_falling_block,std::make_pair(0, -1))) {
            move(_falling_block, std::make_pair(0, -1));
        }
    }
    else if (action.get_type() == ActionType::MOVE_LEFT) {
        if (can_move(_falling_block, std::make_pair(-1, 0))) {
            move(_falling_block, std::make_pair(-1, 0));
        }
    }
    else if (action.get_type() == ActionType::MOVE_RIGHT) {
        if (can_move(_falling_block, std::make_pair(1, 0))) {
            move(_falling_block, std::make_pair(1, 0));
        }
    }
    else if (action.get_type() == ActionType::MOVE_CW) {
        _falling_block -> rotate_right();
        if (out_of_bounds(_falling_block) || collides(_falling_block)) {
            _falling_block -> rotate_left();
        }
    }
    else if (action.get_type() == ActionType::MOVE_CCW) {
        _falling_block -> rotate_left();
        if (out_of_bounds(_falling_block) || collides(_falling_block)) {
            _falling_block -> rotate_right();
        }
    }
    else if (action.get_type() == ActionType::MOVE_DROP) {
        if (_type_id == 4) {
        if (on_drop() < 0) {
                on_action_end();
                return true;
            }
        }
        else {
            if (Level::on_drop() < 0) {
                on_action_end();
                return true;
            }
        }
    }
    else if (action.get_type() == ActionType::LEVEL_RESET) {
        for (int y = 0; y < _board.size(); y++) {
            for (int x = 0; x < _board.at(y).size(); x++) {
                _board.at(y).at(x) = nullptr;
            }
        }
        _live_blocks.clear();
        _hint_block = nullptr;
        _falling_block = nullptr;
    }
    else if (action.get_type() == ActionType::LEVEL_HINT) {
        if (_falling_block == nullptr) return false;
        std::shared_ptr<BlockCluster> temp_hint_block = std::shared_ptr<BlockCluster>((new BlockClusterShape(_falling_block -> get_style() -> print_block(), true)) -> create_cluster(0, 0));

        int most_full_lines = INT_MIN;

        // move cluster to bottom left of board
        int shift_left_by = 0;
        for (int i = 1; i < _board.at(0).size(); ++i) {
            bool found = false;
            for (auto const& block: temp_hint_block -> get_blocks()) {
                if (block -> get_coordinates().first - i < 0) {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                break;
            }
            shift_left_by = i;
        }

        int shift_down_by = 0;
        for (int i = 1; i < _board.size(); ++i) {
            bool found = false;
            for (auto const& block: temp_hint_block -> get_blocks()) {
                if (block-> get_coordinates().second - i < 0) {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                break;
            }
            shift_down_by = i;
        }
        
        move(temp_hint_block, std::make_pair(shift_left_by, -shift_down_by));
        
        for (int i = 0; i < 4; ++i) {
            temp_hint_block -> rotate_right();

            for (int y = 0; y < _board.size() - 3; ++y) {
                for (int x = 0; x < _board.at(y).size(); ++x) {
                    // if is a possible move
                    if (can_move(temp_hint_block, std::make_pair(x, y)) && on_block(temp_hint_block, std::make_pair(x, y)) && clear_above(temp_hint_block, std::make_pair(x, y))) {
                        for (auto const& block: temp_hint_block -> get_blocks()) {
                            std::pair<int, int> coordinates = block -> get_coordinates();
                            _board.at(coordinates.second + y).at(coordinates.first + x) = block;
                        }
                        // check how many lines would be filled
                        // if it fills more lines than current max, then set the hint block to this
                        if ((static_cast<int>(get_full_rows(y).size()) > most_full_lines)) {
                            move(temp_hint_block, std::make_pair(x, y));
                            _hint_block = temp_hint_block;
                            most_full_lines = static_cast<int>(get_full_rows(y).size());
                            
                            for (auto const& block: temp_hint_block -> get_blocks()) {
                                std::pair<int, int> coordinates = block -> get_coordinates();
                                _board.at(coordinates.second).at(coordinates.first) = nullptr;
                            }
                                
                        }
                        else {
                            for (auto const& block: temp_hint_block -> get_blocks()) {
                                std::pair<int, int> coordinates = block -> get_coordinates();
                                _board.at(coordinates.second + y).at(coordinates.first + x) = nullptr;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (action.get_type() == ActionType::RANDOM_OFF && (_type_id == 3 || _type_id == 4)) {
        _bcs_selector -> set_sequence_from(action.get_parameter().value_or(""));
    }
    else if (action.get_type() == ActionType::RANDOM_ON && (_type_id == 3 || _type_id == 4)) {
        _bcs_selector -> clear_sequence();
    }
    else if (action.get_type() >= ActionType::SPAWN_I && action.get_type() <= ActionType::SPAWN_T) {
        std::string spawn_action_to_char_conversion = "IJLSZOT";
        char block_type = spawn_action_to_char_conversion.at((static_cast<int>(action.get_type())) - 15);
        spawn_bc(block_type);
        // game over
        if (collides(_falling_block)) {
            on_action_end();
            return true;
        }
    }
    else if (action.get_type() == ActionType::COMMAND_END && _movement_occured) {
        if (can_move(_falling_block, std::make_pair(0, -1))) {
            move(_falling_block, std::make_pair(0, -1));
        }
        _movement_occured = false;
    }

    // heavy block
    if (_falling_block != nullptr && _falling_block -> get_level_spawned() >= 3 && action.get_type() >= ActionType::MOVE_DOWN && action.get_type() <= ActionType::MOVE_DROP) {
        _movement_occured = true;
    }

    // clear hint
    if (action.get_type() != ActionType::LEVEL_HINT && action.get_type() != ActionType::COMMAND_END) {
        _hint_block = nullptr;
    }

    on_action_end();
    return false;
}

std::array<std::array<std::shared_ptr<Block>, 11>, 18> Level::get_full_board() {
    std::array<std::array<std::shared_ptr<Block>, 11>, 18> full_board;

    // fill full_board with board
    for (int y = 0; y < _board.size(); ++y) {
        for (int x = 0; x < _board.at(y).size(); ++x) {
            full_board.at(y).at(x) = _board.at(y).at(x);
        }
    }
    
    // add in hint block and falling block
    if(_hint_block) {
      std::vector<std::shared_ptr<Block>> _hint_blocks = _hint_block -> get_blocks();
      for (auto const& block: _hint_blocks){
          std::pair<int, int> coordinates = block -> get_coordinates();
          full_board.at(coordinates.second).at(coordinates.first) = block;
      }
    }

    if(_falling_block) {
      std::vector<std::shared_ptr<Block>> _falling_blocks = _falling_block -> get_blocks();
      for (auto const& block: _falling_blocks){
          std::pair<int, int> coordinates = block -> get_coordinates();
          full_board.at(coordinates.second).at(coordinates.first) = block;
      }
    }
    
    return full_board;
}

Result<bool, std::string> Level::update_from(Session* session) {
    _constants = session -> get_session_constants();
    _bcs_selector = session -> get_bcs_selector();
    if (_type_id == 0) {
        return _bcs_selector -> set_sequence_from(session -> get_level_0_sequence());
    } else if (_type_id == 1 || _type_id == 2){
      _bcs_selector->clear_sequence();
    }

    ensure_bc();
    return Result<bool, std::string>::make_success(true);
}

Level2::Level2(std::shared_ptr<SessionConstants> constants, std::shared_ptr<BCSSelector> bcs_selector, std::array<std::array<std::shared_ptr<Block>, 11>, 18> board, std::vector<std::shared_ptr<BlockCluster>> live_blocks, std::shared_ptr<BlockCluster> falling_block, int score) {
    _type_id = 2;
    _weights = {
        {'I', 1},
        {'J', 1},
        {'L', 1},
        {'O', 1},
        {'S', 1},
        {'Z', 1},
        {'T', 1}
    };
    _constants = constants;
    _bcs_selector = bcs_selector;
    _board = board;
    _live_blocks = live_blocks;
    _falling_block = falling_block;
    _score = score;
}

std::shared_ptr<Level> Level2::pass_off(int new_type_id) {
    if (new_type_id == 0) {
        return std::shared_ptr<Level>(new Level0(_constants, _bcs_selector, _board, _live_blocks, _falling_block, _score));
    }
    else if (new_type_id == 1) {
        return std::shared_ptr<Level>(new Level1(_constants, _bcs_selector, _board, _live_blocks, _falling_block, _score));
    }
    else if (new_type_id == 3) {
        return std::shared_ptr<Level>(new Level3(_constants, _bcs_selector, _board, _live_blocks, _falling_block, _score));
    }

    return std::shared_ptr<Level>(nullptr);
}

Level1::Level1(std::shared_ptr<SessionConstants> constants, std::shared_ptr<BCSSelector> bcs_selector, std::array<std::array<std::shared_ptr<Block>, 11>, 18> board, std::vector<std::shared_ptr<BlockCluster>> live_blocks, std::shared_ptr<BlockCluster> falling_block, int score) : Level2(constants, bcs_selector, board, live_blocks, falling_block, score) {
    _type_id = 1;
    _weights = {
        {'I', 2},
        {'J', 2},
        {'L', 2},
        {'O', 2},
        {'S', 1},
        {'Z', 1},
        {'T', 2}
    };
    _constants = constants;
    _bcs_selector = bcs_selector;
    _board = board;
    _live_blocks = live_blocks;
    _falling_block = falling_block;
    _score = score;
}

std::shared_ptr<Level> Level1::pass_off(int new_type_id) {
    if (new_type_id == 2) {
        return std::shared_ptr<Level>(new Level2(_constants, _bcs_selector, _board, _live_blocks, _falling_block, _score));
    }

    return std::shared_ptr<Level>(nullptr);
}

Level0::Level0() : Level2(nullptr, nullptr, {}, std::vector<std::shared_ptr<BlockCluster>>(), nullptr, 0) {
    _score = 0;
    _type_id = 0;
}

Level0::Level0(std::shared_ptr<SessionConstants> constants, std::shared_ptr<BCSSelector> bcs_selector, std::array<std::array<std::shared_ptr<Block>, 11>, 18> board, std::vector<std::shared_ptr<BlockCluster>> live_blocks, std::shared_ptr<BlockCluster> falling_block, int score) : Level2(constants, bcs_selector, board, live_blocks, falling_block, score) {
    _type_id = 0;
    _constants = constants;
    _bcs_selector = bcs_selector;
    _board = board;
    _live_blocks = live_blocks;
    _falling_block = falling_block;
    _score = score;
}

std::shared_ptr<Level> Level0::pass_off(int new_type_id) {
    if (new_type_id == 2) {
        return std::shared_ptr<Level>(new Level2(_constants, _bcs_selector, _board, _live_blocks, _falling_block, _score));
    }

    return std::shared_ptr<Level>(nullptr);
}

Level3::Level3(std::shared_ptr<SessionConstants> constants, std::shared_ptr<BCSSelector> bcs_selector, std::array<std::array<std::shared_ptr<Block>, 11>, 18> board, std::vector<std::shared_ptr<BlockCluster>> live_blocks, std::shared_ptr<BlockCluster> falling_block, int score) : Level2(constants, bcs_selector, board, live_blocks, falling_block, score) {
    _type_id = 3;
    _weights = {
        {'I', 1},
        {'J', 1},
        {'L', 1},
        {'O', 1},
        {'S', 2},
        {'Z', 2},
        {'T', 1}
    };
    _constants = constants;
    _bcs_selector = bcs_selector;
    _board = board;
    _live_blocks = live_blocks;
    _falling_block = falling_block;
    _score = score;
}


std::shared_ptr<Level> Level3::pass_off(int new_type_id) {
    if (new_type_id == 2) {
        return std::shared_ptr<Level>(new Level2(_constants, _bcs_selector, _board, _live_blocks, _falling_block, _score));
    }
    else if (new_type_id == 4) {
        return std::shared_ptr<Level>(new Level4(_constants, _bcs_selector, _board, _live_blocks, _falling_block, _score));
    }

    return std::shared_ptr<Level>(nullptr);
}

Level4::Level4(std::shared_ptr<SessionConstants> constants, std::shared_ptr<BCSSelector> bcs_selector, std::array<std::array<std::shared_ptr<Block>, 11>, 18> board, std::vector<std::shared_ptr<BlockCluster>> live_blocks, std::shared_ptr<BlockCluster> falling_block, int score) : Level3(constants, bcs_selector, board, live_blocks, falling_block, score) {
    _score = 0;
    _type_id = 4;
    _drops_since_last_clear = 0;
    _constants = constants;
    _bcs_selector = bcs_selector;
}

std::shared_ptr<Level> Level4::pass_off(int new_type_id) {
    if (new_type_id == 3) {
        return std::shared_ptr<Level>(new Level3(_constants, _bcs_selector, _board, _live_blocks, _falling_block, _score));
    }

    return std::shared_ptr<Level>(nullptr);
}

int Level4::on_drop() {
    // if lines were cleared, reset counter
    if (Level::on_drop() > 0) {
        _drops_since_last_clear = 0;
    }
    else {
        ++_drops_since_last_clear;
    }
    if (_drops_since_last_clear % 5 == 0) {
        for (int y = 0; y < _board.size(); ++y) {
            if (_board.at(y).at(5) == nullptr) {
                BlockCluster dot_block = *(_constants -> get_bcs_table().find('.') -> second -> create_cluster(0, _type_id));
                dot_block.get_blocks().at(0) -> move_to(5, y);
                _board.at(y).at(5) = dot_block.get_blocks().at(0);
                return clear_rows(y);       
            }
        }
    }
    return -1;
}
