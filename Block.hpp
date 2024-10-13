#ifndef __BLOCK_HPP__
#define __BLOCK_HPP__

#include "BlockStyle.hpp"
#include "BlockCluster.hpp"
#include "Result.hpp"
#include <utility>
#include <memory>

// Forward declaration
class BlockCluster;

/*
 An object holding one square piece block
 */
class Block {
private:
    //Block's coordinates on the board
    int _x, _y;
    //Pointer to the Block's Style
    std::shared_ptr<BlockStyle> _style;
public:
    //Constructor
    Block(const int, const int, std::shared_ptr<BlockStyle>);
    //Copy Constructor
    Block (const Block&);
    //Sets the block's coordinates (setter method)
    void move_to(const int, const int);
    //Returns block's coordinates (getter method)
    //Pair.first = Col, Pair.second = Row
    std::pair<int, int> get_coordinates() ;
    //Output operator overloading for printing
    friend std::ostream& operator<<(std::ostream&, const Block&);
};

#endif
