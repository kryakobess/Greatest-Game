#ifndef ESCAPE_FROM_KAPA
#define ESCAPE_FROM_KAPA

#include "gameEnv.h"
#include "gameObjects.h"
#include "gameMap.h"
#include "Colliders.h"

enum ColliderID
{
	PLAYER_BODY_COL_ID,
	PLAYER_COL_ID,
	WALL_COL_ID,
	ROCK_COL_ID,
	TRAP_COL_ID,
	MAX_COUNT_COLLIDERS_ID
};

bool InitializeGameData(enum DataType dataType);
void GameLoop(enum DataType dataType);

#endif // !ESCAPE_FROM_KAPA
