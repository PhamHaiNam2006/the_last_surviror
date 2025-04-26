#include "map_manager.h"
#include "map_data.h"

void MapManager::loadMaps() {
    maps[0] = loadMapObstacles1();
    maps[1] = loadMapObstacles2();
}

void MapManager::setCurrentMap(int id) {
    if (maps.find(id) != maps.end()) {
        currentMapId = id;
    }
}

std::vector<Obstacle>& MapManager::getCurrentMap() {
    return maps[currentMapId];
}
