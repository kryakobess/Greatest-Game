#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH_w 1280
#define HEIGHT_w 720
#define BG_WIDTH 3800
#define BG_HEIGHT 2170
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
SDL_Rect gSpriteClips[KEY_PRESS_SURFACE_TOTAL][SPRITE_NUMBER];
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gSpriteTexture = NULL;
SDL_Texture* gBackground = NULL;
Mix_Music* gMusic = NULL;
Mix_Chunk* gMove = NULL;


bool IinitScreen() {	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}
	else {
		gWindow = SDL_CreateWindow("TestWindow!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH_w, HEIGHT_w, SDL_WINDOW_SHOWN);
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
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
					printf("MIX init error %s\n", Mix_GetError());
					return false;
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
		gMusic = Mix_LoadMUS("soundtrack1.mp3");
		if (gMusic == NULL) {
			printf("Soundtrack error!\n");
			success = false;
		}
		return success;
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

void DestroyAll() {
	Mix_FreeMusic(gMusic);
	gMusic = NULL;
	Mix_FreeChunk(gMove);
	gMove = NULL;
	SDL_DestroyTexture(gSpriteTexture);
	SDL_DestroyTexture(gBackground);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* args[]) {
	if (!IinitScreen()) {
		printf("Initialization error!");
	}
	else if (loadMedia()) 
	{
		SDL_Rect camera = { BG_WIDTH/2, BG_HEIGHT/2, WIDTH_w, HEIGHT_w };
		SDL_Texture* rockTexture = loadTexture("rock.png");
		if (rockTexture == NULL) printf("Rock!!!\n");
		SDL_Rect rockPos[2];
		rockPos[0].x = 800; rockPos[0].y = 450;
		rockPos[0].w = 70; rockPos[0].h = 65;
		rockPos[1].x = 800; rockPos[1].y = 450+65;
		rockPos[1].w = 70; rockPos[1].h = 65;
		SDL_Rect rockHalf[2];
		rockHalf[0].x = 0; rockHalf[0].y = 0;
		rockHalf[0].w = 160; rockHalf[0].h = 80;
		rockHalf[1].x = 0; rockHalf[1].y = 80;
		rockHalf[1].w = 160; rockHalf[1].h = 80;
		bool quit = false;
		SDL_Event event;
		SDL_Rect stretchRect;
		stretchRect.x = WIDTH_w / 2; stretchRect.y = HEIGHT_w / 2;
		stretchRect.w = 60; stretchRect.h = 85;
		SDL_Rect hitBox;
		hitBox.x = stretchRect.x+10; hitBox.y = stretchRect.y + stretchRect.h-25;
		hitBox.w = 40; hitBox.h = 25;
		
		int dSp = 0; int rSp = 0; int lSp = 0; int uSp = 0;
		int mouseX = 0; int mouseY = 0;
		size_t lastEvent[2] = {0,0};
		Mix_PlayMusic(gMusic, -1);
		while (!quit) {
			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT) {
					quit = true;
				}
				else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
					SDL_GetMouseState(&mouseX, &mouseY);
					if ((mouseX >= (stretchRect.x)) && (mouseX <= (stretchRect.x + stretchRect.w)) &&
						(mouseY >= (stretchRect.y)) && (mouseY <= (stretchRect.y + stretchRect.h)))
					{
						if (event.button.state == SDL_PRESSED) {
							printf("This is me!\n");
						}
					}
				}
				//KEY PRESS CHECKOUT
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
				if (currentKeyStates[SDL_SCANCODE_W]) {
					if (camera.y > 0 && (stretchRect.y == HEIGHT_w/2)) {
						camera.y -= VELOCITY;
						rockPos[0].y += VELOCITY; rockPos[1].y += VELOCITY;
					}
					else if (stretchRect.y > 0) {
						stretchRect.y -= VELOCITY;
						hitBox.y -= VELOCITY;
					}
					if (isCollided(hitBox, rockPos[1])) {
						if (camera.y == 0 || stretchRect.y != HEIGHT_w / 2) {
							stretchRect.y += VELOCITY;
							hitBox.y += VELOCITY;
						}
						else {
							camera.y += VELOCITY;
							rockPos[0].y -= VELOCITY; rockPos[1].y -= VELOCITY;
						}
					}
					dSp = 0; rSp = 0; lSp = 0;
					lastEvent[0] = KEY_PRESS_SURFACE_UP; lastEvent[1] = uSp++;
				}
				if (currentKeyStates[SDL_SCANCODE_S]) {
					if (camera.y < BG_HEIGHT && (stretchRect.y == HEIGHT_w/2)) {
						camera.y += VELOCITY;
						rockPos[0].y -= VELOCITY; rockPos[1].y -= VELOCITY;
					}
					else if (stretchRect.y < HEIGHT_w - stretchRect.h) {
						stretchRect.y += VELOCITY;
						hitBox.y += VELOCITY;
					}
					if (isCollided(hitBox, rockPos[1])) {
						if (camera.y == BG_HEIGHT || stretchRect.y != HEIGHT_w / 2) {
							stretchRect.y -= VELOCITY;
							hitBox.y -= VELOCITY;
						}
						else {
							camera.y -= VELOCITY;
							rockPos[0].y += VELOCITY; rockPos[1].y += VELOCITY;
						}
					}
					uSp = 0; rSp = 0; lSp = 0;
					lastEvent[0] = KEY_PRESS_SURFACE_DOWN; lastEvent[1] = dSp++;
				}
				if (currentKeyStates[SDL_SCANCODE_A]) {
					if (camera.x > 0 && stretchRect.x == WIDTH_w/2) {
						camera.x -= VELOCITY;
						rockPos[0].x += VELOCITY; rockPos[1].x += VELOCITY;
					}
					else if (stretchRect.x > 0 ) {
						stretchRect.x -= VELOCITY;
						hitBox.x -= VELOCITY;
					}
					if (isCollided(hitBox, rockPos[1])) {
						if (camera.x == 0 || stretchRect.x != WIDTH_w / 2) {
							stretchRect.x += VELOCITY;
							hitBox.x += VELOCITY;
						}
						else {
							camera.x += VELOCITY;
							rockPos[0].x -= VELOCITY; rockPos[1].x -= VELOCITY;
						}
					}
					uSp = 0; rSp = 0; dSp = 0;
					lastEvent[0] = KEY_PRESS_SURFACE_LEFT; lastEvent[1] = lSp++;
				}
				if (currentKeyStates[SDL_SCANCODE_D]) {
					if (camera.x < BG_WIDTH && stretchRect.x == WIDTH_w / 2) {
						camera.x += VELOCITY;
						rockPos[0].x -= VELOCITY; rockPos[1].x -= VELOCITY;
					}
					else if (stretchRect.x < WIDTH_w - stretchRect.w) {
						stretchRect.x += VELOCITY;
						hitBox.x += VELOCITY;
					}
					if (isCollided(hitBox, rockPos[1])) {
						if (camera.x == BG_WIDTH || stretchRect.x != WIDTH_w / 2) {
							stretchRect.x -= VELOCITY;
							hitBox.x -= VELOCITY;
						}
						else {
							camera.x -= VELOCITY;
							rockPos[0].x += VELOCITY; rockPos[1].x += VELOCITY;
						}
					}
					uSp = 0; dSp = 0; lSp = 0;
					lastEvent[0] = KEY_PRESS_SURFACE_RIGHT; lastEvent[1] = rSp++;
				}
				if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
					if (Mix_PausedMusic() == 1) {
						Mix_ResumeMusic();
					}
					else {
						Mix_PauseMusic();
					}
				}
				SDL_RenderClear(gRenderer);
				SDL_RenderCopy(gRenderer, gBackground, &camera, NULL);
				SDL_RenderCopy(gRenderer, rockTexture, &rockHalf[1], &rockPos[1]);
				SDL_RenderCopy(gRenderer, gSpriteTexture, &gSpriteClips[lastEvent[0]][((lastEvent[1]) / 8) % 4], &stretchRect);
				SDL_RenderCopy(gRenderer, rockTexture, &rockHalf[0], &rockPos[0]);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	DestroyAll();
	return 0;
}