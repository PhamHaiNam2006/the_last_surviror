#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H
#include <vector>
#include <map>
#include "obstacle.h"

class MapManager {
public:
    void loadMaps();
    void setCurrentMap(int id);
    std::vector<Obstacle>& getCurrentMap();

private:
    std::map<int, std::vector<Obstacle>> maps;
    int currentMapId = 0;
};

#endif
