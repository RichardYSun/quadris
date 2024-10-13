#include "BCSSelector.hpp"
#include "Result.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string>

BCSSelector::BCSSelector() {
    index = 0;
}

Result<bool, std::string> BCSSelector::set_sequence_from(std::string file_name) {
    _sequence.clear();
    index = 0;
    std::ifstream file(file_name);

    char c;

    while (file >> c) {
        //Adds Block Type to sequence if it is valid and is not already in the sequence
        if (c == 'I' || c == 'J' || c == 'O' || c == 'S' || c == 'Z' || c == 'T' || c == 'L') {
            if (find(_sequence.begin(), _sequence.end(), c) == _sequence.end()) {
                _sequence.push_back(c);
                continue;
            }
        }

        file.close();
        return Result<bool, std::string>::make_error("Error with reading in Sequence input");
    }

    file.close();
    return Result<bool, std::string>::make_success(true);
}

void BCSSelector::clear_sequence() {
    _sequence.clear();
}

Result<char, std::string> BCSSelector::choose(std::vector<char> types, std::vector<int> weights) {
    if (!_sequence.empty()) {
        char c = _sequence.at(index);
        index = (index+1) % _sequence.size();
        return Result<char, std::string>::make_success(c);
    } else {
        int sum = 0;
        for (int i = 0; i < weights.size(); i ++) {
            sum += weights.at(i);
        }

        int selected_index = rand() % sum;
        int summed_weight = 0;
        for (int i = 0; i < weights.size(); i ++) {
            summed_weight += weights[i];
            if (selected_index < summed_weight) {
                return Result<char, std::string>::make_success(types.at(i));
            }
        }
    }

    return Result<char, std::string>::make_error("Error with selecting Block Cluster");
}

void BCSSelector::set_seed(int new_seed) {
  srand(new_seed);
}
