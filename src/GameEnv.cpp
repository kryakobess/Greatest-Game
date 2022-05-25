#include "gameEnv.h"

bool IinitScreen(SDL_Window** gWindow, SDL_Renderer** gRenderer, Mix_Music** gMusic, const int winW, const int winH) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}
	else {
		*gWindow = SDL_CreateWindow("TestWindow!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winW, winH, SDL_WINDOW_SHOWN);
		if (*gWindow == NULL) return false;
		else {
			*gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (*gRenderer == NULL) {
				printf("Render Error!\n");
				return false;
			}
			else {
				SDL_SetRenderDrawColor(*gRenderer, 0xEF, 0x32, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) && imgFlags)) {
					printf("IMG init error %s\n", IMG_GetError());
					return false;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
					printf("MIX init error %s\n", Mix_GetError());
					return false;
				}
			}
		}
	}
	return true;
}

SDL_Texture* loadTexture(const char* path, SDL_Renderer** gRenderer) {
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurf = IMG_Load(path);
	if (loadedSurf == NULL) {
		printf("Load err");
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(*gRenderer, loadedSurf);
		if (newTexture == NULL) printf("Texture creation error!\n");
		SDL_FreeSurface(loadedSurf);
	}
	return newTexture;
}

void DestroyAll(SDL_Window** gWindow, SDL_Renderer** gRenderer, Mix_Music** gMusic) {
	Mix_FreeMusic(*gMusic);
	*gMusic = NULL;
	SDL_DestroyRenderer(*gRenderer);
	SDL_DestroyWindow(*gWindow);
	*gWindow = NULL;
	*gRenderer = NULL;
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}