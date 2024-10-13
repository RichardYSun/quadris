#include "Action.hpp"

bool uses_parameter(const ActionType& act) {
  return act == ActionType::RANDOM_OFF;
}

std::string to_string(const ActionType& act) {
  // generated offline using regex
  if(act == ActionType::COMMAND_BEGIN) {return "COMMAND_BEGIN";}
  if(act == ActionType::COMMAND_END) {return "COMMAND_END";}
  if(act == ActionType::MOVE_DOWN) {return "MOVE_DOWN";}
  if(act == ActionType::MOVE_LEFT) {return "MOVE_LEFT";}
  if(act == ActionType::MOVE_RIGHT) {return "MOVE_RIGHT";}
  if(act == ActionType::MOVE_CW) {return "MOVE_CW";}
  if(act == ActionType::MOVE_CCW) {return "MOVE_CCW";}
  if(act == ActionType::MOVE_DROP) {return "MOVE_DROP";}
  if(act == ActionType::LEVEL_DOWN) {return "LEVEL_DOWN";}
  if(act == ActionType::LEVEL_UP) {return "LEVEL_UP";}
  if(act == ActionType::LEVEL_RESET) {return "LEVEL_RESET";}
  if(act == ActionType::LEVEL_HINT) {return "LEVEL_HINT";}
  if(act == ActionType::RANDOM_OFF) {return "RANDOM_OFF";}
  if(act == ActionType::RANDOM_ON) {return "RANDOM_ON";}
  if(act == ActionType::SPAWN_I) {return "SPAWN_I";}
  if(act == ActionType::SPAWN_J) {return "SPAWN_J";}
  if(act == ActionType::SPAWN_L) {return "SPAWN_L";}
  if(act == ActionType::SPAWN_S) {return "SPAWN_S";}
  if(act == ActionType::SPAWN_Z) {return "SPAWN_Z";}
  if(act == ActionType::SPAWN_O) {return "SPAWN_O";}
  if(act == ActionType::SPAWN_T) {return "SPAWN_T";}
  return "UNDEFINED";
}

std::vector<ActionType> get_all_action_types(bool also_undefined) {
  std::vector<ActionType> result;
  if(also_undefined) {
    result.push_back(ActionType::UNDEFINED);
  }
  result.push_back(ActionType::COMMAND_BEGIN);
  result.push_back(ActionType::COMMAND_END);
  result.push_back(ActionType::MOVE_DOWN);
  result.push_back(ActionType::MOVE_LEFT);
  result.push_back(ActionType::MOVE_RIGHT);
  result.push_back(ActionType::MOVE_CW);
  result.push_back(ActionType::MOVE_CCW);
  result.push_back(ActionType::MOVE_DROP);
  result.push_back(ActionType::LEVEL_DOWN);
  result.push_back(ActionType::LEVEL_UP);
  result.push_back(ActionType::LEVEL_RESET);
  result.push_back(ActionType::LEVEL_HINT);
  result.push_back(ActionType::RANDOM_OFF);
  result.push_back(ActionType::RANDOM_ON);
  result.push_back(ActionType::SPAWN_I);
  result.push_back(ActionType::SPAWN_J);
  result.push_back(ActionType::SPAWN_L);
  result.push_back(ActionType::SPAWN_S);
  result.push_back(ActionType::SPAWN_Z);
  result.push_back(ActionType::SPAWN_O);
  result.push_back(ActionType::SPAWN_T);
  return result;
}

Action::Action(ActionType act) : _type{act}, _parameter{} {
}

Action::Action(ActionType act, std::optional<std::string> parameter)
  : _type{act}, _parameter{parameter} {
}

ActionType Action::get_type() const {
  return _type;
}

void Action::set_type(ActionType act) {
  _type = act;
}

std::optional<std::string> Action::get_parameter() const {
  return _parameter;
}

void Action::set_parameter(std::optional<std::string> parameter) {
  _parameter = parameter;
}

bool operator == (const Action& lhs, const Action& rhs) {
  return lhs.get_type() == rhs.get_type()
    && lhs.get_parameter() == rhs.get_parameter();
}

bool operator != (const Action& lhs, const Action& rhs) {
  return !(lhs == rhs);
}
