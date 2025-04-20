#include "TileSet.h"

TileSet::TileSet(int columns, int rows, SDL_Texture* texture, const std::string& tsxPath, const std::string& imgPath, int firstID, int lastID)
    : m_Columns(columns), m_Rows(rows), m_TsxPath(tsxPath), m_ImagePath(imgPath), m_FirstID(firstID), m_LastID(lastID) {
    m_TileWidth = 32; // Default tile size, can be parameterized
    m_TileHeight = 32;

	if (texture) {
		if (!m_pTexture) {
			m_pTexture = std::make_shared<TextureManager>();
		}
        m_pTexture->SetTextureObj(texture);
		SDL_QueryTexture(texture, NULL, NULL, &m_TileWidth, &m_TileHeight);
		m_TileWidth /= columns;
		m_TileHeight /= rows;
	}
}

TileSet::~TileSet() {
}

SDL_Rect TileSet::GetSourceRect(int tileID) const {
    SDL_Rect srcRect = { 0, 0, m_TileWidth, m_TileHeight };
    int localID = tileID - m_FirstID;

    srcRect.x = (localID % m_Columns) * m_TileWidth;
    srcRect.y = (localID / m_Columns) * m_TileHeight;

    return srcRect;
}

void TileSet::Init() {
	// Initialization logic (if needed)
}

void TileSet::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
    // TileSet-specific drawing logic (if needed)
}

void TileSet::Update(float deltaTime) {
    // TileSet-specific update logic (if needed)
}
