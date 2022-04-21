#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WEIGHT_w 1280
#define HIGHT_w 720

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface* gCurSurface = NULL;

bool IinitScreen() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}
	else {
		gWindow = SDL_CreateWindow("TestWindow!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WEIGHT_w, HIGHT_w, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) return false;
		else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return true;
}

SDL_Surface* LoadSurface(const char* path) {
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurf = SDL_LoadBMP(path);
	if (loadedSurf == NULL) {
		printf("Load Error %s", SDL_GetError());
	}
	else {
		optimizedSurface = SDL_ConvertSurface(loadedSurf, gScreenSurface->format, 0);
		if (optimizedSurface == NULL) {
			printf("Optimization error!");
		}
		SDL_FreeSurface(loadedSurf);
	}
	return optimizedSurface;
}

bool loadMedia()
	{
		//Loading success flag
		bool success = true;

		//Load default surface
		gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = LoadSurface("press.bmp");
		if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
		{
			printf("Failed to load default image!\n");
			success = false;
		}

		//Load up surface
		gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = LoadSurface("up.bmp");
		if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
		{
			printf("Failed to load up image!\n");
			success = false;
		}

		//Load down surface
		gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = LoadSurface("down.bmp");
		if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
		{
			printf("Failed to load down image!\n");
			success = false;
		}

		//Load left surface
		gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = LoadSurface("left.bmp");
		if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
		{
			printf("Failed to load left image!\n");
			success = false;
		}

		//Load right surface
		gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = LoadSurface("right.bmp");
		if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
		{
			printf("Failed to load right image!\n");
			success = false;
		}

		return success;
}


void DestroyAll() {
	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i) {
		SDL_FreeSurface(gKeyPressSurfaces[i]);
		gKeyPressSurfaces[i] = NULL; 
	}
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}


int main(int argc, char* args[]) {
	if (!IinitScreen()) {
		printf("Initialization error!");
	}
	else if (loadMedia()) 
	{
		bool quit = false;
		SDL_Event event;
		SDL_Rect stretchRect;
		stretchRect.x = 0;
		stretchRect.y = 0;
		stretchRect.w = 60;
		stretchRect.h = 85;
		gCurSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
		while (!quit) {
			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT) {
					quit = true;
				}
				else if (event.type == SDL_KEYDOWN) {
					switch (event.key.keysym.sym) {
					case SDLK_UP:
						gCurSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
						if (stretchRect.y > 0) {
							stretchRect.y-=10;
							printf("Moving up\n");
						}
						else {
							printf("There is a wall above\n");
						}
						break;

					case SDLK_DOWN:
						gCurSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
						if (stretchRect.y < HIGHT_w-stretchRect.h) {
							stretchRect.y+=10;
							printf("Moving down\n");
						}
						else {
							printf("There is a wall down\n");
						}
						break;

					case SDLK_LEFT:
						gCurSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
						if (stretchRect.x > 0) {
							stretchRect.x-=10;
							printf("Moving left\n");
						}
						else {
							printf("There is a wall at the left\n");
						}
						break;

					case SDLK_RIGHT:
						gCurSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
						if (stretchRect.x < WEIGHT_w-stretchRect.w) {
							stretchRect.x+=10;
							printf("Moving right\n");
						}
						else {
							printf("There is a wall at the right\n");
						}
						break;

					default:
						gCurSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
						break;
					}
				}
				SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));
				SDL_BlitScaled(gCurSurface, NULL, gScreenSurface, &stretchRect);
				SDL_UpdateWindowSurface(gWindow);
				
			}
		}
	}
	DestroyAll();
	return 0;
}