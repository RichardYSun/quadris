#ifndef __DISPLAY_MANAGER_HPP__
#define __DISPLAY_MANAGER_HPP__

#include "Session.hpp"
#include "Level.hpp"
#include <iostream>

class DisplayManager {
private:
    std::shared_ptr<Session> _session;
    std::shared_ptr<Level> _level;
    //TODO: Implement GTK GUI
    //GTK::Window _window;

    //Display helper methods
    void display_level_stats(std::ostream&);
    void display_level_board(std::ostream&);
    void display_next_piece(std::ostream&);
    void display_separation_line(std::ostream&);
public:
    //Constructor
    DisplayManager(std::shared_ptr<Session>, std::shared_ptr<Level> = nullptr);
    //Displays terminal output or GTK GUI depending on the bool
    //TODO: GTK GUI implementation
    void display(bool, std::ostream&);
    //Observer update function
    //Updates member variables if necessary
    void update();
};

#endif
