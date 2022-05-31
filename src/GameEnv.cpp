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
				if (!(IMG_Init(imgFlags) & imgFlags)) {
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

bool isCollided(SDL_Rect a, SDL_Rect b)
{
	int leftA = a.x;
	int rightA = a.x + a.w;
	int topA = a.y;
	int bottomA = a.y + a.h;

	int leftB = b.x;
	int rightB = b.x + b.w;
	int topB = b.y;
	int bottomB = b.y + b.h;

	if (bottomA <= topB) return false;
	if (topA >= bottomB) return false;
	if (rightA <= leftB) return false;
	if (leftA >= rightB) return false;
	return true;
}

void Timer_Init(Timer* t) {
	t->startTicks = 0;
	t->pausedTicks = 0;
	t->paused = false;
	t->started = false;
}

void Timer_Start(Timer* t) {
	t->started = true;
	t->paused = false;
	t->startTicks = SDL_GetTicks();
	t->pausedTicks = 0;
}

void Timer_Stop(Timer* t) {
	t->started = false;
	t->paused = false;
	t->startTicks = 0;
	t->pausedTicks = 0;
}

void Timer_Pause(Timer* t) {
	if (t->started && !t->paused)
	{
		t->paused = true;
		t->pausedTicks = SDL_GetTicks() - t->startTicks;
		t->startTicks = 0;
	}
}

void Timer_Unpause(Timer* t) {
	if (t->started && t->paused)
	{
		t->paused = false;
		t->startTicks = SDL_GetTicks() - t->pausedTicks;
		t->pausedTicks = 0;
	}
}

Uint32 Timer_GetTicks(Timer* t) {
	Uint32 time = 0;
	if (t->started)
	{
		if (t->paused)
		{
			time = t->pausedTicks;
		}
		else
		{
			time = SDL_GetTicks() - t->startTicks;
		}
	}
	return time;
}