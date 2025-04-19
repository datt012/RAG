#include "Map.h"
#include "Tinyxml2/tinyxml2.h"
#include <iostream>
#include <sstream>
#include <SDL_image.h>
#include "SDL.h"
#include "GameObject/Renderer.h"
#include "Define.h"
#include "CMath.h"
#include "TextureManager.h"

Map::Map() : mapColumn(0), mapRow(0) {}

Map::~Map() {
    dsData.clear();
    dsMapPixel.clear();
    dsTileSet.clear();
}

bool Map::init(const std::string& path) {
    try {
        
        dsData.clear();
        dsMapPixel.clear();
        dsTileSet.clear();

        
        tmxFilePath = path;

        if (!loadTilesets()) {
            std::cerr << "Failed to load tilesets" << std::endl;
            return false;
        }
        if (!loadMapData()) {
            std::cerr << "Failed to load map data" << std::endl;
            return false;
        }
        if (!createMapPixel()) {
            std::cerr << "Failed to create map pixels" << std::endl;
            return false;
        }
        

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error initializing map: " << e.what() << std::endl;
        return false;
    }
}

bool Map::loadTilesets() {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(tmxFilePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load TMX file: " << tmxFilePath << std::endl;
        return false;
    }

    auto* mapElement = doc.FirstChildElement("map");
    if (!mapElement) {
        std::cerr << "TMX file does not contain <map> element." << std::endl;
        return false;
    }

    
    mapColumn = mapElement->IntAttribute("width");
    mapRow = mapElement->IntAttribute("height");

    
    std::string tmxDir = tmxFilePath.substr(0, tmxFilePath.find_last_of("\\/") + 1);

    
    for (auto* tilesetElement = mapElement->FirstChildElement("tileset");
        tilesetElement;
        tilesetElement = tilesetElement->NextSiblingElement("tileset")) {

        int firstGid = tilesetElement->IntAttribute("firstgid");
        const char* tsxRelativePath = tilesetElement->Attribute("source");
        if (!tsxRelativePath) {
            std::cerr << "Tileset <source> attribute is missing." << std::endl;
            return false;
        }

        
        std::string tsxFullPath = tmxDir + tsxRelativePath;

        
        tinyxml2::XMLDocument tsxDoc;
        if (tsxDoc.LoadFile(tsxFullPath.c_str()) != tinyxml2::XML_SUCCESS) {
            std::cerr << "Failed to load TSX file: " << tsxFullPath << std::endl;
            std::cerr << "Error: " << tsxDoc.ErrorStr() << std::endl;
            return false;
        }

        auto* tileset = tsxDoc.FirstChildElement("tileset");
        if (!tileset) {
            std::cerr << "TSX file does not contain <tileset> element." << std::endl;
            return false;
        }

        auto* imageElement = tileset->FirstChildElement("image");
        if (!imageElement) {
            std::cerr << "TSX file does not contain <image> element in tileset." << std::endl;
            return false;
        }

        
        const char* imgRelativePath = imageElement->Attribute("source");
        if (!imgRelativePath) {
            std::cerr << "Tileset image <source> attribute is missing." << std::endl;
            return false;
        }

        
        std::string tsxDir = tsxFullPath.substr(0, tsxFullPath.find_last_of("\\/") + 1);
        std::string imgFullPath = tsxDir + imgRelativePath;

        
        int tileWidth = tileset->IntAttribute("tilewidth");
        int tileHeight = tileset->IntAttribute("tileheight");
        int columns = tileset->IntAttribute("columns");
        int tileCount = tileset->IntAttribute("tilecount");

        
        SDL_Surface* surface = IMG_Load(imgFullPath.c_str());
        if (!surface) {
            std::cerr << "Failed to load tileset image: " << IMG_GetError() << std::endl;
            std::cerr << "Image path: " << imgFullPath << std::endl;
            return false;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetRenderer(), surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            return false;
        }

        
        int lastGid = firstGid + tileCount - 1;

        
        dsTileSet.emplace_back(
            columns,
            tileCount / columns,
            texture,
            tsxFullPath,
            imgFullPath,
            firstGid,
            lastGid
        );

        std::cout << "Successfully loaded tileset: " << tsxRelativePath << std::endl;
    }

    return !dsTileSet.empty();
}

bool Map::loadMapData() {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(tmxFilePath.c_str()) != tinyxml2::XML_SUCCESS) {
        return false;
    }

    auto* mapElement = doc.FirstChildElement("map");
    if (!mapElement) {
        return false;
    }

    
    mapColumn = mapElement->IntAttribute("width");
    mapRow = mapElement->IntAttribute("height");

    
    for (auto* layerElement = mapElement->FirstChildElement("layer");
        layerElement;
        layerElement = layerElement->NextSiblingElement("layer")) {

        
        auto* dataElement = layerElement->FirstChildElement("data");
        if (!dataElement) {
            continue;
        }

        
        const char* encoding = dataElement->Attribute("encoding");
        if (!encoding || std::string(encoding) != "csv") {
            std::cerr << "Unsupported encoding (only CSV is supported)." << std::endl;
            return false;
        }

        
        std::vector<int> layerData;
        layerData.reserve(mapColumn * mapRow);

        
        std::string csvData = dataElement->GetText();
        std::stringstream ss(csvData);
        std::string item;

        
        while (std::getline(ss, item, ',')) {
            
            item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
            if (!item.empty()) {
                try {
                    int tileId = std::stoi(item);
                    layerData.push_back(tileId);
                }
                catch (const std::exception&) {
                    std::cerr << "Error parsing tile ID: " << item << std::endl;
                    return false;
                }
            }
        }

       
        if (layerData.size() != static_cast<size_t>(mapColumn * mapRow)) {
            std::cerr << "Mismatch in the number of tiles in layer." << std::endl;
            return false;
        }

        
        dsData.push_back(layerData);
    }

    
    return !dsData.empty();
}


bool Map::createMapPixel() {
    for (int i = 0; i < dsData.size(); i++) {
        // Duyệt qua từng layer
        std::vector<Pixel> layer;
        for (int j = 0; j < dsData[i].size(); j++) {
            Pixel pi;
            int r = j / mapColumn;
            int c = j % mapColumn;
            Vector2 v2(c * PIXEL_WIDTH, r * PIXEL_HEIGHT);
            pi.SetPosition(v2);

            // duyệt qua danh sách các tileset để
            for (int k = 0; k < dsTileSet.size(); k++) {
                if (dsTileSet[k].firstTileSetID <= dsData[i][j] && dsData[i][j] <= dsTileSet[k].lastTileSetID) {
                    SDL_Rect srcRect = dsTileSet[k].getSrcRect(dsData[i][j]);

                    pi.pixelSrcRect = srcRect;
                        auto textureManager = std::make_shared<TextureManager>();
                        textureManager->SetTextureObj(dsTileSet[k].tileSetTexture);
                        pi.SetTexture(textureManager);
                        break;
                }
            }

            if (k == dsTileSet.size()) {
                std::cerr << "No matching tileset found for TileID: " << dsData[i][j] << std::endl;
            }
            
            layer.push_back(pi);
        }
        dsMapPixel.push_back(layer);
    }
    return !dsMapPixel.empty();
}
bool Map::render(SDL_Renderer* renderer) {
    for (int i = 0; i < dsMapPixel.size(); i++) {
        for (int j = 0; j < dsMapPixel[i].size(); j++) {
            dsMapPixel[i][j].Draw(renderer);
        }
    }
    return true;
}