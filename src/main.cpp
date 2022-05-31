#include "gameEnv.h"
#include "gameObjects.h"
#include "DateBase\DateBase.h"

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
	gMusic = Mix_LoadMUS("music.mp3");
	if (gMusic == NULL) {
		printf("Soundtrack error!\n");
		success = false;
	}
	return success;
}

int main(int argc, char* args[])
{

	//InitializeGameData();
	return 0;
}

//int main(int argc, char* args[]) {
//	if (!IinitScreen(&gWindow, &gRenderer, &gMusic, WIDTH_w, HEIGHT_w)) {
//		printf("Initialization error!");
//	}
//	else if (loadMedia())
//	{
//		SDL_Rect camera = { BG_WIDTH / 2, BG_HEIGHT / 2, WIDTH_w, HEIGHT_w };
//		gameObj rockUp; gameObj rockDown; gameObj sampleRock;
//		initGameObject(&rockUp, loadTexture("rock.png", &gRenderer), { 800, 450, 70, 65 }, { 0,0,160,80 }, {-10000,-10000, 0, 0});
//		initGameObject(&rockDown, rockUp.texture, { 800, 450 + 65, 70, 65 }, { 0, 80, 160, 80 }, { 800, 450 + 65, 70, 65 });
//		initGameObject(&sampleRock, rockUp.texture, { -1500, -750, 70, 65 }, { 0, 0, 160, 160 }, { -1500, -750, 70, 65 });
//		if (rockUp.texture == NULL) printf("Rock!!!\n");
//		int playersCount = 2;
//
//		character** Survs = (character**)malloc(sizeof(character*)*playersCount);
//		for (int i = 0; i < playersCount; ++i) {
//			Survs[i] = (character*)malloc(sizeof(character));
//		}
//		characterInit(Survs[LocalPlayer], gSpriteTexture, {WIDTH_w / 2, HEIGHT_w / 2, 60, 85}, {WIDTH_w / 2 + 10, HEIGHT_w / 2 + 85 - 25, 40, 25},
//								{ WIDTH_w / 2, HEIGHT_w / 2, 60, 85 }, { BG_WIDTH / 2, BG_HEIGHT / 2, WIDTH_w, HEIGHT_w });
//		characterInit(Survs[1], gSpriteTexture, { 190, 180, 60, 85 }, { 190, 180, 60, 85 }, { 190, 180, 60, 85 }, {0});
//		initGameItem(&Survs[LocalPlayer]->trap, loadTexture("trap.png", &gRenderer), {Survs[LocalPlayer]->model.posRect->x, Survs[LocalPlayer]->model.posRect->y, 100, 100},
//			{ 0,0,600,600 }, {0}, ActivateTrap);
//		initGameItem(&Survs[1]->trap, loadTexture("trap.png", &gRenderer), { Survs[1]->model.posRect->x, Survs[1]->model.posRect->y, 100, 100 },
//			{ 0,0,600,600 }, { 0 }, ActivateTrap);
//
//		for (int i = 0; i < 4; ++i) {
//			for (int j = 0; j < 4; ++j) {
//				Survs[LocalPlayer]->spriteClips[i][j] = gSpriteClips[i][j];
//				Survs[1]->spriteClips[i][j] = gSpriteClips[i][j];
//			}
//		}
//		int mouseX = 0; int mouseY = 0;
//		size_t lastEvent[2] = { 0,0 };
//		bool quit = false;
//		SDL_Event event;
//		Mix_PlayMusic(gMusic, -1);
//		while (!quit) {
//			SDL_PollEvent(&event);
//			if (event.type == SDL_QUIT) {
//				quit = true;
//			}
//			else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP ) {
//				SDL_GetMouseState(&mouseX, &mouseY);
//				if ((mouseX >= (Survs[LocalPlayer]->model.posRect->x)) && (mouseX <= (Survs[LocalPlayer]->model.posRect->x + Survs[LocalPlayer]->model.posRect->w)) &&
//					(mouseY >= (Survs[LocalPlayer]->model.posRect->y)) && (mouseY <= (Survs[LocalPlayer]->model.posRect->y + Survs[LocalPlayer]->model.posRect->h)))
//				{
//					if (event.button.state == SDL_PRESSED) {
//						printf("This is me!\n");
//					}
//				}
//			}
//			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
//			gameObj* saveObj[3] = { &rockUp, &rockDown, &sampleRock };
//			HandleMovement(Survs, currentKeyStates, lastEvent, saveObj, 3, playersCount, 1);
//			printf("y = %d x = %d\n", Survs[0]->model.posRect->y, Survs[0]->model.posRect->x);
//			SDL_RenderClear(gRenderer);
//			SDL_RenderCopy(gRenderer, gBackground, Survs[LocalPlayer]->camera, NULL);
//			SDL_RenderCopy(gRenderer, rockDown.texture, &rockDown.srcRect, rockDown.posRect);
//			SDL_RenderCopy(gRenderer, sampleRock.texture, &sampleRock.srcRect, sampleRock.posRect);
//			for (int i = 0; i < playersCount; ++i) {
//				if (Survs[i]->trap.isActive) RenderObject(&Survs[i]->trap.itemModel, gRenderer);
//			}
//			SDL_RenderCopy(gRenderer, Survs[1]->model.texture, &Survs[1]->spriteClips[0][0], Survs[1]->model.posRect);
//			SDL_RenderCopy(gRenderer, Survs[LocalPlayer]->model.texture, &Survs[0]->spriteClips[lastEvent[0]][((lastEvent[1]) / 8) % 4], Survs[LocalPlayer]->model.posRect);
//			SDL_RenderDrawRect(gRenderer, Survs[LocalPlayer]->hitBox);
//			SDL_RenderDrawRect(gRenderer, Survs[LocalPlayer]->model.collisionBox);
//			SDL_RenderDrawRect(gRenderer, rockDown.collisionBox);
//			SDL_RenderCopy(gRenderer, rockUp.texture, &rockUp.srcRect, rockUp.posRect);
//			SDL_RenderPresent(gRenderer);
//		}
//	}
//	DestroyAll(&gWindow, &gRenderer, &gMusic);
//	return 0;
//}