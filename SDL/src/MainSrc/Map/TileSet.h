#pragma once
#include <string>
#include <SDL.h>
#include "BaseObject.h"
class TileSet : public BaseObject {
public:
    TileSet(int columns, int rows, SDL_Texture* texture, const std::string& tsxPath, const std::string& imgPath, int firstID, int lastID);
    ~TileSet();
    SDL_Rect GetSourceRect(int tileID) const;
    int GetFirstID() const { return m_FirstID; }
    int GetLastID() const { return m_LastID; }
    void Init() override;
    void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
    void Update(float deltaTime) override;
private:
    int m_TileWidth;
    int m_TileHeight;
    int m_Columns;
    int m_Rows;
    std::string m_TsxPath;
    std::string m_ImagePath;
    int m_FirstID;
    int m_LastID;
};
