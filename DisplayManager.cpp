#include <array>
#include "DisplayManager.hpp"

DisplayManager::DisplayManager(std::shared_ptr<Session> session, std::shared_ptr<Level> level) {
    _session = session;
    _level = level;
}

void DisplayManager::display_level_stats(std::ostream& sout) {
    sout << "    Level:      " << _level->get_level() << std::endl;
    sout << "    Score:      " << _level->get_score() << std::endl;
    sout << "    Hi-Score:   " << _session->get_high_score() << std::endl;
}

void DisplayManager::display_level_board(std::ostream& sout) {
    std::array<std::array<std::shared_ptr<Block>, 11>, 18> board = _level->get_full_board();
    for (int r = board.size() - 1; r >= 0 ; r --) {
        //Spacing left of board
        if (r < 9) {
            sout << " ";
        }
        sout << r + 1 << "  ";

        //Board elements
        for (int c = 0; c < board.at(r).size() ; c ++) {
            if (board.at(r).at(c) == nullptr) {
                sout << " ";
            } else {
                sout << *board.at(r).at(c);
            }
        }
        sout << std::endl;
    }
}

void DisplayManager::display_next_piece(std::ostream& sout) {
    sout << "    Next:" << std::endl;
    sout << "    ////////" << std::endl;
    sout << "    ////////" << std::endl;
}

void DisplayManager::display_separation_line(std::ostream& sout) {
    sout << "    -----------" << std::endl;
}

void DisplayManager::display(bool useTerm, std::ostream& sout) {
    if (useTerm) {
        //Display Top of board (statistics)
        this->display_level_stats(sout);
        this->display_separation_line(sout);
        this->display_level_board(sout);
        this->display_separation_line(sout);
        this->display_next_piece(sout);
    }
}

void DisplayManager::update() {
    if (_session->get_level() != _level) {
        _level = _session->get_level();
    }
}
