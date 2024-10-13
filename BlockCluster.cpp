#include "Block.hpp"
#include "BlockCluster.hpp"
#include <vector>

BlockCluster::BlockCluster(const std::vector<std::pair<int, int>> blocks, std::shared_ptr<BlockStyle> style, int removal_score, int level) {
    _style = style;
    _removal_score = removal_score;
    _level_spawned = level;
    _orientation = 0;

    for (int i = 0; i < blocks.size(); i ++) {
        _blocks.push_back(std::shared_ptr<Block>(new Block(blocks.at(i).first, blocks.at(i).second, style)));
    }
}

std::pair<int,int> BlockCluster::_get_pivot() {
    auto it = _blocks.begin();

    std::pair<int,int> pivot = (*it)->get_coordinates();
    for (it = it + 1; it != _blocks.end(); it++) {
        auto coords = (*it)->get_coordinates();
        if (coords.first < pivot.first) {
            pivot.first = coords.first;
        }

        if (coords.second < pivot.second) {
            pivot.second = coords.second;
        }
    }

    return pivot;
}

void BlockCluster::rotate_left() {
    std::pair<int,int> pivot = _get_pivot();

    switch (_style.get()->print_block()) {
        case 'I':
            switch (_orientation) {
                case 0:
                case 2:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first, pivot.second + 2);
                    (_blocks.at(3))->move_to(pivot.first, pivot.second + 3);
                    break;
                case 1:
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 2, pivot.second);
                    (_blocks.at(3))->move_to(pivot.first + 3, pivot.second);
                    break;
            }
            break;
        case 'J':
            switch (_orientation) {
                case 0:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second + 2);
                    break;
                case 1:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 2, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second);
                    break;
                case 2:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first, pivot.second + 2);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second + 2);
                    break;
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second);
                    break;
            }
            break;
        case 'L':
            switch (_orientation) {
                case 0:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second + 2);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second + 2);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second);
                    break;
                case 1:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second + 1);
                    break;
                case 2:
                    (_blocks.at(0))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first, pivot.second + 2);
                    break;
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 2, pivot.second);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second + 1);
                    break;
            }
            break;
        case 'O':
            break;
        case 'S':
            switch (_orientation) {
                case 0:
                case 2:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second + 2);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second);
                    break;
                case 1:
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second + 1);
                    break;
            }
            break;
        case 'Z':
            switch (_orientation) {
                case 0:
                case 2:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second + 2);
                    break;
                case 1:
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second);
                    break;
            }
            break;
        case 'T':
            switch (_orientation) {
                case 0:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first, pivot.second + 2);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second + 1);
                    break;
                case 1:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 2, pivot.second);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second + 1);
                    break;
                case 2:
                    (_blocks.at(0))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 2);
                    (_blocks.at(3))->move_to(pivot.first, pivot.second + 1);
                    break;
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 2, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second);
                    break;
            }
            break;
    }

    _orientation ++;
    _orientation = _orientation % 4;
}

void BlockCluster::rotate_right() {
    std::pair<int,int> pivot = _get_pivot();

    switch (_style.get()->print_block()) {
        case 'I':
            switch (_orientation) {
                case 0:
                case 2:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first, pivot.second + 2);
                    (_blocks.at(3))->move_to(pivot.first, pivot.second + 3);
                    break;
                case 1:
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 2, pivot.second);
                    (_blocks.at(3))->move_to(pivot.first + 3, pivot.second);
                    break;
            }
            break;
        case 'J':
            switch (_orientation) {
                case 0:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first, pivot.second + 2);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second + 2);
                    break;
                case 1:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 2, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second);
                    break;
                case 2:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second + 2);
                    break;
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second);
                    break;
            }
            break;
        case 'L':
            switch (_orientation) {
                case 0:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first, pivot.second + 2);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second);
                    break;
                case 1:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second + 1);
                    break;
                case 2:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second + 2);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second + 2);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second);
                    break;
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 2, pivot.second);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second + 1);
                    break;
            }
            break;
        case 'O':
            break;
        case 'S':
            switch (_orientation) {
                case 0:
                case 2:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second + 2);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second);
                    break;
                case 1:
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second + 1);
                    break;
            }
            break;
        case 'Z':
            switch (_orientation) {
                case 0:
                case 2:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second + 2);
                    break;
                case 1:
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(3))->move_to(pivot.first + 2, pivot.second);
                    break;
            }
            break;
        case 'T':
            switch (_orientation) {
                case 0:
                    (_blocks.at(0))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first + 1, pivot.second + 2);
                    (_blocks.at(3))->move_to(pivot.first, pivot.second + 1);
                    break;
                case 1:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 2, pivot.second);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second + 1);
                    break;
                case 2:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first, pivot.second + 1);
                    (_blocks.at(2))->move_to(pivot.first, pivot.second + 2);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second + 1);
                    break;
                case 3:
                    (_blocks.at(0))->move_to(pivot.first, pivot.second);
                    (_blocks.at(1))->move_to(pivot.first + 1, pivot.second);
                    (_blocks.at(2))->move_to(pivot.first + 2, pivot.second);
                    (_blocks.at(3))->move_to(pivot.first + 1, pivot.second + 1);
                    break;
            }
            break;
    }

    _orientation ++;
    _orientation = _orientation % 4;
}

bool BlockCluster::remove_blocks(const int row) {
    for (int i = _blocks.size() - 1; i >= 0; i --) {
        if (_blocks.at(i)->get_coordinates().second == row) {
            _blocks.erase(_blocks.begin() + i);
        }
    }

    return _blocks.empty();
}