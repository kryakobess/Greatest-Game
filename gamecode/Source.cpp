#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WEIGHT_w 1280
#define HIGHT_w 720
#define SPRITE_NUMBER 4
#define VELOCITY 3

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Rect gSpriteClips[KEY_PRESS_SURFACE_TOTAL][SPRITE_NUMBER];
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gSpriteTexture = NULL;
SDL_Texture* gBackground = NULL;


bool IinitScreen() {	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}
	else {
		gWindow = SDL_CreateWindow("TestWindow!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WEIGHT_w, HIGHT_w, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) return false;
		else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL) {
				printf("Render Error!\n");
				return false;
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 0xEF, 0x32, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) && imgFlags)) {
					printf("IMG init error %s\n", IMG_GetError());
					return false;
				}
				else {
					gScreenSurface = SDL_GetWindowSurface(gWindow);
				}
			}
		}
	}
	return true;
}

SDL_Texture* loadTexture(const char* path) {
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurf = IMG_Load(path);
	if (loadedSurf == NULL) {
		printf("Load err");
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurf);
		if (newTexture == NULL) printf("Texture creation error!\n");
		SDL_FreeSurface(loadedSurf);
	}
	return newTexture;
}


bool loadMedia()
	{
		//Loading success flag
		bool success = true;

		gSpriteTexture = loadTexture("character.png");
		if (gSpriteTexture == NULL) {
			printf("Sprite texture loading fail!");
			success = false;
		}

		for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i) {
			for (int j = 0; j < SPRITE_NUMBER; ++j) {
				gSpriteClips[i][j].w = 210;
				gSpriteClips[i][j].h = 260;
				if (j > 0) gSpriteClips[i][j].x = gSpriteClips[i][j].w + gSpriteClips[i][j - 1].x + 30;
				else gSpriteClips[i][j].x = 0;
				if (i > 0) gSpriteClips[i][j].y = gSpriteClips[i][j].h + gSpriteClips[i - 1][j].y + 25;
				else gSpriteClips[i][j].y = 0;
			}
		}

		gBackground = loadTexture("background.png");
		if (gBackground == NULL) {
			printf("bg fail\n");
			success = false;
		}
		return success;
}


void DestroyAll() {
	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;
	SDL_DestroyTexture(gSpriteTexture);
	SDL_DestroyTexture(gBackground);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* args[]) {
	if (!IinitScreen()) {
		printf("Initialization error!");
	}
	else if (loadMedia()) 
	{
		SDL_Texture* rockTexture = loadTexture("rock.png");
		if (rockTexture == NULL) printf("Rock!!!\n");
		SDL_Rect rockPos;
		rockPos.x = 800;
		rockPos.y = 450;
		rockPos.w = 100;
		rockPos.h = 130;
		bool quit = false;
		SDL_Event event;
		SDL_Rect stretchRect;
		stretchRect.x = 300;
		stretchRect.y = 180;
		stretchRect.w = 60;
		stretchRect.h = 85;
		int dSp = 0; int rSp = 0; int lSp = 0; int uSp = 0;
		int mouseX = 0; int mouseY = 0;
		size_t lastEvent[2] = {0,0};

		while (!quit) {
			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT) {
					quit = true;
				}
				else if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
					SDL_GetMouseState(&mouseX, &mouseY);
					if ((mouseX >= (stretchRect.x)) && (mouseX <= (stretchRect.x + stretchRect.w)) &&
						(mouseY >= (stretchRect.y)) && (mouseY <= (stretchRect.y + stretchRect.h)))
					{
						if (event.button.state == SDL_PRESSED) {
							printf("This is me!\n");
						}
					}
				}
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
				if (currentKeyStates[SDL_SCANCODE_W]) {
					if (stretchRect.y > 0) {
						stretchRect.y -= VELOCITY;
						printf("Moving up\n");
					}
					else {
						printf("There is a wall above\n");
					}
					dSp = 0; rSp = 0; lSp = 0;
					lastEvent[0] = KEY_PRESS_SURFACE_UP; lastEvent[1] = uSp++;
				}
				if (currentKeyStates[SDL_SCANCODE_S]) {
					if (stretchRect.y < HIGHT_w - stretchRect.h) {
						stretchRect.y += VELOCITY;
						printf("Moving down\n");
					}
					else {
						printf("There is a wall down\n");
					}
					uSp = 0; rSp = 0; lSp = 0;
					lastEvent[0] = KEY_PRESS_SURFACE_DOWN; lastEvent[1] = dSp++;
				}
				if (currentKeyStates[SDL_SCANCODE_A]) {
					if (stretchRect.x > 0) {
						stretchRect.x -= VELOCITY;
						printf("Moving left\n");
					}
					else {
						printf("There is a wall at the left\n");
					}
					uSp = 0; rSp = 0; dSp = 0;
					lastEvent[0] = KEY_PRESS_SURFACE_LEFT; lastEvent[1] = lSp++;
				}
				if (currentKeyStates[SDL_SCANCODE_D]) {
					if (stretchRect.x < WEIGHT_w - stretchRect.w) {
						stretchRect.x += VELOCITY;
						printf("Moving right\n");
					}
					else {
						printf("There is a wall at the right\n");
					}
					uSp = 0; dSp = 0; lSp = 0;
					lastEvent[0] = KEY_PRESS_SURFACE_RIGHT; lastEvent[1] = rSp++;
				}
				SDL_RenderClear(gRenderer);
				SDL_RenderCopy(gRenderer, gBackground, NULL, NULL);
				SDL_RenderCopy(gRenderer, gSpriteTexture, &gSpriteClips[lastEvent[0]][((lastEvent[1]) / 8) % 4], &stretchRect);
				SDL_RenderCopy(gRenderer, rockTexture, NULL, &rockPos);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	DestroyAll();
	return 0;
}