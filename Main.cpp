#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include "DisplayManager.hpp"
#include "Session.hpp"

#define _ARG_BOUNDS_CHECK if(argi >= argc) {std::cout << "Missing parameter(s). Try -help to see usage info." << std::endl;}

int main(int argc, char** argv) {
  // Read in command line arguments
  bool text_only = true; // TODO implement graphics
  int seed = 1;
  std::optional<std::string> l0sequence = "sequence.txt";
  int starting_level = 0;
  for(int argi = 1; argi < argc; ++argi) {
    std::string arg(argv[argi]);
    if(arg == "-text") {
      text_only = true;
    } else if(arg == "-seed") {
      ++argi;
      _ARG_BOUNDS_CHECK;
      std::string xarg(argv[argi]);
      seed = std::stoi(xarg);
    } else if(arg == "-scriptfile") {
      ++argi;
      _ARG_BOUNDS_CHECK;
      std::string xarg(argv[argi]);
      l0sequence = xarg;
    } else if(arg == "-startlevel") {
      ++argi;
      _ARG_BOUNDS_CHECK;
      std::string xarg(argv[argi]);
      starting_level = std::stoi(xarg);
    } else {
      std::cout << "Usage:\n"
        "\t-text\tRuns the program in text-only mode.\n"
        "\t-seed <seed>\tSets the PRNG seed.\n"
        "\t-scriptfile <path>\tSets the sequence file to use in level 0\n"
        "\t-startlevel <num>\tSets the starting level."
        << std::endl;
      exit(0);
    }
  }
  // Set up the game
  std::shared_ptr<Session> session = std::make_shared<Session>();
  std::shared_ptr<DisplayManager> dm = std::make_shared<DisplayManager>(DisplayManager(session));
  session->dm_subscribe(dm);
  session->set_seed(seed);
  if(l0sequence) {
    session->set_level_0_sequence(*l0sequence);
  }
  session->level_update();
  bool trans_success = session->change_level(starting_level);
  if(!trans_success) {
    std::cout << "Requested level does not exist." << std::endl;
    exit(1);
  }
  // Force refresh
  session->send_null();
  // Main loop
  while(!session->is_ended()) {
    session->wait_for_input();
  }
  return 0;
}
