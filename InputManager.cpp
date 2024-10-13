#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "Action.hpp"
#include "InputManager.hpp"
#include "Result.hpp"
#include "Session.hpp"

InputManager::InputManager() {
  std::vector<ActionType> temp_act;
  // Add each command one at a time
  temp_act.push_back(ActionType::MOVE_DOWN);
  _macro_map.emplace("down", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::MOVE_LEFT);
  _macro_map.emplace("left", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::MOVE_RIGHT);
  _macro_map.emplace("right", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::MOVE_CW);
  _macro_map.emplace("clockwise", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::MOVE_CCW);
  _macro_map.emplace("counterclockwise", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::MOVE_DROP);
  _macro_map.emplace("drop", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::LEVEL_UP);
  _macro_map.emplace("levelup", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::LEVEL_DOWN);
  _macro_map.emplace("leveldown", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::RANDOM_OFF);
  _macro_map.emplace("norandom", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::RANDOM_ON);
  _macro_map.emplace("random", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::SPAWN_I);
  _macro_map.emplace("I", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::SPAWN_J);
  _macro_map.emplace("J", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::SPAWN_L);
  _macro_map.emplace("L", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::SPAWN_S);
  _macro_map.emplace("S", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::SPAWN_Z);
  _macro_map.emplace("Z", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::SPAWN_O);
  _macro_map.emplace("O", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::SPAWN_T);
  _macro_map.emplace("T", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::LEVEL_RESET);
  _macro_map.emplace("restart", temp_act);
  temp_act.clear();
  
  temp_act.push_back(ActionType::LEVEL_HINT);
  _macro_map.emplace("hint", temp_act);
  temp_act.clear();
}

Result<std::vector<Action>, std::string>
  InputManager::map_command(const std::string& command0, const std::vector<std::string>& parameters) const {
  // try to extract multiplier prefix
  int multiplier = 1;
  std::string command = command0;
  std::istringstream mult_iss(command);
  if(mult_iss >> multiplier) {
    mult_iss >> command;
  } else {
    multiplier = 1;
  }
  // find possible matches
  std::vector<std::string> candidates;
  for(auto it = _macro_map.cbegin(); it != _macro_map.cend(); ++it) {
    // check for prefix
    if(it->first.find(command) == 0) {
      candidates.push_back(it->first);
    }
  }
  // expect exactly 1 match
  if(candidates.size() == 0) {
    return Result<std::vector<Action>, std::string>::make_error(
      "No commands match your input. Try \"help\" to see all available commands.");
  }
  if(candidates.size() > 1) {
    std::ostringstream error_oss;
    error_oss << "Multiple possible matches: ";
    for(int i = 0; i < candidates.size(); ++i) {
      if(i != 0) {
        error_oss << ", ";
      }
      error_oss << candidates[i];
    }
    error_oss << ". Please be more specific.";
    std::string error_str = error_oss.str();
    return Result<std::vector<Action>, std::string>::make_error(error_str);
  }
  std::string macro_key = candidates[0];
  // check correct number of parameters
  int expect_parameters = 0;
  const auto& macro_act = _macro_map.at(macro_key);
  for(auto it = macro_act.cbegin(); it != macro_act.cend(); ++it) {
    ActionType act = *it;
    expect_parameters += uses_parameter(act);
  }
  if(expect_parameters != parameters.size()) {
    std::ostringstream error_oss;
    error_oss << "This command expects " << expect_parameters << " parameters, but you provided "
      << parameters.size();
    std::string error_str = error_oss.str();
    return Result<std::vector<Action>, std::string>::make_error(error_str);
  }
  // All clear!
  // We can build the sequence now.
  // Due to a quirk in the specification, the begin/end tags need to be added
  // after the multiplier is applied rather than before,
  // which means using a multiplier is the only way to achieve certain inputs.
  // As an example, try entering "3left" on level 3.
  // This ultimately results in behaviour equivalent to MOVE_LEFT MOVE_LEFT MOVE_LEFT MOVE_DOWN,
  // rather than MOVE_LEFT MOVE_DOWN MOVE_LEFT MOVE_DOWN MOVE_LEFT MOVE_DOWN.
  // This sequence of actions is not possible to achieve without using a multiplier.
  std::vector<Action> core_actions;
  int parameter_i = 0;
  for(auto it = macro_act.cbegin(); it != macro_act.cend(); ++it) {
    ActionType act = *it;
    Action action;
    if(uses_parameter(act)) {
      action = Action(act, parameters[parameter_i]);
      ++parameter_i;
    } else {
      action = Action(act);
    }
    core_actions.push_back(action);
  }
  std::vector<Action> actions;
  actions.push_back(Action(ActionType::COMMAND_BEGIN));
  for(int i = 0; i < multiplier; ++i) {
    actions.insert(actions.end(), core_actions.begin(), core_actions.end());
  }
  actions.push_back(Action(ActionType::COMMAND_END));
  return Result<std::vector<Action>, std::string>::make_success(actions);
}

void InputManager::do_command(Session& session, const std::string& raw_command) {
  std::istringstream split_iss(raw_command);
  std::vector<std::string> bits;
  std::string bit;
  while(split_iss >> bit) {
    bits.push_back(bit);
  }
  // ensure the input was not empty
  if(bits.size() == 0) {
    std::cout << "Input is empty! Try \"help\" to see what commands are available." << std::endl;
  } else {
    // split off the first value as the command
    std::string command = bits[0];
    bits.erase(bits.begin());
    // check for keywords
    if(command == "help") {
      // ignore any parameters
      // sort the commands for nice presentation
      std::vector<std::string> macro_keys;
      for(auto it = _macro_map.cbegin(); it != _macro_map.cend(); ++it) {
        macro_keys.push_back(it->first);
      }
      std::sort(macro_keys.begin(), macro_keys.end());
      // print messages
      std::cout << "Keywords:\n"
        "* help - shows this message.\n"
        "* exit - exits the program.\n"
        "* refresh - force the display to update without taking any actions in the game.\n"
        "* define <new_command> <actions...> - define a new command as some sequence of actions. You can use existing commands as well to build your new command.\n"
        "* undefine <command> - removes a command from the command map. You can always get it back using the \"define\" keyword."
        "* sequence <filename> - runs the commands in the given file, one per line. Warning: recursive usages of \"sequence\" can cause the program to hang or crash.\n"
        ;
      std::cout << "Commands:\n";
      for(auto it = macro_keys.cbegin(); it != macro_keys.cend(); ++it) {
        std::cout << "* " << (*it) << " -";
        const auto& macro = _macro_map.at(*it);
        for(auto jt = macro.cbegin(); jt != macro.cend(); ++jt) {
          ActionType act = *jt;
          std::cout << " " << to_string(act);
        }
        std::cout << "\n";
      }
      std::cout << "Advanced usage:\n"
        "* Keywords only work if you enter them exactly.\n"
        "* Keywords cannot be overridden or changed.\n"
        "* Regular commands still work if you don't enter the whole word.\n"
        "* Regular commands can be mapped to multiple actions, like a macro.\n"
        ;
      std::cout << "---" << std::endl;
    } else if(command == "exit") {
      session.end_session();
    } else if(command == "refresh") {
      session.dm_update(true);
    } else if(command == "undefine") {
      if(bits.size() == 0) {
        std::cout << "No command name provided. What command do you want to undefine?" << std::endl;
      } else if(bits.size() > 1) {
        std::cout << "You can only undefine 1 command at a time." << std::endl;
      } else {
        std::string target = bits[0];
        if(!_macro_map.count(target)) {
          std::cout << "The command " << target << " does not exist." << std::endl;
        } else {
          _macro_map.erase(target);
        }
      }
    } else if(command == "define") {
      if(bits.size() == 0) {
        std::cout << "No command name provided. What command do you want to define?" << std::endl;
      } else {
        std::string target = bits[0];
        // Try to construct our new command
        std::unordered_map<std::string, std::vector<ActionType>> macro_bits = _macro_map;
        // Ensure raw actions are always accessible by inserting them after
        std::vector<ActionType> all_action_types = get_all_action_types(false);
        for(auto it = all_action_types.cbegin(); it != all_action_types.cend(); ++it) {
          ActionType act = *it;
          std::string self_key = to_string(act);
          // Insert and replace
          std::vector<ActionType> self_vec;
          self_vec.push_back(act);
          macro_bits[self_key] = self_vec;
        }
        // Build the new command
        std::vector<ActionType> new_actions;
        bool build_ok = true;
        for(int i = 1; i < bits.size(); ++i) {
          std::string key = bits[i];
          if(macro_bits.count(key)) {
            std::vector<ActionType>& blob = macro_bits[key];
            new_actions.insert(new_actions.end(), blob.begin(), blob.end());
          } else {
            build_ok = false;
            std::cout << "No command or action exists with the name \"" << key << "\".\n"
              "Valid actions are:";
            for(auto it = all_action_types.cbegin(); it != all_action_types.cend(); ++it) {
              ActionType act = *it;
              std::string self_key = to_string(act);
              std::cout << " " << self_key;
            }
            std::cout << std::endl;
            break;
          }
        }
        if(build_ok) {
          _macro_map[target] = new_actions;
        }
      }
    } else if(command == "sequence") {
      if(bits.size() == 0) {
        std::cout << "No file path provided. sequence keyword needs a file path to read from." << std::endl;
      } else if(bits.size() > 1) {
        std::cout << "Too many parameters provided. sequence keyword uses exactly 1 file path." << std::endl;
      } else {
        std::ifstream fin(bits[0]);
        if(!fin) {
          std::cout << "Unable to read file or file is empty." << std::endl;
        }
        std::string line;
        while(std::getline(fin, line)) {
          do_command(session, line);
        }
      }
    } else {
      // not a keyword
      Result<std::vector<Action>, std::string> mapped = map_command(command, bits);
      if(mapped.is_error()) {
        // something went wrong
        std::cout << mapped.get_error_value() << std::endl;
      } else {
        // valid command, pass it on
        session.send_actions(mapped.get_success_value());
      }
    }
  }
}

void InputManager::wait_for_input(Session& session) {
  std::string line;
  std::getline(std::cin, line);
  do_command(session, line);
}
