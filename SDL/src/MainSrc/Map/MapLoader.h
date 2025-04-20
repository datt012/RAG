#pragma once
#include <string>
#include <memory>
#include "Map.h"
#include <SDL.h>
#include "Tinyxml2/tinyxml2.h"

class MapLoader {
public:
    MapLoader(SDL_Renderer* renderer);
    ~MapLoader();

    std::shared_ptr<Map> LoadMap(const std::string& filePath);

private:
    SDL_Renderer* m_Renderer;

    bool LoadTileSets(tinyxml2::XMLElement* mapElement, Map& map);
    bool LoadLayers(tinyxml2::XMLElement* mapElement, Map& map);
};
