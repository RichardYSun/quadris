#include "Block.hpp"
#include <utility>

Block::Block(const int x, const int y, std::shared_ptr<BlockStyle> style) {
    _x = x;
    _y = y;
    _style = style;
}

Block::Block(const Block& b) {
    _x = b._x;
    _y = b._y;
    _style = b._style;
}

void Block::move_to(const int x, const int y) {
    _x = x;
    _y = y;
}

std::pair<int, int> Block::get_coordinates() {
    return std::make_pair(_x, _y);
}

std::ostream& operator<<(std::ostream& sout, const Block& block) {
    sout << block._style->print_block();
    return sout;
}
