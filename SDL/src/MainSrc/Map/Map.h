#pragma once
#include "Pixel.h"
#include "TileSet.h"
#include <vector>
#include <string>
#include <stdexcept>

class Map {
private:
    int mapColumn;
    int mapRow;
    std::vector<TileSet> dsTileSet;
    std::vector<std::vector<int>> dsData;
    std::vector<std::string > dsMapPixelType; // type các pixel của layer trên cùng 
    std::vector<std::vector<Pixel>> dsMapPixel;
    std::string tmxFilePath;

    bool loadTilesets();
    bool loadMapData();
    bool createMapPixel();

public:
    Map();
    ~Map();

    
    int getMapColumn() const { return mapColumn; }
    int getMapRow() const { return mapRow; }
    const std::vector<TileSet>& getTilesets() const { return dsTileSet; }
    const std::vector<std::vector<int>>& getMapData() const { return dsData; }
    const std::vector<std::vector<Pixel>>& getMapPixel() const { return dsMapPixel; }
    
    
    bool init(const std::string& path);
    bool render(SDL_Renderer* renderer);
};