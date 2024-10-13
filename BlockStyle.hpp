#ifndef __BLOCK_STYLE_HPP__
#define __BLOCK_STYLE_HPP__

#include <iostream>

class BlockStyle {
private:
    //String indicating the type of block
    char _block_type;
    //Bool for hint block
    bool _hint;
public:
    //Constructor
    BlockStyle(const char type, const bool hint = false) { _block_type = type; _hint = hint; };
    //Method for terminal graphics
    //Returns char for printing on terminal
    char print_block() const { return _hint ? '?' : _block_type; };
    //TODO: Need additional methods for GTK GUI
};

#endif
