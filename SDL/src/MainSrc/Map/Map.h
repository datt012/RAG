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
    void AddTileSet(const TileSet& tileset);
    void AddLayer(const Layer& layer);
    const std::vector<TileSet>& GetTileSets() const;
    const std::vector<Layer>& GetLayers() const;
    int GetWidth() const;
    int GetHeight() const;
	std::shared_ptr<Layer> GetCollisionLayer() const;
    void SetWidth(int width);
    void SetHeight(int height);
    void SetFilePath(const std::string& filePath);
    const std::string& GetFilePath() const;
    void SetCollisionLayer(std::shared_ptr<Layer> collisionLayer);
private:
    std::string m_FilePath;                     
    int m_Width;                                
    int m_Height;                               
    std::vector<TileSet> m_TileSets;            
    std::vector<Layer> m_Layers;                
	std::shared_ptr<Layer> m_CollisionLayer; 
};