#include "TextureManager.h"
#include "SDL_image.h"
TextureManager::TextureManager() {
	m_texture = NULL;
}
TextureManager::~TextureManager() {}
SDL_Texture* TextureManager::LoadImage(const std::string & path) {
	if (m_texture) {
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		m_texture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetRenderer(), loadedSurface);
		if (m_texture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else {
			SetOriginalTextureSurface(loadedSurface);
		}
		SDL_FreeSurface(loadedSurface);
	}
	return m_texture;
}
SDL_Texture* TextureManager::LoadTextureText(TTF_Font* font, SDL_Color color, const std::string& textureText) {
	if (m_texture) {
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), color);
	if (textSurface == NULL) {
		printf("Unable to render text surface ! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		m_texture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetRenderer(), textSurface);
		if (m_texture == NULL) {
			printf("Unable to create texture from renderered text!  SDL Error: %s\n", SDL_GetError());
		}
		else {
			SetOriginalTextureSurface(textSurface);
		}
		SDL_FreeSurface(textSurface);
	}
	return m_texture;
}
void TextureManager::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(m_texture, red, green, blue);
}
void TextureManager::SetBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(m_texture, blending);
}
void TextureManager::SetAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(m_texture, alpha);
}
void TextureManager::SetOriginalTextureSurface(SDL_Surface* surface) {
	originWidth = surface->w;
	originHeight = surface->h;
}
SDL_Texture* TextureManager::GetTextureObj() {
	return m_texture;
}
void TextureManager::SetTextureObj(SDL_Texture* texture) {
	m_texture = std::move(texture);
}
int TextureManager::GetOriginalWidth() {
	return originWidth;
}
int TextureManager::GetOriginalHeight() {
	return originHeight;
}

