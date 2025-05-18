#include "Renderer.h"
#include "Define.h"
Renderer::Renderer() {
	gWindow = NULL;
	gRenderer = NULL;
}
Renderer::~Renderer() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
}
bool Renderer::Init() {
	bool success = true;
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		printf("Warning: Linear texture filtering not enabled!");
	}
	gWindow = SDL_CreateWindow("Run and gun", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIDHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (gRenderer == NULL) {
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			SDL_SetRenderDrawColor(Renderer::GetInstance()->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
		}
		SDL_Surface* mainIcon = IMG_Load("Data//Textures//icon.png");
		SDL_SetWindowIcon(gWindow, mainIcon);
	}
	bool ret = true;
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		ret = false;
	}
	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	return (success && ret);
}
SDL_Window* Renderer::GetWindow() {
	return gWindow;
}
SDL_Renderer* Renderer::GetRenderer() {
	return gRenderer;
}
void Renderer::SetWindow(SDL_Window* mWindow) {
	gWindow = mWindow;
}
void Renderer::SetRenderer(SDL_Renderer* mRenderer) {
	gRenderer = mRenderer;
}
