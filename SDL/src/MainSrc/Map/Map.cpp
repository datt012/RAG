#include "Map.h"
#include "Tinyxml2/tinyxml2.h"
#include <iostream>
#include <sstream>
#include "stdio.h"
#include <SDL_image.h>
#include "SDL.h"
#include "GameObject/Renderer.h"


Map::Map() : mapWidth(0), mapHeight(0) {
   
}

Map::~Map() {
    
    dsData.clear();
    dsMap.clear();
    dsTileSet.clear();
}

bool Map::init(const std::string& path) {
    try {
        
        dsData.clear();
        dsMap.clear();
        dsTileSet.clear();

        tmxFilePath = path;

        if (!loadTilesets()) {
            std::cerr << "Failed to load tilesets" << std::endl;
            return false;
        }
        else {
            printf("Success\n");
            printf("%d ", dsTileSet.size());
            // In thông tin chi tiết của từng tileset
            /*for (size_t i = 0; i < dsTileSet.size(); i++) {
                printf("\n[%s] Tileset #%zu Information:\n", "2025-04-13 12:52:26", i + 1);
                printf("├── Grid Size: %d columns x %d rows\n", dsTileSet[i].tileSetWidth, dsTileSet[i].tileSetHeight);
                printf("├── Tile Size: %d x %d pixels\n", dsTileSet[i].pixelWidth, dsTileSet[i].pixelHeight);
                printf("├── ID Range: %d to %d (Total: %d tiles)\n",
                    dsTileSet[i].firstTileSetID,
                    dsTileSet[i].lastTileSetID,
                    dsTileSet[i].lastTileSetID - dsTileSet[i].firstTileSetID + 1);
                printf("├── TSX File: %s\n", dsTileSet[i].tsxFilePath.c_str());
                printf("└── Image File: %s\n", dsTileSet[i].imgFilePath.c_str());
            }*/
        }

       /* if (!loadMapData()) {
            std::cerr << "Failed to load map data" << std::endl;
            return false;
        }

        if (!createMapPixels()) {
            std::cerr << "Failed to create map pixels" << std::endl;
            return false;
        }*/

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
    if (!mapElement) return false;

    mapWidth = mapElement->IntAttribute("width");
    mapHeight = mapElement->IntAttribute("height");

    // Get the directory path of TMX file
    std::string tmxDir = tmxFilePath.substr(0, tmxFilePath.find_last_of("\\/") + 1);
    std::cout << "TMX directory: " << tmxDir << std::endl;

    for (auto* tilesetElement = mapElement->FirstChildElement("tileset");
        tilesetElement;
        tilesetElement = tilesetElement->NextSiblingElement("tileset")) {

        int firstGid = tilesetElement->IntAttribute("firstgid");
        std::string tsxRelativePath = tilesetElement->Attribute("source");

        // Combine TMX directory with TSX path
        std::string tsxFullPath = tmxDir + tsxRelativePath;
        std::cout << "Loading TSX file: " << tsxFullPath << std::endl;

        // Load TSX file
        tinyxml2::XMLDocument tsxDoc;
        if (tsxDoc.LoadFile(tsxFullPath.c_str()) != tinyxml2::XML_SUCCESS) {
            std::cerr << "Failed to load TSX file: " << tsxFullPath << std::endl;
            std::cerr << "Error: " << tsxDoc.ErrorStr() << std::endl;
            return false;
        }

        auto* tileset = tsxDoc.FirstChildElement("tileset");
        if (!tileset) return false;

        auto* imageElement = tileset->FirstChildElement("image");
        if (!imageElement) return false;

        // Get image path relative to TSX file
        std::string imgRelativePath = imageElement->Attribute("source");
        // Get TSX directory
        std::string tsxDir = tsxFullPath.substr(0, tsxFullPath.find_last_of("\\/") + 1);
        // Combine to get full image path
        std::string imgFullPath = tsxDir + imgRelativePath;

        std::cout << "Loading tileset image: " << imgFullPath << std::endl;

        int tileWidth = tileset->IntAttribute("tilewidth");
        int tileHeight = tileset->IntAttribute("tileheight");
        int columns = tileset->IntAttribute("columns");
        int tileCount = tileset->IntAttribute("tilecount");

        // Load tileset texture
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


        dsTileSet.emplace_back(
            columns,
            tileCount / columns,// row
            texture,
            tsxFullPath,
            imgFullPath,
            firstGid,
            firstGid + tileCount - 1// ;last ID
        );

        std::cout << "Successfully loaded tileset: " << tsxRelativePath << std::endl;
    }

    return !dsTileSet.empty();
}





