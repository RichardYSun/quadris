#ifndef __BLOCK_CLUSTER_SHAPE_HPP__
#define __BLOCK_CLUSTER_SHAPE_HPP__

#include "BlockCluster.hpp"
#include "BlockStyle.hpp"
//#include <iostream>

class BlockClusterShape {
private:
    char _name;
    std::vector<std::pair<int,int>> _coordinates;
    std::shared_ptr<BlockStyle> _style;
public:
    //Constructor
    //Creates the default oriented BlockCluster with correct style
    BlockClusterShape(const char type, const bool hint = false);
    //Creates BlockCluster given removal score and current level
    BlockCluster* create_cluster(int, int);
    //Input method for reading in the BlockClusterShape
    //Removing for now since I'm not sure we'll be using this
    //friend std::istream& operator>>(std::istream &, BlockClusterShape &);
    //Getter methods
    std::shared_ptr<BlockStyle> get_style() const { return _style; };
};

#endif
