#include "map_data.h"

std::vector<Obstacle> loadMapObstacles1() {
    std::vector<Obstacle> map;

    const int tileSize = 32;
    const int rows = 19;
    const int cols = 25;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int type = 0;

            if (x == 0 || y == 0 || y == rows - 1) {
                type = 80;
            }

            if ((x == 20 && y >= 4 && y <= 13 && y != 6 && y != 7 && y != 10 && y != 11)) {
                type = 65;
            }

            if (x >= 1 && x <= 18 && (y == 4 || y == 13 )) {
                type = 65;
            }

            map.emplace_back(x * tileSize, y * tileSize, tileSize, tileSize, type);
        }
    }

    return map;
}
