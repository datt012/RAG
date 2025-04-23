#include "Map.h"
#include "MapLoader.h"
#include <iostream>

Map::Map() : m_Width(0), m_Height(0) {}

Map::~Map() {}

bool Map::LoadFromFile(const std::string& filePath, SDL_Renderer* renderer) {
    // Use MapLoader to load the map
    MapLoader loader(renderer);
    std::shared_ptr<Map> loadedMap = loader.LoadMap(filePath);
    if (!loadedMap) {
        std::cerr << "Failed to load map from file: " << filePath << std::endl;
        return false;
    }

    // Copy data from the loaded map
    m_FilePath = filePath;
    m_Width = loadedMap->GetWidth();
    m_Height = loadedMap->GetHeight();
    m_TileSets = loadedMap->GetTileSets();
    m_Layers = loadedMap->GetLayers();
    m_CollisionLayer = loadedMap->GetCollisionLayer();

    return true;
}

void Map::Init() {
	// Initialize each layer
	for (auto& layer : m_Layers) {
		layer.Init();
	}
}

void Map::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
    for (auto& layer : m_Layers) {
        layer.Draw(renderer);
    }
}

void Map::Update(float deltaTime) {
    for (auto& layer : m_Layers) {
        layer.Update(deltaTime);                                                               
    }
}

void Map::AddTileSet(const TileSet& tileset) {
    m_TileSets.push_back(tileset);
}

void Map::AddLayer(const Layer& layer) {
    m_Layers.push_back(layer);
}

const std::vector<TileSet>& Map::GetTileSets() const {
    return m_TileSets;
}

const std::vector<Layer>& Map::GetLayers() const {
    return m_Layers;
}

int Map::GetWidth() const {
    return m_Width;
}

int Map::GetHeight() const {
    return m_Height;
}

void Map::SetFilePath(const std::string& filePath) {
    m_FilePath = filePath;
}

const std::string& Map::GetFilePath() const {
    return m_FilePath;
}

void Map::SetWidth(int width) {
    m_Width = width;
}

void Map::SetHeight(int height) {
    m_Height = height;
}

std::shared_ptr<Layer> Map::GetCollisionLayer() const {
	return m_CollisionLayer;
}

void Map::SetCollisionLayer(std::shared_ptr<Layer> collisionLayer) {
	m_CollisionLayer = collisionLayer;
}