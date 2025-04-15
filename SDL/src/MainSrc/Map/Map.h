#pragma once
#include "Pixel.h"
#include "TileSet.h"
#include <vector>
#include <string>
#include <stdexcept>

class Map {
private:
    int mapWidth;
    int mapHeight;
    std::vector<TileSet> dsTileSet;
    std::vector<std::vector<int>> dsData;
    std::vector<std::vector<Pixel>> dsMap;
    std::string tmxFilePath;

    bool loadTilesets();
    bool loadMapData();
    bool createMapPixels();

public:
    Map();
    ~Map();

    // Getters
    int getMapWidth() const { return mapWidth; }
    int getMapHeight() const { return mapHeight; }
    const std::vector<TileSet>& getTilesets() const { return dsTileSet; }
    const std::vector<std::vector<int>>& getMapData() const { return dsData; }
    const std::vector<std::vector<Pixel>>& getMapPixels() const { return dsMap; }

    // Main methods
    bool init(const std::string& path);
    bool render(SDL_Renderer* renderer);


};