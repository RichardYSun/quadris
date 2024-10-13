#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>
#include "Action.hpp"
#include "BCSSelector.hpp"
#include "DisplayManager.hpp"
#include "InputManager.hpp"
#include "Level.hpp"
#include "Session.hpp"
#include "SessionConstants.hpp"

Session::Session() {
  _high_score = 0;
  _constants = std::make_shared<SessionConstants>();
  _bcs_selector = std::make_shared<BCSSelector>();
  _input_manager = std::make_shared<InputManager>();
  _level = std::shared_ptr<Level>(new Level0());
  _level_0_sequence = "sequence.txt";
  _queued_end = false;
  level_update();
}

void Session::wait_for_input() {
  _input_manager->wait_for_input(*this);
}

std::shared_ptr<Level> Session::get_level() const {
  return _level;
}

bool Session::change_level(int target_id) {
  int old_id = _level->get_type_id();
  // If same level, do nothing
  if(old_id == target_id)return true;
  // Get the level graph
  const auto& level_graph = _constants->get_level_graph();
  // If the target ID does not exist, this is a no-op
  if(!level_graph.count(target_id))return false;
  // Find a path in the level graph
  std::unordered_map<int, int> bwedge;
  std::deque<std::pair<int, int>> nextverts;
  nextverts.push_back(std::make_pair(old_id, old_id));
  while(!nextverts.empty()) {
    int cvert, lvert;
    std::tie(cvert, lvert) = nextverts.front();
    nextverts.pop_front();
    if(!bwedge.count(cvert)) {
      bwedge.emplace(cvert, lvert);
      const auto& neighbours = level_graph.at(cvert);
      for(auto it = neighbours.cbegin(); it != neighbours.cend(); ++it) {
        int nvert = *it;
        nextverts.push_back(std::make_pair(nvert, cvert));
      }
    }
  }
  std::vector<int> trace;
  int curr = target_id;
  while(curr != old_id) {
    trace.push_back(curr);
    curr = bwedge[curr];
  }
  // Perform sequence of pass-offs
  while(!trace.empty()) {
    int trans_id = trace.back();
    trace.pop_back();
    _level = _level->pass_off(trans_id);
    // Update info from session
    level_update();
  }
  return true;
}

void Session::send_actions(std::vector<Action> actions) {
  bool force_update = false;
  for(auto it = actions.begin(); it != actions.end(); ++it) {
    if(force_update) {
      dm_update(true);
    }
    force_update = false;
    const Action& act = *it;
    if(act.get_type() == ActionType::LEVEL_UP) {
      int target_id = _level->get_type_id() + 1;
      change_level(target_id);
    } else if(act.get_type() == ActionType::LEVEL_DOWN) {
      int target_id = _level->get_type_id() - 1;
      change_level(target_id);
    } else {
      force_update = _level->handle_action(act);
    }
    _high_score = std::max(_high_score, _level->get_score());
  }
  dm_update(true);
}

void Session::send_null() {
  std::vector<Action> actions;
  actions.push_back(Action(ActionType::COMMAND_BEGIN));
  actions.push_back(Action(ActionType::COMMAND_END));
  send_actions(actions);
}

void Session::dm_subscribe(std::shared_ptr<DisplayManager> dm) {
  // check if we already have it
  for(int i = _display_managers.size() - 1; i >= 0; --i) {
    if(dm == _display_managers[i])return;
  }
  // add it
  _display_managers.push_back(dm);
}

void Session::dm_unsubscribe(std::shared_ptr<DisplayManager> dm) {
  // remove all copies
  for(int i = _display_managers.size() - 1; i >= 0; --i) {
    if(dm == _display_managers[i]) {
      _display_managers.erase(_display_managers.begin() + i);
    }
  }
}

void Session::dm_update(bool force_display) {
  for(auto it = _display_managers.begin(); it != _display_managers.end(); ++it) {
    DisplayManager& idm = **it;
    idm.update();
  }
  if(force_display) {
    for(auto it = _display_managers.begin(); it != _display_managers.end(); ++it) {
      DisplayManager& idm = **it;
      idm.display(true, std::cout);
    }
  }
}

void Session::level_update() {
  _level->update_from(this);
}

void Session::set_seed(int seed) {
  _bcs_selector->set_seed(seed);
}

void Session::set_level_0_sequence(const std::string& seq) {
  _level_0_sequence = seq;
}

std::string Session::get_level_0_sequence() const {
  return _level_0_sequence;
}

std::shared_ptr<SessionConstants> Session::get_session_constants() {
  return _constants;
}

std::shared_ptr<BCSSelector> Session::get_bcs_selector() {
  return _bcs_selector;
}

void Session::end_session() {
  _queued_end = true;
}

bool Session::is_ended() {
  return _queued_end;
}

int Session::get_high_score() {
  return _high_score;
}
