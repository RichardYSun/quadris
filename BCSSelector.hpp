#ifndef __BCS_SELECTOR_HPP__
#define __BCS_SELECTOR_HPP__

#include "BlockCluster.hpp"
#include "BlockStyle.hpp"
#include "BlockClusterShape.hpp"
#include "Result.hpp"
#include <vector>
#include <string>

/*
 * BlockClusterShape selector, which either uses a sequence
 * from a file or a pseudorandom source.
 *
 * Note that this implementation uses rand() and srand(), which are global/static,
 * and therefore there may be interference when running multiple sessions
 * within the same program.
 * This program only uses 1 session, so it is not an issue, but a different PRNG
 * would need to be used if multiple sessions were needed.
 */
class BCSSelector {
private:
    //Sequence of Quadris blocks to be selected (can be empty)
    std::vector<char> _sequence;
    int index;
public:
    //Constructor
    BCSSelector();
    //Clears the sequence then re-initializes it from a sequence file
    //On success, returns true
    //On error, returns human-readable error message
    Result<bool, std::string> set_sequence_from(std::string);
    //Clears the sequence
    void clear_sequence();
    //Selects BlockClusterShape from sequence
    //If no sequence is provided, randomly select a BlockClusterShape from a vector of choices and respective weights
    Result<char, std::string> choose(std::vector<char>, std::vector<int>);
    /*
     * Seed the PRNG. Useful for getting the same output every time.
     */
    void set_seed(int);
};

#endif
