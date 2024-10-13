#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "BlockClusterShape.hpp"
#include "SessionConstants.hpp"

#define _SC_BCS_TABLE_ADD_DEFAULT(cc) _bcs_table.emplace(cc, std::make_shared<BlockClusterShape>(cc));
#define _SC_LEVEL_GRAPH_ADD_VERTEX(i) _level_graph.emplace(i, std::unordered_set<int>());
#define _SC_LEVEL_GRAPH_ADD_EDGE(i, j) _level_graph[i].insert(j);_level_graph[j].insert(i);

SessionConstants::SessionConstants() {
  // Build BCS table
  _SC_BCS_TABLE_ADD_DEFAULT('I');
  _SC_BCS_TABLE_ADD_DEFAULT('J');
  _SC_BCS_TABLE_ADD_DEFAULT('L');
  _SC_BCS_TABLE_ADD_DEFAULT('O');
  _SC_BCS_TABLE_ADD_DEFAULT('S');
  _SC_BCS_TABLE_ADD_DEFAULT('Z');
  _SC_BCS_TABLE_ADD_DEFAULT('T');
  _SC_BCS_TABLE_ADD_DEFAULT('.');
  // Build level graph
  _SC_LEVEL_GRAPH_ADD_VERTEX(0);
  _SC_LEVEL_GRAPH_ADD_VERTEX(1);
  _SC_LEVEL_GRAPH_ADD_VERTEX(2);
  _SC_LEVEL_GRAPH_ADD_VERTEX(3);
  _SC_LEVEL_GRAPH_ADD_VERTEX(4);
  _SC_LEVEL_GRAPH_ADD_EDGE(0, 2);
  _SC_LEVEL_GRAPH_ADD_EDGE(1, 2);
  _SC_LEVEL_GRAPH_ADD_EDGE(2, 3);
  _SC_LEVEL_GRAPH_ADD_EDGE(3, 4);
}

const std::unordered_map<char, std::shared_ptr<BlockClusterShape>>& SessionConstants::get_bcs_table() const {
  return _bcs_table;
}

const std::unordered_map<int, std::unordered_set<int>>& SessionConstants::get_level_graph() const {
  return _level_graph;
}
