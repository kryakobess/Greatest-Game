#ifndef ESCAPE_FROM_KAPA
#define ESCAPE_FROM_KAPA

#include "gameEnv.h"
#include "gameObjects.h"
#include "gameMap.h"
#include "Colliders.h"
#include "DateBase/DateBase.h"
#include "Network/netlib.h"

int LaunchGame(myServer* server, myClient* client);
bool InitializeGameData(enum DataType dataType);
void GameLoop(enum DataType dataType);

#endif // !ESCAPE_FROM_KAPA
