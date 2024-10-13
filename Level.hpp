#ifndef __LEVEL_HPP__
#define __LEVEL_HPP__

#include <array>
#include <vector>
#include <map>
#include <string>
#include "Action.hpp"
#include "BCSSelector.hpp"
#include "Block.hpp"
#include "BlockCluster.hpp"
#include "SessionConstants.hpp"
#include "Result.hpp"

// Forward declaration
class Session;

/*
 * The Level object.
 * It is responsible for some level specific behaviours.
 */
class Level {
    private:
        /*
         * Flag for checking if a movement occured.
         * Used for heavy block implementation.
         */
        bool _movement_occured = false;
        /*
         * Function that gets the rows in a board that are full.
         * Optional parameter indicates the row at which to start searching for full rows from, moving in the upwards position.
         */
        std::vector<int> get_full_rows(int = 0);
        /*
         * Function that checks if a BlockCluster is out of bounds.
         */
        bool out_of_bounds(std::shared_ptr<BlockCluster>);
        /*
         * Function that checks if a BlockCluster collides with any blocks on the board.
         * Useful for checking if a precomputed cluster has overlap with _board, like for rotation checking.
         */
        bool collides(std::shared_ptr<BlockCluster>);
        /*
         * Function that checks if a BlockCluster can move in a certain direction.
         * Takes an int array where element 0 is the change in x and element 1 is the change in y.
         */
        bool can_move(std::shared_ptr<BlockCluster>, std::pair<int, int>);
        /*
         * Function that shifts the blocks in a BlockCluster, if possible.
         * Takes an int array where element 0 is the change in x and element 1 is the change in y.
         */
        bool move(std::shared_ptr<BlockCluster>, std::pair<int, int>);
        /*
         * Function that checks if a BlockCluster is on top of a block in the board.
         * Optional parameter for adding to the x and y values of the blocks in the BLockCluster.
         */
        bool on_block(std::shared_ptr<BlockCluster>, std::pair<int, int> = std::make_pair(0, 0));
        /*
         * Function that checks if there are any blocks above a BlockCluster.
         * Optional parameter for adding to the x and y values of the blocks in the BLockCluster.
         */
        bool clear_above(std::shared_ptr<BlockCluster>, std::pair<int, int> = std::make_pair(0, 0));
    protected:
        /*
         * An integer from 0 to 4 inclusive that indicates the level number.
         */
        int _type_id;
        /*
         * A 2D array of the game board.
         */
        std::array<std::array<std::shared_ptr<Block>, 11>, 18> _board;
        /*
         * BlockClusters currently on the board.
         */
        std::vector<std::shared_ptr<BlockCluster>> _live_blocks;
        /*
         * BlockCluster that suggests a landing place for the current block.
         */
        std::shared_ptr<BlockCluster> _hint_block;
        /*
         * BlockCluster that is falling.
         */
        std::shared_ptr<BlockCluster> _falling_block;
        /*
         * A pointer reference to the SessionConstants object used in the session.
         */
        std::shared_ptr<SessionConstants> _constants;
        /*
         * A pointer reference to the BCSSelector used in the session.
         */
        std::shared_ptr<BCSSelector> _bcs_selector;
        /*
         * Score of the level.
         */
        int _score;
        /*
         * Probabilities of different randomly selected BlockClusters.
         */
        std::unordered_map<char, int> _weights;
        /*
         * Function that looks for any full rows, then removes them and updates the score. Returns the amount of rows that were cleared.
         * Optional parameter indicates the row at which to start searching for full rows from, moving in the upwards position.
         */
        int clear_rows(int = 0);
        /*
         * Function that runs after each BlockCluster drop.
         * Returns the amount of lines that were cleared.
         */
        int on_drop();
        /*
         * Spawn a new block of the given shape.
         * If there is currently a live block, this replaces the existing one.
         */
        void spawn_bc(char);
        /*
         * Spawn a new block, with shape determined by the BCSSelector.
         */
        void spawn_bc();
        /*
         * Spawn a new block if there is not already a falling block.
         */
        void ensure_bc();
        /*
         * Triggers at the end of each action.
         */
        void on_action_end();
    public:
         /*
         * Destructor
         */
         virtual ~Level() = 0;
         /*
         * Returns a new level object of the desired level number.
         */
        virtual std::shared_ptr<Level> pass_off(int) = 0;
        /*
         * Get this level's type ID.
         */
        int get_type_id() const;
        /*
         * Handles action commands.
         * Returns true if the action results in a game over.
         */
        bool handle_action(Action);
        /*
         * Gets the full game board, including the hint and falling block.
         */
        std::array<std::array<std::shared_ptr<Block>, 11>, 18> get_full_board();
        /*
         * Get any extra information from the session.
         * Return any value on success, otherwise some error message.
         * Raw pointer used here to prevent ownership problems.
         */
        Result<bool, std::string> update_from(Session*);
        //Getter methods for DisplayManager
        virtual int get_level() = 0;
        int get_score() { return _score; };
};

class Level2: public Level {
    public:
        ~Level2();
        Level2(std::shared_ptr<SessionConstants>, std::shared_ptr<BCSSelector>, std::array<std::array<std::shared_ptr<Block>, 11>, 18>, std::vector<std::shared_ptr<BlockCluster>>, std::shared_ptr<BlockCluster>, int);
        std::shared_ptr<Level> pass_off(int);
        int get_level() { return 2; }
};

/*
 * The Level2 class inherits from the Level1 as it is simply an adjustment in the weights.
 */
class Level1: public Level2 {
    public:
        ~Level1();
        Level1(std::shared_ptr<SessionConstants>, std::shared_ptr<BCSSelector>, std::array<std::array<std::shared_ptr<Block>, 11>, 18>, std::vector<std::shared_ptr<BlockCluster>>, std::shared_ptr<BlockCluster>, int);
        std::shared_ptr<Level> pass_off(int);
        int get_level() { return 1; }
};

class Level0: public Level2 {
    public:
        Level0();
        ~Level0();
        Level0(std::shared_ptr<SessionConstants>, std::shared_ptr<BCSSelector>, std::array<std::array<std::shared_ptr<Block>, 11>, 18>, std::vector<std::shared_ptr<BlockCluster>>, std::shared_ptr<BlockCluster>, int);
        std::shared_ptr<Level> pass_off(int);
        int get_level() { return 0; }
};

class Level3: public Level2 {
    public:
        ~Level3();
        Level3(std::shared_ptr<SessionConstants>, std::shared_ptr<BCSSelector>, std::array<std::array<std::shared_ptr<Block>, 11>, 18>, std::vector<std::shared_ptr<BlockCluster>>, std::shared_ptr<BlockCluster>, int);
        std::shared_ptr<Level> pass_off(int);
        int get_level() { return 3; }
};

class Level4: public Level3 {
    private:
        /*
         * The number of block drops that have occured since the last time a row was cleared.
         */
        int _drops_since_last_clear;
    public:
        ~Level4();
        Level4(std::shared_ptr<SessionConstants>, std::shared_ptr<BCSSelector>, std::array<std::array<std::shared_ptr<Block>, 11>, 18>, std::vector<std::shared_ptr<BlockCluster>>, std::shared_ptr<BlockCluster>, int);
        std::shared_ptr<Level> pass_off(int);
        int on_drop();
        int get_level() { return 4; }
};

#endif
