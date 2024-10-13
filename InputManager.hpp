#ifndef __INPUT_MANAGER_HPP__
#define __INPUT_MANAGER_HPP__

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "Action.hpp"
#include "Result.hpp"
#include "Session.hpp"

/*
 * The input manager. Its roles are:
 * - Detecting user input
 * - Translating user input to internal actions
 * - Performing any appropriate behaviours or triggers for those actions
 * Since GUI is not supported right now, only text input is handled.
 */
class InputManager {
private:
  /*
   * Level 1 macro maps.
   * Maps command names (not keywords) to sequences of actions.
   * Keywords have special handling.
   */
  std::unordered_map<std::string, std::vector<ActionType>> _macro_map;
public:
  // Constructor
  InputManager();
  /*
   * Attempt to map a command to a sequence of actions.
   * This function handles:
   * - Matching the command in the macro map
   * - Extracting the prefix multiplier, if any, and applying it
   * - Attaching the string parameters (second parameter of this function) to actions with a parameter
   * - Surrounding the real actions by a COMMAND_BEGIN and COMMAND_END
   * In case of error, this returns a human-readable error message.
   */
  Result<std::vector<Action>, std::string> map_command(const std::string&, const std::vector<std::string>&) const;
  /*
   * Do the behaviour for this command.
   * This function handles:
   * - Error message in case of empty input.
   * - Special behaviour for keywords.
   * - Separating command from extra string parameters.
   * - Determining the sequence of actions using map_command()
   * - Sending the actions to the session
   */
  void do_command(Session&, const std::string&);
  /*
   * Wait for input, get input, and then do something via do_command().
   * Takes the session as a parameter so we don't need to store a reference to it.
   */
  void wait_for_input(Session&);
};

#endif
