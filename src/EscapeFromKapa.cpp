#include "EscapeFromKapa.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
Mix_Music* gMusic = NULL;
Mix_Chunk* gMoveChunk = NULL;
Mix_Chunk* gRunChunk = NULL;
Mix_Chunk* gSwordAttackChunk = NULL;
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
DateBase gDataBase;

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
	
	gMusic = Mix_LoadMUS("soundtrack1.mp3");
	if (gMusic == NULL) {
		printf("Soundtrack error!\n");
		success = false;
	}
	gSwordAttackChunk = Mix_LoadWAV("sword_attack.wav");
	if (gSwordAttackChunk == NULL) {
		printf("attack chunk error!\n");
		success = false;
	}
	Mix_VolumeChunk(gSwordAttackChunk, 30);
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
				gCollidersArray->collisionMatrix[i][j] = false;
			}
		}
		gCollidersArray->collisionMatrix[PLAYER_COL_ID][ROCK_COL_ID] = true;
		gCollidersArray->collisionMatrix[PLAYER_COL_ID][WALL_COL_ID] = true;
		gCollidersArray->collisionMatrix[PLAYER_COL_ID][TRAP_COL_ID] = true;
		if(!InitCreateLabirint(&gMatrix, gCollidersArray)) return false;
		if (!initGameObject(&rockUp, loadTexture("rock.png", &gRenderer), { 800, 450, 70, 65 }, { 0,0,160,80 }, { 800, 450, 70, 65 }, gCollidersArray, ROCK_COL_ID)) return false;
		if (!initGameObject(&rockDown, rockUp.texture, { 800, 450 + 65, 70, 65 }, { 0, 80, 160, 80 }, { 800, 450 + 65, 70, 65 }, gCollidersArray, ROCK_COL_ID)) return false;
		if (!initGameObject(&sampleRock, rockUp.texture, { -1500, -750, 70, 65 }, { 0, 0, 160, 160 }, { -1500, -750, 70, 65 }, gCollidersArray, ROCK_COL_ID)) return false;
		for (int i = 0; i < playersCount; ++i) {
			players[i] = (character*)malloc(sizeof(character));
		}
		if (!characterInit(players[LocalPlayer], gSpriteTexture, { WIDTH_w / 2, HEIGHT_w / 2, 60, 85 }, { WIDTH_w / 2 + 10, HEIGHT_w / 2 + 85 - 25, 40, 25 },
			{ WIDTH_w / 2, HEIGHT_w / 2, 60, 85 }, { 0, 0, WIDTH_w, HEIGHT_w }, gCollidersArray)) return false;
		if (!initGameItem(&players[LocalPlayer]->trap, loadTexture("trap.png", &gRenderer),
			{ players[LocalPlayer]->model.posRect.x, players[LocalPlayer]->model.posRect.y, 100, 100 }, { 0,0,600,600 },
			{ players[LocalPlayer]->model.posRect.x, players[LocalPlayer]->model.posRect.y, 100, 100 }, ActivateTrap, gCollidersArray, TRAP_COL_ID)) return false;
		if (!initGameItem(&players[LocalPlayer]->sword, loadTexture("slash3.png", &gRenderer), { 0,0,0,0 }, { 0,0,0, 0 }, { 0 }, ActivateSword, gCollidersArray, SWORD_COL_ID)) return false;
		players[LocalPlayer]->hasSword = true;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				players[LocalPlayer]->spriteClips[i][j] = gSpriteClips[i][j];
			}
		}
		SDL_PollEvent(&event);
		InitDateBase(&gDataBase);
		/*AddStructureInDateBase("")*/
	}
	return true;
}

int LaunchGame(myServer* server, myClient* client) {
	while (true) {
		system("cls");
		printf("					   --------------------------\n					   |Welcome to EFK Launcher!|\n				  	   --------------------------\n\n");
		printf("	 Press 1 to HOST Game              Press 2 to CONNECT to Game              Press -1 to EXIT Game\n\n Enter your decision: ");
		int choice = 0;
		scanf("%d", &choice);
		if (choice == 1 || choice == 2) {
			system("cls");
			char IP[15];
			printf("Press 0 to return back\n\nEnter server IP:\n");
			fgets(IP, 15, stdin);
			fgets(IP, 15, stdin);
			size_t ipSize = strlen(IP);
			if (IP[ipSize - 1] == '\n') IP[ipSize - 1] = '\0';
			if (!strcmp("0\n", IP)) {
				continue;
			}
			printf("Enter server Port:\n");
			int port = 0;
			scanf("%d", &port);
			if (port == 0 || port >= 100000) {
				continue;
			}
			if (choice == 1) {
				if (StartServer(server, (const char*)IP, (const int)port) == SUCCESSFUL_SERVER_INSTALATION) return HOST;
				else {
					printf("Error creating server!\n");
					continue;
				}
			}
			else {
				if (ConnectToServer(client, (const char*)IP, (const int)port) == SUCCESSFUL_CLIENT_INSTALATION) return CLIENT;
				else {
					printf("Error connecting to server!\n");
					continue;
				}
			}
		}
		else if (choice == -1) return -1;
	}
}

void GetData(enum DataType dataType) {
	if (dataType == CLIENT){}
	if (dataType == HOST) {
		//Обновляем все структуры игроков
	}
}

void SendData(enum DataType dataType) {
	if (dataType == CLIENT) {
		//client.sentData = StructureToString();
		// Генерирует запросы 

	}
	if (dataType == HOST) {
		//СОхраняем в БД свою структуру
		//
	}
}

void Drawing() {
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, gBackground, players[LocalPlayer]->camera, NULL);
	DrawLabirint(gRenderer, players[LocalPlayer]->camera, &gMatrix);
	for (int i = 0; i < playersCount; ++i) { if (players[i]->trap.isActive) RenderObject(&players[i]->trap.itemModel, gRenderer); }
	RenderObject(&rockDown, gRenderer);
	RenderObject(&sampleRock, gRenderer);
	for (int i = playersCount - 1; i >= LocalPlayer; --i) {
		if (players[i]->hasSword && players[i]->sword.isActive) {
			players[i]->model.srcRect = players[i]->spriteClips[players[i]->spriteNumber[0]][1];
			if (players[i]->spriteNumber[0] == KEY_PRESS_SURFACE_UP) {
				AttackSword(&players[i]->sword, gRenderer, 0, players[i]->spriteNumber[0]);
				RenderObject(&players[i]->model, gRenderer);
			}
			else {
				RenderObject(&players[i]->model, gRenderer);
				AttackSword(&players[i]->sword, gRenderer, 0, players[i]->spriteNumber[0]);
			}
		}
		else {
			players[i]->model.srcRect = players[i]->spriteClips[players[i]->spriteNumber[0]][(players[i]->spriteNumber[1] / 8) % 4];
			RenderObject(&players[i]->model, gRenderer);
		}
	}
	SDL_Rect staminaLine = { 10,40, 300, 20 };
	SDL_Rect hpLine = { 10, 10, 300, 20 };
	SDL_SetRenderDrawColor(gRenderer, 0xA0, 0xA0, 0xA0, 0x00);
	SDL_RenderFillRect(gRenderer, &hpLine);
	SDL_RenderFillRect(gRenderer, &staminaLine);
	hpLine.w = players[LocalPlayer]->HP * 3;
	staminaLine.w = players[LocalPlayer]->stamina * 3;
	if (!players[LocalPlayer]->canRun) SDL_SetRenderDrawColor(gRenderer, 0xDA, 0xFB, 0x80, 0xFF);
	else	SDL_SetRenderDrawColor(gRenderer, 0x0A, 0xFB, 0x80, 0xFF);
	SDL_RenderFillRect(gRenderer, &staminaLine);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x55, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, &hpLine);
	RenderObject(&rockUp, gRenderer);
	SDL_RenderPresent(gRenderer);
}

bool HandleInput(SDL_Event e, double velCoef ) {
	SDL_PollEvent(&e);
	if (e.type == SDL_QUIT) {
			return false;
	}
	const Uint8* movement = SDL_GetKeyboardState(NULL);
	if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
		if (players[LocalPlayer]->hasSword && (Timer_GetTicks(&players[LocalPlayer]->sword.delay) >= 1000) && e.button.state == SDL_PRESSED) {
			if (players[LocalPlayer]->canRun && players[LocalPlayer]->stamina > 40) {
				players[LocalPlayer]->sword.ItemFunc(&players[LocalPlayer]->sword, players[LocalPlayer]->spriteNumber[0], &players[LocalPlayer]->model.posRect);
				Mix_PlayChannel(1, gSwordAttackChunk, 0);
				players[LocalPlayer]->stamina -= 40;
				if (players[LocalPlayer]->stamina < 10) players[LocalPlayer]->canRun = false;
			}
		}
	}
	if (players[LocalPlayer]->sword.isActive) velCoef = 0;
	HandleMovement(players, movement, objs, objNumber, playersCount, velCoef, gCollidersArray, &gMatrix);
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