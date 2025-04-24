#include "MapLoader.h"
#include <iostream>
#include <sstream>
#include <SDL_image.h>
#include "Tinyxml2/tinyxml2.h"

MapLoader::MapLoader(SDL_Renderer* renderer) : m_Renderer(renderer) {}

MapLoader::~MapLoader() {}

std::shared_ptr<Map> MapLoader::LoadMap(const std::string& filePath) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load TMX file: " << filePath << std::endl;
        return nullptr;
    }

    auto* mapElement = doc.FirstChildElement("map");
    if (!mapElement) {
        std::cerr << "TMX file does not contain <map> element." << std::endl;
        return nullptr;
    }

    std::shared_ptr<Map> map = std::make_shared<Map>();
    map->SetFilePath(filePath);

    // Read width and height from the <map> element
    int width = mapElement->IntAttribute("width");
    int height = mapElement->IntAttribute("height");
    map->SetWidth(width * PIXEL_WIDTH);
    map->SetHeight(height * PIXEL_HEIGHT);

    // Load tilesets
    if (!LoadTileSets(mapElement, *map)) {
        std::cerr << "Failed to load tilesets." << std::endl;
        return nullptr;
    }

    // Load layers
    if (!LoadLayers(mapElement, *map)) {
        std::cerr << "Failed to load layers." << std::endl;
        return nullptr;
    }

    return map;
}

bool MapLoader::LoadTileSets(tinyxml2::XMLElement* mapElement, Map& map) {
    std::string tmxDir = map.GetFilePath().substr(0, map.GetFilePath().find_last_of("\\/") + 1);
    //std::cerr << map.GetFilePath() << std::endl;

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

        std::string imgFullPath = tmxDir + imgRelativePath;

        int tileWidth = tileset->IntAttribute("tilewidth");
        int tileHeight = tileset->IntAttribute("tileheight");
        int columns = tileset->IntAttribute("columns");
        int tileCount = tileset->IntAttribute("tilecount");

        SDL_Surface* surface = IMG_Load(imgFullPath.c_str());
        if (!surface) {
            std::cerr << "Failed to load tileset image: " << IMG_GetError() << std::endl;
            return false;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            return false;
        }

        int lastGid = firstGid + tileCount - 1;

        map.AddTileSet(TileSet(columns, tileCount / columns, texture, tsxFullPath, imgFullPath, firstGid, lastGid));
    }

    return true;
}

bool MapLoader::LoadLayers(tinyxml2::XMLElement* mapElement, Map& map) {
    int mapWidth = mapElement->IntAttribute("width");
    int mapHeight = mapElement->IntAttribute("height");

    for (auto* layerElement = mapElement->FirstChildElement("layer");
        layerElement;
        layerElement = layerElement->NextSiblingElement("layer")) {

		int idLayer = layerElement->IntAttribute("id");
        const char* name = layerElement->Attribute("name");
		bool visible = layerElement->BoolAttribute("visible", true);

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
        std::string csvData = dataElement->GetText();
        std::stringstream ss(csvData);
        std::string item;

        while (std::getline(ss, item, ',')) {
            item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
            if (!item.empty()) {
                try {
                    layerData.push_back(std::stoi(item));
                }
                catch (const std::exception&) {
                    std::cerr << "Error parsing tile ID: " << item << std::endl;
                    return false;
                }
            }
        }

		std::shared_ptr<Layer> layer = std::make_shared<Layer>(mapWidth, mapHeight);
        for (int i = 0; i < layerData.size(); ++i) {
            int tileID = layerData[i];
            if (tileID == 0) continue;

            std::shared_ptr<Pixel> pixel = std::make_shared<Pixel>();
            int row = i / mapWidth;
            int col = i % mapWidth;

            pixel->SetPosition(Vector3(col * PIXEL_WIDTH, row * PIXEL_HEIGHT, 0));

            for (auto tileset : map.GetTileSets()) {
                if (tileset.GetFirstID() <= tileID && tileID <= tileset.GetLastID()) {
                    pixel->SetSourceRect(tileset.GetSourceRect(tileID));
                    pixel->SetTexture(tileset.GetTexture());
                    break;
                }
            }

            layer->AddPixel(*pixel);
        }

        layer->SetID(idLayer);
        layer->SetVisible(visible);
        layer->SetName(name ? name : "");

		if (layer->GetName() == MAP_COLLISION_LAYER_NAME) {
			map.SetCollisionLayer(layer);
		}

        map.AddLayer(*layer);
    }

    return true;
}
