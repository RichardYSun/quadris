#ifndef __ACTION_HPP__
#define __ACTION_HPP__

#include <functional>
#include <optional>
#include <string>
#include <vector>
#include "Utils.hpp"

enum ActionType {
  UNDEFINED = 0,
  COMMAND_BEGIN,
  COMMAND_END,
  MOVE_DOWN,//3
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_CW,
  MOVE_CCW,
  MOVE_DROP,//8
  LEVEL_DOWN,
  LEVEL_UP,
  LEVEL_RESET,
  LEVEL_HINT,
  RANDOM_OFF,
  RANDOM_ON,
  SPAWN_I,//15
  SPAWN_J,
  SPAWN_L,
  SPAWN_S,
  SPAWN_Z,
  SPAWN_O,
  SPAWN_T//21
};

/*
 * Does this action type use a string parameter?
 *
 * Current list of action types that use a string parameter:
 * - RANDOM_OFF
 */
bool uses_parameter(const ActionType&);

/*
 * Get the string name of an action type, according to the enum.
 */
std::string to_string(const ActionType&);

/*
 * Get all action types, in the order that they appear in the enum.
 * Flag to include UNDEFINED.
 */
std::vector<ActionType> get_all_action_types(bool);

class Action {
private:
  // The type ID for this action, indicating what kind of action it is
  ActionType _type;
  // Some action types have an additional string parameter
  std::optional<std::string> _parameter;
public:
  // Constructor that initializes nothing
  Action() = default;
  // Constructor for types with no string parameter
  Action(ActionType);
  // Constructor for types with a string parameter
  Action(ActionType, std::optional<std::string>);
  // No need to manually define:
  // * Destructor
  // * Copy/move constructor
  // * Assignment operator
  // Getters and setters
  ActionType get_type() const;
  void set_type(ActionType);
  std::optional<std::string> get_parameter() const;
  void set_parameter(std::optional<std::string>);
};

// Comparison operators
bool operator == (const Action&, const Action&);
bool operator != (const Action&, const Action&);

// Hash function
// to allow hash-based data structures to use these classes as keys
namespace std {

  // Hash for ActionType
  template <>
  struct hash<ActionType>
  {
    std::size_t operator()(const ActionType& k) const
    {
      return enum_to_size_t(k);
    }
  };

  // Hash for Action
  template <>
  struct hash<Action>
  {
    std::size_t operator()(const Action& k) const
    {
      size_t h = std::hash<ActionType>{}(k.get_type());
      h *= 204509;
      h += std::hash<std::optional<std::string>>{}(k.get_parameter());
      h ^= h >> 16;
      return h;
    }
  };

}

#endif
