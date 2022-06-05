#include "EscapeFromKapa.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
Mix_Music* gMusic = NULL;
Mix_Chunk* gMove = NULL;
CollidersArray* gCollidersArray = NULL;
Matrix gMatrix;
SDL_Rect gSpriteClips[KEY_PRESS_SURFACE_TOTAL][SPRITE_NUMBER];
SDL_Texture* gSpriteTexture = NULL;
SDL_Texture* gBackground = NULL;
gameObj rockUp; gameObj rockDown; gameObj sampleRock;
gameObj* objs[] = { &rockUp, &rockDown, &sampleRock };
int objNumber = 3;
SDL_Rect camera = { BG_WIDTH / 2, BG_HEIGHT / 2, WIDTH_w, HEIGHT_w };
int playersCount = 1;
character** players = (character**)malloc(sizeof(character*) * playersCount);
SDL_Event event;

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
	gMatrix.gTileTexture = loadTexture("tileset_regular.png", &gRenderer);
	if (gMatrix.gTileTexture == NULL) {
		printf("labirint tileset fail\n");
		success = false;
	}
	
	/*gMusic = Mix_LoadMUS("soundtrack1.mp3");
	if (gMusic == NULL) {
		printf("Soundtrack error!\n");
		success = false;
	}*/
	return success;
}

bool InitializeGameData(enum DataType dataType)
{
	if (!IinitScreen(&gWindow, &gRenderer, &gMusic, WIDTH_w, HEIGHT_w)) {
				printf("Initialization error!");
				return false;
	}
	else if (loadMedia())
	{
		if (!InitCollidersArray(&gCollidersArray, MAX_COUNT_COLLIDERS_ID)) return false;
		for (int i = 0; i < MAX_COUNT_COLLIDERS_ID; i++)
		{
			for (int j = 0; j < MAX_COUNT_COLLIDERS_ID; j++)
			{
				gCollidersArray->collisionMatrix[i][j] = true;
			}
		}
		if(!InitCreateLabirint(&gMatrix)) return false;
		if (!initGameObject(&rockUp, loadTexture("rock.png", &gRenderer), { 800, 450, 70, 65 }, { 0,0,160,80 }, { 800, 450, 70, 65 }, gCollidersArray, ROCK_COL_ID)) return false;
		if (!initGameObject(&rockDown, rockUp.texture, { 800, 450 + 65, 70, 65 }, { 0, 80, 160, 80 }, { 800, 450 + 65, 70, 65 }, gCollidersArray, ROCK_COL_ID)) return false;
		if (!initGameObject(&sampleRock, rockUp.texture, { -1500, -750, 70, 65 }, { 0, 0, 160, 160 }, { -1500, -750, 70, 65 }, gCollidersArray, ROCK_COL_ID)) return false;
		for (int i = 0; i < playersCount; ++i) {
			players[i] = (character*)malloc(sizeof(character));
		}
		if (!characterInit(players[LocalPlayer], gSpriteTexture, { WIDTH_w / 2, HEIGHT_w / 2, 60, 85 }, { WIDTH_w / 2 + 10, HEIGHT_w / 2 + 85 - 25, 40, 25 },
			{ WIDTH_w / 2, HEIGHT_w / 2, 60, 85 }, { 0, 0, WIDTH_w, HEIGHT_w }, gCollidersArray)) return false;
		if (!initGameItem(&players[LocalPlayer]->trap, loadTexture("trap.png", &gRenderer),
		{ players[LocalPlayer]->model.posRect->x, players[LocalPlayer]->model.posRect->y, 100, 100 }, { 0,0,600,600 }, 
			{ players[LocalPlayer]->model.posRect->x, players[LocalPlayer]->model.posRect->y, 100, 100 }, ActivateTrap, gCollidersArray, TRAP_COL_ID)) return false;
		
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				players[LocalPlayer]->spriteClips[i][j] = gSpriteClips[i][j];
			}
		}
		SDL_PollEvent(&event);
	}
	return true;
}

void GetData(enum DataType dataType) {

}

void SendData(enum DataType dataType) {

}

void Drawing() {
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, gBackground, players[LocalPlayer]->camera, NULL);
	DrawLabirint(gRenderer, players[LocalPlayer]->camera, &gMatrix);
	for (int i = 0; i < playersCount; ++i) { if (players[i]->trap.isActive) RenderObject(&players[i]->trap.itemModel, gRenderer); }
	RenderObject(&rockDown, gRenderer);
	RenderObject(&sampleRock, gRenderer);
	for (int i = playersCount - 1; i >= LocalPlayer; --i) {
		players[i]->model.srcRect = players[i]->spriteClips[players[i]->spriteNumber[0]][(players[i]->spriteNumber[1] / 8) % 4];
		RenderObject(&players[i]->model, gRenderer);
	}
	RenderObject(&rockUp, gRenderer);
	SDL_RenderPresent(gRenderer);
}

bool HandleInput(SDL_Event e, double velCoef) {
	int mouseX = 0; int mouseY = 0;
	SDL_PollEvent(&e);
	if (e.type == SDL_QUIT) {
			return false;
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
		SDL_GetMouseState(&mouseX, &mouseY);
		if ((mouseX >= (players[LocalPlayer]->model.posRect->x)) && (mouseX <= (players[LocalPlayer]->model.posRect->x + players[LocalPlayer]->model.posRect->w)) &&
			(mouseY >= (players[LocalPlayer]->model.posRect->y)) && (mouseY <= (players[LocalPlayer]->model.posRect->y + players[LocalPlayer]->model.posRect->h)))
		{
			if (e.button.state == SDL_PRESSED) {
				printf("This is me!\n");
			}
		}
	}
	const Uint8* movement = SDL_GetKeyboardState(NULL);
	HandleMovement(players, movement, objs, objNumber, playersCount, velCoef, gCollidersArray);
	return true;
}


void GameLoop(enum DataType dataType)
{
	Mix_PlayMusic(gMusic, -1);
	while (true) {
		GetData(dataType);
		Drawing();
		if (!HandleInput(event, 1)) break;
		SendData(dataType);
	}
}

//int main(int argc, char* args[]) {
//	if (!IinitScreen(&gWindow, &gRenderer, &gMusic, WIDTH_w, HEIGHT_w)) {
//		printf("Initialization error!");
//	}
//	else if (loadMedia())
//	{
//		SDL_Rect camera = { BG_WIDTH / 2, BG_HEIGHT / 2, WIDTH_w, HEIGHT_w };
//		gameObj rockUp; gameObj rockDown; gameObj sampleRock;
//		initGameObject(&rockUp, loadTexture("rock.png", &gRenderer), { 800, 450, 70, 65 }, { 0,0,160,80 }, {0});
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
//			SDL_RenderClear(gRenderer);
//			SDL_RenderCopy(gRenderer, gBackground, Survs[LocalPlayer]->camera, NULL);
//			SDL_RenderCopy(gRenderer, rockDown.texture, &rockDown.srcRect, rockDown.posRect);
//			SDL_RenderCopy(gRenderer, sampleRock.texture, &sampleRock.srcRect, sampleRock.posRect);
//			for (int i = 0; i < playersCount; ++i) { if (Survs[i]->trap.isActive) RenderObject(&Survs[i]->trap.itemModel, gRenderer); }
//			SDL_RenderCopy(gRenderer, Survs[1]->model.texture, &Survs[1]->spriteClips[0][0], Survs[1]->model.posRect);
//			SDL_RenderCopy(gRenderer, Survs[LocalPlayer]->model.texture, &Survs[0]->spriteClips[lastEvent[0]][((lastEvent[1]) / 8) % 4], Survs[LocalPlayer]->model.posRect);
//			SDL_RenderDrawRect(gRenderer, Survs[LocalPlayer]->hitBox);
//			SDL_RenderDrawRect(gRenderer, Survs[LocalPlayer]->model.collisionBox);
//			SDL_RenderDrawRect(gRenderer, sampleRock.collisionBox);
//			SDL_RenderCopy(gRenderer, rockUp.texture, &rockUp.srcRect, rockUp.posRect);
//			SDL_RenderPresent(gRenderer);
//		}
//	}
//	DestroyAll(&gWindow, &gRenderer, &gMusic);
//	return 0;
//}