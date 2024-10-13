#ifndef __BLOCK_CLUSTER_HPP__
#define __BLOCK_CLUSTER_HPP__

#include <utility>
#include <vector>
#include <memory>

//Forward Declaration
class Block;
class BlockStyle;

class BlockCluster {
private:
    //Vector of individual blocks that make up a block cluster
    std::vector<std::shared_ptr<Block>> _blocks;
    //Pointer to this cluster's BlockStyle
    std::shared_ptr<BlockStyle> _style;
    //Orientation (0 is default, 1 is defaulted rotate ccw, 2 ..., up to 3, cycles back to 0)
    int _orientation;
    //Bonus score on removal
    int _removal_score;
    //Level Spawned in
    int _level_spawned;    
    //Helper methods for rotation
    //Return coordinates of pivot position (lower left corner)
    std::pair<int,int> _get_pivot();
public:
    //Constructor
    //Input is a vector of 4 blocks' coordinates, the cluster's style, and removal score
    BlockCluster(const std::vector<std::pair<int, int>>, std::shared_ptr<BlockStyle>, int, int);
    //Rotates the block cluster
    //Implementation to check valid rotation will be implmented later.
    void rotate_left();
    void rotate_right();
    //Removes blocks in given row
    //Deallocates the blocks in the row
    //Returns bool for if all blocks are removed. Cluster should be deallcoated if true
    bool remove_blocks(const int);
    //Returns _removal_score
    int get_removal_score() const { return _removal_score; };
    //Getter for style
    std::shared_ptr<BlockStyle> get_style() { return _style; };
    //Returns vector of blocks
    std::vector<std::shared_ptr<Block>> get_blocks() { return _blocks; };
    //Getter/Seet for _level_spawned
    int get_level_spawned() { return _level_spawned; };
    //TODO: Ideally, this would be initialized in the constructor
    void set_level_spawned(const int l) { _level_spawned = l; };
    //TODO: Need additional methods for GTK GUI
};

#endif
