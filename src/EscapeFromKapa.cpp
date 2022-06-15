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
SDL_Texture* gSpriteTexture[ASSETS_TOTAL] = { 0 };
SDL_Texture* gBackground = NULL;
gameObj rockUp; gameObj rockDown; gameObj sampleRock;
gameObj* objs[] = { &rockUp, &rockDown, &sampleRock };
int objNumber = 3;
int BG_WIDTH = 3800;
int BG_HEIGHT = 2170;
SDL_Rect camera = { BG_WIDTH / 2, BG_HEIGHT / 2, WIDTH_w, HEIGHT_w };
character* players[MAX_PLAYER_COUNT];
SDL_Event event;
DateBase gDataBase;
char gMyLogin[20];
myServer gServer; myClient gClient;
int playerCount = 1;
char playerNames[MAX_PLAYER_COUNT][MAX_LOGIN_SIZE];
int LocalPlayer = 0;

void DataProcessing(char* received, char* transmit) {
	/*SPPN — Send Player Position by Name
	SMP — Send My Position 
	SPC — Send Players' Count*/
	char task[5] = {0};
	sscanf(received, "{%[A-Z ]}", task);
	//Saving structures from Data Base in transmit variable: "<name> <structure>". 
	//Then server transmit information about chosen player(by name) to client who requested it
	// Receive format: "{TASK}[Name]"
	//Transmit format: "{TASK}/Name/[Size of Structure][Structure]" Structure is not in string format! It is char array.
	/*if (!strcmp("SPPN", task)){
		int trnLen = 0;
		char name[128] = { 0 };
		sscanf(received, "{%[A-Z ]}[%[a-zA-Z0-9_ ]]", task, name);
		int id = getIDStructure(name, &gDataBase);
		sprintf(transmit, "{%s}/%s/[%d][", task, name, gDataBase.sizesStructure[id]);
		int tLen = strlen(transmit);
		int i = 0;
		for (i = 0; i < gDataBase.sizesStructure[id]; ++i) {
			transmit[tLen + i] = gDataBase.stringStructure[id][i];
		}
		transmit[tLen + i] = ']';
	}*/
	//In receive variable there is a string of the current client's structure. We parse the string var and then save this structure in server's Data Base. 
	// If login do not exist in Data Base, we create new client's  row
	//Receive format: "{TASK}[LoginName][Size of Structure][structure]"
	//Transmit format: "{TASK}[Answer]"
	if (!strcmp("SMP", task)) {
		char name[128] = { 0 };
		int sizeStr = 0;
		sscanf(received, "{%[A-Z ]}[%[a-zA-Z0-9_ ]][%d][",task, name, &sizeStr);
		int ID = getIDStructure(name, &gDataBase);
		if (ID == -1) {
			if (gDataBase.countStructure == MAX_STRUCTURE_COUNT) {
				strcpy(transmit, "{SMP}[ConnectionFail]");
			}
			else if (!(gDataBase.nameStructure[gDataBase.countStructure] = (char*)calloc(strlen(name) + 1, sizeof(char)))) {
				strcpy(transmit, "{SMP}[ConnectionFail]");
			}
			else if (!(gDataBase.stringStructure[gDataBase.countStructure] = (char*)calloc(sizeStr + 1, sizeof(char)))) {
				strcpy(transmit, "{SMP}[ConnectionFail]");
			}
			else {
				char sPointerBuf[256] = { 0 };
				sprintf(sPointerBuf, "{%s}[%s][%d][", task, name, sizeStr);
				int sPointer = strlen(sPointerBuf);
				sprintf(gDataBase.nameStructure[gDataBase.countStructure], "%s", name);
				for (int i = 0; i < sizeStr; ++i) {
					gDataBase.stringStructure[gDataBase.countStructure][i] = received[i + sPointer];
				}
				gDataBase.sizesStructure[gDataBase.countStructure] = sizeStr;
				gDataBase.countStructure++;
				strcpy(transmit, "{SMP}[Connected]");
				printf("I connect %s to server\n", name);
			}
		}
		else {
			char sPointerBuf[256] = { 0 };
			sprintf(sPointerBuf, "{%s}[%s][%d][", task, name, sizeStr);
			int sPointer = strlen(sPointerBuf);
			gDataBase.stringStructure[ID] = (char*)realloc(gDataBase.stringStructure[ID], (size_t)sizeStr);
			for (int i = 0; i < sizeStr; ++i) {
				gDataBase.stringStructure[ID][i] = received[sPointer + i];
			}
			gDataBase.sizesStructure[ID] = sizeStr;
			strcpy(transmit, "{SMP}[Data_Sent]");
		}
	}
	//We check how many players are connected to server and send to client ASCII code of players count then we send names from data base
	// Receive format: "{TASK}/ClientPlayersCount/"
	//Transmit format: "{TASK}/PlayersCount/[P1_name][P2_name][PN_name]"
	/*if (!strcmp("SPC", task)) {
		int curClientCount = 0;
		sscanf(received, "{%[A-Z ]}/%d/", task, &curClientCount);
		if (curClientCount != gDataBase.countStructure) {
			sprintf(transmit, "{SPC}/%d/[", gDataBase.countStructure);
			int trnLen = 0;
			for (int i = 0; i < gDataBase.countStructure; ++i) {
				strcat(transmit, gDataBase.nameStructure[i]);
				trnLen = strlen(transmit);
				transmit[trnLen] = ']';
				transmit[trnLen + 1] = '[';
			}
			transmit[trnLen + 1] = '\0';
		}
		else {
			sprintf(transmit, "{SPC}/%d/", -1);
		}
	}*/
}

void DataAcceptence(char* received)
{
	char task[5] = { 0 };
	sscanf(received, "{%[A-Z ]}", task);
	/*if (!strcmp("SPPN", task)) {
		int sizeStructure = 0;
		char name[128] = { 0 };
		sscanf(received, "{%[A-Z ]}/%[a-zA-Z0-9 ]/[%d]", task, name, &sizeStructure);
		char buf[100];
		sprintf(buf, "{%s}/%s/[%d][", task, name, sizeStructure);
		int shiftBeforeStructure = strlen(buf);
		char* structure = (char*)calloc(sizeStructure, sizeof(char));
		for (int i = 0; i < sizeStructure; i++)
			structure[i] = received[shiftBeforeStructure + i];
		for (int id = 0; id < playerCount; id++)
		{
			if (!strcmp(name, playerNames[id]))
			{
				if (id != LocalPlayer)
				{
					Collider* feet = players[id]->feetCol;
					Collider* body = players[id]->model.body;
					Collider* trap = players[id]->trap.itemModel.body;
					LoadStructureFromString((void**)&players[id], sizeof(character), structure);
					if (id != LocalPlayer) {
						players[id]->model.posRect.x = players[id]->camera.x + players[id]->model.posRect.x - players[LocalPlayer]->camera.x;
						players[id]->model.posRect.y = players[id]->camera.y + players[id]->model.posRect.y - players[LocalPlayer]->camera.y;
					}
					players[id]->feetCol = feet;
					players[id]->model.body = body;
					players[id]->trap.itemModel.body = trap;

					players[id]->model.texture = gSpriteTexture[players[id]->model.asset];
				}
				break;
			}
		}
	}*/
	if (!strcmp("SMP", task)) {
		char status[10] = { 0 };
		sscanf(received, "{%[A-Z ]}[%[a-zA-Z_ ]", task, status);
		if (!strcmp(status, "ConnectionFail"))
		{
			printf("ConnectionFail\n");
		}
		if (!strcmp(status, "Connected"))
		{
			printf("I connected to server\n");
		}
		if (!strcmp(status, "Data_Sent"))
		{
			printf("Data_Sent\n");
		}
	}
	/*if (!strcmp("SPC", task)) {
		int clientCount;
		sscanf(received, "{%[A-Z ]}/%d/", task, &clientCount);
		if (clientCount != -1)
		{
			playerCount = clientCount;
			for (int i = 0; i < playerCount; i++)
			{
				if (players[i] == NULL)
				{
					players[i] = (character*)calloc(1, sizeof(character));
					BoxCollider* ptr;
					AddColliderInArray(gCollidersArray, players[i]->feetCol = CreateCollider(ptr = CreateBoxCollider({ 0,0,0,0 }), BOX, ONLINE_PLAYER_COL_ID));
					players[i]->feetCol->collider = ptr;

					AddColliderInArray(gCollidersArray, players[i]->model.body = CreateCollider(ptr = CreateBoxCollider({ 0,0,0,0 }), BOX, ONLINE_PLAYER_BODY_COL_ID));
					players[i]->model.body->collider = ptr;

					AddColliderInArray(gCollidersArray, players[i]->trap.itemModel.body = CreateCollider(ptr = CreateBoxCollider({ 0,0,0,0 }), BOX, ONLINE_TRAP_COL_ID));
					players[i]->trap.itemModel.body->collider = ptr;

					players[i]->model.texture = gSpriteTexture[players[i]->model.asset];
				}
			}
			char buf[100];
			int c = strlen(task);
			sprintf(buf, "{%s}/%d/", task, clientCount);
			int shift = strlen(buf);
			for (int nameID = 0; nameID < clientCount; nameID++)
			{
				shift++;
				char name[MAX_LOGIN_SIZE] = {0};
				int i = 0;
				while (received[shift + i] != ']')
				{
					name[i] = received[shift + i];
					i++;
				}
				shift += i;
				name[i] = '\0';
				shift++;
				strcpy(playerNames[nameID], name);
				if (!strcmp(playerNames[nameID], gMyLogin))
				{
					LocalPlayer = nameID;
				}
			}
		}
	}*/
}

void* SendData(void* arg) {
	while (true) {
		char* structure;
		sprintf(gClient.sentData, "{SPC}/%d/\0", playerCount);
		while (strlen(gClient.sentData) != 0);
		SaveStructureToString(players[LocalPlayer], sizeof(character), &structure);
		sprintf(gClient.sentData, "{SMP}[%s][%d][", gMyLogin, sizeof(character));
		int sentLen = strlen(gClient.sentData);
		int i = 0;
		for (i = 0; i < sizeof(character); i++) {
			gClient.sentData[sentLen + i] = structure[i];
		}
		gClient.sentData[sentLen + i] = ']';
		while (strlen(gClient.sentData) != 0);
		for (i = 0; i < playerCount; ++i) {
			if (i != LocalPlayer) {
				sprintf(gClient.sentData, "{SPPN}[%s]\0", playerNames[i]);
				while (strlen(gClient.sentData) != 0);
			}
		}
	}
	return (void*)NULL;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	gSpriteTexture[A_E_ELF] = loadTexture("E_elf.png", &gRenderer);
	if (gSpriteTexture[A_E_ELF] == NULL) {
		printf("A_E_ELF texture loading fail!");
		success = false;
	}
	gSpriteTexture[A_BOY_1] = loadTexture("boy_1.png", &gRenderer);
	if (gSpriteTexture[A_BOY_1] == NULL) {
		printf("A_BOY_1 texture loading fail!");
		success = false;
	}
	gSpriteTexture[A_BOY_2] = loadTexture("boy_2.png", &gRenderer);
	if (gSpriteTexture[A_BOY_2] == NULL) {
		printf("A_BOY_2 texture loading fail!");
		success = false;
	}
	gSpriteTexture[A_GIRL_1] = loadTexture("girl_1.png", &gRenderer);
	if (gSpriteTexture[A_GIRL_1] == NULL) {
		printf("A_GIRL_1 texture loading fail!");
		success = false;
	}
	gSpriteTexture[A_GIRL_2] = loadTexture("girl_2.png", &gRenderer);
	if (gSpriteTexture[A_GIRL_2] == NULL) {
		printf("A_GIRL_2 texture loading fail!");
		success = false;
	}
	gSpriteTexture[A_MALE_1] = loadTexture("male_1.png", &gRenderer);
	if (gSpriteTexture[A_MALE_1] == NULL) {
		printf("A_MALE_1 texture loading fail!");
		success = false;
	}
	gSpriteTexture[A_MALE_2] = loadTexture("male_2.png", &gRenderer);
	if (gSpriteTexture[A_MALE_2] == NULL) {
		printf("A_MALE_2 texture loading fail!");
		success = false;
	}
	gSpriteTexture[A_FMALE_1] = loadTexture("fmale_1.png", &gRenderer);
	if (gSpriteTexture[A_FMALE_1] == NULL) {
		printf("A_FMALE_1 texture loading fail!");
		success = false;
	}
	gSpriteTexture[A_E_GHOST] = loadTexture("e_girl.png", &gRenderer);
	if (gSpriteTexture[A_E_GHOST] == NULL) {
		printf("A_E_GHOST texture loading fail!");
		success = false;
	}
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i) {
		for (int j = 0; j < SPRITE_NUMBER; ++j) {
			gSpriteClips[i][j].w = 32;
			gSpriteClips[i][j].h = 32;
			if (j > 0) gSpriteClips[i][j].x = gSpriteClips[i][j].w + gSpriteClips[i][j - 1].x;
			else gSpriteClips[i][j].x = 0;
			if (i > 0) gSpriteClips[i][j].y = gSpriteClips[i][j].h + gSpriteClips[i - 1][j].y;
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
		gCollidersArray->collisionMatrix[PLAYER_COL_ID][ONLINE_TRAP_COL_ID] = true;
		gCollidersArray->collisionMatrix[PLAYER_COL_ID][ONLINE_PLAYER_COL_ID] = true;

		if(!InitCreateLabirint(&gMatrix, gCollidersArray)) return false;
		BG_WIDTH = gMatrix.countCol* WIDTH_TILE;
		BG_HEIGHT = gMatrix.countRow * HEIGHT_TILE;
		if (!initGameObject(&rockUp, loadTexture("rock.png", &gRenderer), { 800, 450, 70, 65 }, { 0,0,160,80 }, { 800, 450, 70, 65 }, gCollidersArray, ROCK_COL_ID)) return false;
		if (!initGameObject(&rockDown, rockUp.texture, { 800, 450 + 65, 70, 65 }, { 0, 80, 160, 80 }, { 800, 450 + 65, 70, 65 }, gCollidersArray, ROCK_COL_ID)) return false;
		if (!initGameObject(&sampleRock, rockUp.texture, { -1500, -750, 70, 65 }, { 0, 0, 160, 160 }, { -1500, -750, 70, 65 }, gCollidersArray, ROCK_COL_ID)) return false;
		players[LocalPlayer] = (character*)malloc(sizeof(character));
		if (!characterInit(players[LocalPlayer], gSpriteTexture[A_GIRL_1], {WIDTH_w / 2, HEIGHT_w / 2, 60, 85}, {WIDTH_w / 2 + 10, HEIGHT_w / 2 + 85 - 25, 40, 25},
			{ WIDTH_w / 2, HEIGHT_w / 2, 60, 85 }, { 0, 0, WIDTH_w, HEIGHT_w }, gCollidersArray)) return false;
		players[LocalPlayer]->model.asset = (AssetStatus)(rand() % (ASSETS_TOTAL - 2));
		if (!initGameItem(&players[LocalPlayer]->trap, loadTexture("trap.png", &gRenderer),
			{ players[LocalPlayer]->model.posRect.x, players[LocalPlayer]->model.posRect.y, 100, 100 }, { 0,0,600,600 },
			{ players[LocalPlayer]->model.posRect.x, players[LocalPlayer]->model.posRect.y, 100, 100 }, ActivateTrap, gCollidersArray, TRAP_COL_ID)) return false;
		if (!initGameItem(&players[LocalPlayer]->sword, loadTexture("slash3.png", &gRenderer), { 0,0,0,0 }, { 0,0,0, 0 }, { 0 }, ActivateSword, gCollidersArray, ONLINE_SWORD_COL_ID)) return false;
		players[LocalPlayer]->hasSword = true;
		for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i) {
			for (int j = 0; j < SPRITE_NUMBER; ++j) {
				players[LocalPlayer]->spriteClips[i][j] = gSpriteClips[i][j];
			}
		}
		SDL_PollEvent(&event);
		if (dataType == HOST) {
			InitDateBase(&gDataBase);
			AddStructureInDateBase(gMyLogin, players[LocalPlayer], sizeof(character), &gDataBase);
		}
		strcpy(playerNames[LocalPlayer], gMyLogin);
		if (dataType == CLIENT) {
			pthread_t sendData;
			int status = pthread_create(&sendData, NULL, SendData, (void*)NULL);
			pthread_detach(sendData);
		}
	}
	return true;
}

int LaunchGame() {
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
			if (!strcmp("0", IP)) {
				continue;
			}
			printf("Enter server Port:\n");
			int port = 0;
			scanf("%d", &port);
			if (port == 0 || port >= 100000) {
				continue;
			}

			printf("Enter your Login(Less then 21 characters):\n");
			fgets(gMyLogin, MAX_LOGIN_SIZE+1, stdin);
			int lgLen = 0;
			do {
				fgets(gMyLogin, MAX_LOGIN_SIZE + 1, stdin);
				lgLen = strlen(gMyLogin);
				gMyLogin[lgLen-1] = '\0';
			} while (lgLen > MAX_LOGIN_SIZE || lgLen == 0 || gMyLogin[0] == '\0');

			if (choice == 1) {
				if (StartServer(&gServer, (const char*)IP, (const int)port) == SUCCESSFUL_SERVER_INSTALATION) {
					gServer.DataProcessing = DataProcessing;
					return HOST;
				}
				else {
					printf("Error creating server!\n");
					continue;
				}
			}
			else {
				if (ConnectToServer(&gClient, (const char*)IP, (const int)port) == SUCCESSFUL_CLIENT_INSTALATION) {
					gClient.DataAcceptance = DataAcceptence;
					return CLIENT;
				}
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
		if (playerCount != gDataBase.countStructure)
		{
			playerCount = gDataBase.countStructure;
			for (int i = 0; i < playerCount; i++)
			{
				if (players[i] == NULL)
				{
					strcpy(playerNames[i], gDataBase.nameStructure[i]);
					players[i] = (character*)calloc(1, sizeof(character));
					BoxCollider* ptr;
					AddColliderInArray(gCollidersArray, players[i]->feetCol = CreateCollider(ptr = CreateBoxCollider({ 0,0,0,0 }), BOX, ONLINE_PLAYER_COL_ID));
					players[i]->feetCol->collider = ptr;

					AddColliderInArray(gCollidersArray, players[i]->model.body = CreateCollider(ptr = CreateBoxCollider({ 0,0,0,0 }), BOX, ONLINE_PLAYER_BODY_COL_ID));
					players[i]->model.body->collider = ptr;

					AddColliderInArray(gCollidersArray, players[i]->trap.itemModel.body = CreateCollider(ptr = CreateBoxCollider({ 0,0,0,0 }), BOX, ONLINE_TRAP_COL_ID));
					players[i]->trap.itemModel.body->collider = ptr;

					players[i]->model.texture = gSpriteTexture[players[i]->model.asset];
				}
			}
		}
		for (int i = 0; i < playerCount; i++)
		{
			Collider* feet = players[i]->feetCol;
			Collider* body = players[i]->model.body;
			Collider* trap = players[i]->trap.itemModel.body;
			GetStructureFromDateBase(playerNames[i], (void**)&players[i], sizeof(character), &gDataBase);
			if (i != LocalPlayer) {
				players[i]->model.posRect.x = players[i]->camera.x + players[i]->model.posRect.x - players[LocalPlayer]->camera.x;
				players[i]->model.posRect.y = players[i]->camera.y + players[i]->model.posRect.y - players[LocalPlayer]->camera.y;
			}
			players[i]->feetCol = feet;
			players[i]->model.body = body;
			players[i]->trap.itemModel.body = trap;
			players[i]->model.texture = gSpriteTexture[players[i]->model.asset];
		}
	}
}

void Drawing() {
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, gBackground, &players[LocalPlayer]->camera, NULL);
	DrawLabirint(gRenderer, &players[LocalPlayer]->camera, &gMatrix);
	for (int i = 0; i < playerCount; ++i) { if (players[i]->trap.isActive) RenderObject(&players[i]->trap.itemModel, gRenderer); }
	RenderObject(&rockDown, gRenderer);
	RenderObject(&sampleRock, gRenderer);
	for (int i = playerCount - 1; i >= LocalPlayer; --i) {
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
			players[i]->model.srcRect = players[i]->spriteClips[players[i]->spriteNumber[0]][(players[i]->spriteNumber[1] / 8) % SPRITE_NUMBER];
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
	HandleMovement(players, movement, objs, objNumber, playerCount, velCoef, gCollidersArray, &gMatrix, BG_WIDTH, BG_HEIGHT, LocalPlayer);
	return true;
}


void GameLoop(enum DataType dataType)
{
	Mix_PlayMusic(gMusic, -1);
	while (true) {
		GetData(dataType);
		Drawing();
		if (!HandleInput(event, 1)) break;
		if (dataType == HOST) RewriteStructureInDateBase(gMyLogin, players[LocalPlayer], sizeof(character), &gDataBase);
	}
}