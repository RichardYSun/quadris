#include "BlockClusterShape.hpp"

BlockClusterShape::BlockClusterShape(const char type, const bool hint) {
    switch (type) {
        case 'I':
            _coordinates.push_back(std::pair<int, int>(0, 14));
            _coordinates.push_back(std::pair<int, int>(1, 14));
            _coordinates.push_back(std::pair<int, int>(2, 14));
            _coordinates.push_back(std::pair<int, int>(3, 14));
            break;
        case 'J':
            _coordinates.push_back(std::pair<int, int>(0, 14));
            _coordinates.push_back(std::pair<int, int>(1, 14));
            _coordinates.push_back(std::pair<int, int>(2, 14));
            _coordinates.push_back(std::pair<int, int>(0, 15));

            break;
        case 'L':
            _coordinates.push_back(std::pair<int, int>(0, 14));
            _coordinates.push_back(std::pair<int, int>(1, 14));
            _coordinates.push_back(std::pair<int, int>(2, 14));
            _coordinates.push_back(std::pair<int, int>(2, 15));

            break;
        case 'O':
            _coordinates.push_back(std::pair<int, int>(0, 14));
            _coordinates.push_back(std::pair<int, int>(1, 14));
            _coordinates.push_back(std::pair<int, int>(0, 15));
            _coordinates.push_back(std::pair<int, int>(1, 15));

            break;
        case 'S':
            _coordinates.push_back(std::pair<int, int>(0, 14));
            _coordinates.push_back(std::pair<int, int>(1, 14));
            _coordinates.push_back(std::pair<int, int>(1, 15));
            _coordinates.push_back(std::pair<int, int>(2, 15));

            break;
        case 'Z':
            _coordinates.push_back(std::pair<int, int>(0, 15));
            _coordinates.push_back(std::pair<int, int>(1, 15));
            _coordinates.push_back(std::pair<int, int>(1, 14));
            _coordinates.push_back(std::pair<int, int>(2, 14));

            break;
        case 'T':
            _coordinates.push_back(std::pair<int, int>(0, 15));
            _coordinates.push_back(std::pair<int, int>(1, 15));
            _coordinates.push_back(std::pair<int, int>(1, 14));
            _coordinates.push_back(std::pair<int, int>(2, 15));

            break;
    }
    _name = type;
    _style = std::shared_ptr<BlockStyle>(new BlockStyle(type, hint));
}

BlockCluster* BlockClusterShape::create_cluster(int removal_score, int level) {
    return new BlockCluster(_coordinates, _style, removal_score, level);
}