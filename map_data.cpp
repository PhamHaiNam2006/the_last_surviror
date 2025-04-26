#include "map_data.h"

std::vector<Obstacle> loadMapObstacles1() {
    std::vector<Obstacle> map;

    const int tileSize = 32;
    const int rows = 19;
    const int cols = 25;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int type = 0;

            if (x == 0 || y == 0 || (x == cols - 1 && (y < 8 || y > 10)) || y == rows - 1) {
                type = 80;
            }

            if ((x == 4 && y == 4) || (x == 10 && y == 5)) {
                type = 65;
            }

            map.emplace_back(x * tileSize, y * tileSize, tileSize, tileSize, type);
        }
    }

    return map;
}

std::vector<Obstacle> loadMapObstacles2() {
    std::vector<Obstacle> map;
    const int tileSize = 32;
    const int rows = 19;
    const int cols = 25;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int type = 0;
            if (y == 0 || y == rows - 1) {
                type = 80;
            }
            if ((x == 5 && y == 8) || (x == 15 && y == 12)) {
                type = 65;
            }
            if (x == cols - 1 && y != 9) {
                type = 80;
            }
            map.emplace_back(x * tileSize, y * tileSize, tileSize, tileSize, type);
        }
    }
    return map;
}
