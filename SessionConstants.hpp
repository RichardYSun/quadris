#ifndef __SESSION_CONSTANTS_HPP__
#define __SESSION_CONSTANTS_HPP__

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "BlockClusterShape.hpp"

class SessionConstants {
private:
  /*
   * Table of all known block cluster shapes, mapped by their name.
   */
  std::unordered_map<char, std::shared_ptr<BlockClusterShape>> _bcs_table;
  /*
   * A graph on all the level types, represented by the integer type ID.
   * Transitions between levels can occur where there is an edge in this graph.
   * We call these transitions pass-offs.
   * This system reduces the number of direct transitions that need to be coded for,
   * and reduces the implementation complexity for those transitions.
   */
  std::unordered_map<int, std::unordered_set<int>> _level_graph;
public:
  // Constructor
  SessionConstants();
  // Getters
  const std::unordered_map<char, std::shared_ptr<BlockClusterShape>>& get_bcs_table() const;
  const std::unordered_map<int, std::unordered_set<int>>& get_level_graph() const;
};

#endif
