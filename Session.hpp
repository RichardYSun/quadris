#ifndef __SESSION_HPP__
#define __SESSION_HPP__

#include <memory>
#include <vector>
#include "Action.hpp"
#include "BCSSelector.hpp"
#include "Level.hpp"
#include "SessionConstants.hpp"

// Forward declaration
class InputManager;

// Forward declaration
class DisplayManager;

/*
 * The session object.
 * There should be exactly one in the running program.
 * That said, preventing other developers from running multiple sessions
 * at once seems like an unnecessary restriction, so we do not enforce
 * the singleton property or put Session data in the static (global) scope.
 */
class Session {
private:
  /*
   * Highest score achieved during this session.
   */
  int _high_score;
  /*
   * Current level.
   * Level is responsible for game mechanics and internal behaviour.
   */
  std::shared_ptr<Level> _level;
  /*
   * Constants, useful for Session operations.
   */
  std::shared_ptr<SessionConstants> _constants;
  /*
   * Block cluster shape selector, used by the level.
   */
  std::shared_ptr<BCSSelector> _bcs_selector;
  /*
   * Input manager.
   * Responsible for retrieving and parsing inputs, and passing on the actions
   * to the session.
   */
  std::shared_ptr<InputManager> _input_manager;
  /*
   * Subscribed display managers.
   */
  std::vector<std::shared_ptr<DisplayManager>> _display_managers;
  /*
   * Sequence file to use for level 0.
   */
  std::string _level_0_sequence;
  /*
   * Whether this game has been told to end.
   */
  bool _queued_end;
public:
  // Constructor
  Session();
  // Redirect to InputManager::wait_for_input()
  // Would be called by the main loop
  void wait_for_input();
  /*
   * Get the current level.
   */
  std::shared_ptr<Level> get_level() const;
  // Change the level type, for a given target type ID.
  // Finds a path in the level graph and performs the appropriate sequence of pass-offs.
  // Returns true on success, false if the requested type ID is not valid or on another error.
  bool change_level(int);
  // Pass on some actions to the level, then notify the DisplayManager as needed.
  void send_actions(std::vector<Action>);
  // Send the null command (COMMAND_BEGIN, COMMAND_END)
  // Useful to "wake up" the level without actually doing anything
  void send_null();
  /*
   * Subscribe a DisplayManager if it is not already subscribed.
   * Subscribed DisplayManagers will receive updates.
   */
  void dm_subscribe(std::shared_ptr<DisplayManager>);
  /*
   * Unsubscribe a DisplayManager if it is already subscribed.
   */
  void dm_unsubscribe(std::shared_ptr<DisplayManager>);
  // Notify all DisplayManagers of an update.
  // If flag is set, also tell them to update the display.
  void dm_update(bool);
  // Tell the level to update from Session.
  void level_update();
  // Set the PRNG seed.
  void set_seed(int);
  // Set the sequence file to use for level 0.
  void set_level_0_sequence(const std::string&);
  // Get the sequence file to use for level 0.
  std::string get_level_0_sequence() const;
  // Get a pointer to the SessionConstants object.
  std::shared_ptr<SessionConstants> get_session_constants();
  // Get a pointer to the BCSSelector object.
  std::shared_ptr<BCSSelector> get_bcs_selector();
  // Time for the game to end
  void end_session();
  // Is the game set to end?
  bool is_ended();
  // Highscore getter method
  int get_high_score();
};

#endif
