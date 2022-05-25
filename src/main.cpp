#include "gameEnv.h"
#include "gameObjects.h"

#define SPRITE_NUMBER 4
#define WIDTH_w 1280
#define HEIGHT_w 720
#define BG_WIDTH 3800
#define BG_HEIGHT 2170

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
Mix_Music* gMusic = NULL;
Mix_Chunk* gMove = NULL;
SDL_Rect gSpriteClips[KEY_PRESS_SURFACE_TOTAL][SPRITE_NUMBER];
SDL_Texture* gSpriteTexture = NULL;
SDL_Texture* gBackground = NULL;

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	gSpriteTexture = loadTexture("character.png", &gRenderer);
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

	gBackground = loadTexture("background.png", &gRenderer);
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

int main(int argc, char* args[]) {
	if (!IinitScreen(&gWindow, &gRenderer, &gMusic, WIDTH_w, HEIGHT_w)) {
		printf("Initialization error!");
	}
	else if (loadMedia())
	{
		SDL_Rect camera = { BG_WIDTH / 2, BG_HEIGHT / 2, WIDTH_w, HEIGHT_w };
		gameObj rockUp; gameObj rockDown;
		initGameObject(&rockUp, loadTexture("rock.png", &gRenderer), { 800, 450, 70, 65 }, { 0,0,160,80 }, { 800, 450, 70, 65 });
		initGameObject(&rockDown, rockUp.texture, { 800, 450 + 65, 70, 65 }, { 0, 80, 160, 80 }, { 800, 450 + 65, 70, 65 });
		if (rockUp.texture == NULL) printf("Rock!!!\n");

		character Surv;
		characterInit(&Surv, gSpriteTexture, { WIDTH_w / 2, HEIGHT_w / 2, 60, 85 }, { WIDTH_w / 2 + 10, HEIGHT_w / 2 + 85 - 25, 40, 25 },
			{ BG_WIDTH / 2, BG_HEIGHT / 2, WIDTH_w, HEIGHT_w });

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				Surv.spriteClips[i][j] = gSpriteClips[i][j];
			}
		}
		/*Surv.camera = { BG_WIDTH / 2, BG_HEIGHT / 2, WIDTH_w, HEIGHT_w };
		Surv.posRect = { WIDTH_w / 2, HEIGHT_w / 2, 60, 85 };
		Surv.hitBox = { Surv.posRect.x + 10, Surv.posRect.y + Surv.posRect.h - 25, 40, 25 };
		Surv.texture = gSpriteTexture;*/

		/*SDL_Rect stretchRect;
		stretchRect.x = WIDTH_w / 2; stretchRect.y = HEIGHT_w / 2;
		stretchRect.w = 60; stretchRect.h = 85;*/
		/*SDL_Rect hitBox;
		hitBox.x = stretchRect.x + 10; hitBox.y = stretchRect.y + stretchRect.h - 25;
		hitBox.w = 40; hitBox.h = 25;*/

		//int dSp = 0; int rSp = 0; int lSp = 0; int uSp = 0;
		//int mouseX = 0; int mouseY = 0;
		size_t lastEvent[2] = { 0,0 };
		bool quit = false;
		SDL_Event event;
		Mix_PlayMusic(gMusic, -1);
		while (!quit) {
			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT) {
					quit = true;
				}
				/*else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP ) {
					SDL_GetMouseState(&mouseX, &mouseY);
					if ((mouseX >= (stretchRect.x)) && (mouseX <= (stretchRect.x + stretchRect.w)) &&
						(mouseY >= (stretchRect.y)) && (mouseY <= (stretchRect.y + stretchRect.h)))
					{
						if (event.button.state == SDL_PRESSED) {
							printf("This is me!\n");
						}
					}
				}*/
			}
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			int yShift = 0; int xShift = 0;
			HandleMovement(&Surv, currentKeyStates, HEIGHT_w, WIDTH_w, BG_HEIGHT, BG_WIDTH, &yShift, &xShift, lastEvent);
			gameObj* saveObj[2] = { &rockUp, &rockDown };
			SaveObjPosition(saveObj, 2, yShift, xShift);
			/*if (currentKeyStates[SDL_SCANCODE_W]) {
				if (camera.y > 0 && (stretchRect.y == HEIGHT_w / 2)) {
					camera.y -= VELOCITY;
					rockUp.posRect.y += VELOCITY; rockDown.posRect.y += VELOCITY;
					rockDown.hitBox = rockDown.posRect;
				}
				else if (stretchRect.y > 0) {
					stretchRect.y -= VELOCITY;
					hitBox.y -= VELOCITY;
				}
				if (isCollided(hitBox, rockDown.hitBox)) {
					if (camera.y == 0 || stretchRect.y != HEIGHT_w / 2) {
						stretchRect.y += VELOCITY;
						hitBox.y += VELOCITY;
					}
					else {
						camera.y += VELOCITY;
						rockUp.posRect.y -= VELOCITY; rockDown.posRect.y -= VELOCITY;
						rockDown.hitBox = rockDown.posRect;
					}
				}
				dSp = 0; rSp = 0; lSp = 0;
				lastEvent[0] = KEY_PRESS_SURFACE_UP; lastEvent[1] = uSp++;
			}
			if (currentKeyStates[SDL_SCANCODE_S]) {
				if (camera.y < BG_HEIGHT && (stretchRect.y == HEIGHT_w / 2)) {
					camera.y += VELOCITY;
					rockUp.posRect.y -= VELOCITY; rockDown.posRect.y -= VELOCITY;
					rockDown.hitBox = rockDown.posRect;
				}
				else if (stretchRect.y < HEIGHT_w - stretchRect.h) {
					stretchRect.y += VELOCITY;
					hitBox.y += VELOCITY;
				}
				if (isCollided(hitBox, rockDown.hitBox)) {
					if (camera.y == BG_HEIGHT || stretchRect.y != HEIGHT_w / 2) {
						stretchRect.y -= VELOCITY;
						hitBox.y -= VELOCITY;
					}
					else {
						camera.y -= VELOCITY;
						rockUp.posRect.y += VELOCITY; rockDown.posRect.y += VELOCITY;
						rockDown.hitBox = rockDown.posRect;
					}
				}
				uSp = 0; rSp = 0; lSp = 0;
				lastEvent[0] = KEY_PRESS_SURFACE_DOWN; lastEvent[1] = dSp++;
			}
			if (currentKeyStates[SDL_SCANCODE_A]) {
				if (camera.x > 0 && stretchRect.x == WIDTH_w / 2) {
					camera.x -= VELOCITY;
					rockUp.posRect.x += VELOCITY; rockDown.posRect.x += VELOCITY;
					rockDown.hitBox = rockDown.posRect;
				}
				else if (stretchRect.x > 0) {
					stretchRect.x -= VELOCITY;
					hitBox.x -= VELOCITY;
				}
				if (isCollided(hitBox, rockDown.hitBox)) {
					if (camera.x == 0 || stretchRect.x != WIDTH_w / 2) {
						stretchRect.x += VELOCITY;
						hitBox.x += VELOCITY;
					}
					else {
						camera.x += VELOCITY;
						rockUp.posRect.x -= VELOCITY; rockDown.posRect.x -= VELOCITY;
						rockDown.hitBox = rockDown.posRect;
					}
				}
				uSp = 0; rSp = 0; dSp = 0;
				lastEvent[0] = KEY_PRESS_SURFACE_LEFT; lastEvent[1] = lSp++;
			}
			if (currentKeyStates[SDL_SCANCODE_D]) {
				if (camera.x < BG_WIDTH && stretchRect.x == WIDTH_w / 2) {
					camera.x += VELOCITY;
					rockUp.posRect.x -= VELOCITY; rockDown.posRect.x -= VELOCITY;
					rockDown.hitBox = rockDown.posRect;
				}
				else if (stretchRect.x < WIDTH_w - stretchRect.w) {
					stretchRect.x += VELOCITY;
					hitBox.x += VELOCITY;
				}
				if (isCollided(hitBox, rockDown.hitBox)) {
					if (camera.x == BG_WIDTH || stretchRect.x != WIDTH_w / 2) {
						stretchRect.x -= VELOCITY;
						hitBox.x -= VELOCITY;
					}
					else {
						camera.x -= VELOCITY;
						rockUp.posRect.x += VELOCITY; rockDown.posRect.x += VELOCITY;
						rockDown.hitBox = rockDown.posRect;
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
			}*/
			SDL_RenderClear(gRenderer);
			SDL_RenderCopy(gRenderer, gBackground, Surv.camera, NULL);
			SDL_RenderCopy(gRenderer, rockDown.texture, &rockDown.srcRect, &rockDown.posRect);
			SDL_RenderCopy(gRenderer, gSpriteTexture, &Surv.spriteClips[lastEvent[0]][((lastEvent[1]) / 8) % 4], Surv.posRect);
			SDL_RenderCopy(gRenderer, rockUp.texture, &rockUp.srcRect, &rockUp.posRect);
			SDL_RenderPresent(gRenderer);
		}
	}
	DestroyAll(&gWindow, &gRenderer, &gMusic);
	return 0;
}