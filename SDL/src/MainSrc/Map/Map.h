#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Layer.h"
#include "TileSet.h"
#include "BaseObject.h"
#include <SDL.h>

class Map : public BaseObject {
public:
    Map();
    ~Map();

    bool LoadFromFile(const std::string& filePath, SDL_Renderer* renderer);

    void Init() override;
    void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
    void Update(float deltaTime) override;

    // Add tilesets and layers
    void AddTileSet(const TileSet& tileset);
    void AddLayer(const Layer& layer);

    // Getters
    const std::vector<TileSet>& GetTileSets() const;
    const std::vector<Layer>& GetLayers() const;
    int GetWidth() const;
    int GetHeight() const;

    // Setters
    void SetWidth(int width);
    void SetHeight(int height);
    void SetFilePath(const std::string& filePath);
    const std::string& GetFilePath() const;

private:
    std::string m_FilePath;               // Path to the map file
    int m_Width;                          // Map width in tiles
    int m_Height;                         // Map height in tiles
    std::vector<TileSet> m_TileSets;      // List of tilesets
    std::vector<Layer> m_Layers;          // List of layers
};