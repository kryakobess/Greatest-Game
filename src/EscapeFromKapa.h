#ifndef ESCAPE_FROM_KAPA
#define ESCAPE_FROM_KAPA

#include "gameEnv.h"
#include "gameObjects.h"
#include "gameMap.h"
#include "Colliders.h"
#include "DateBase/DateBase.h"
#include "Network/netlib.h"

#define MAX_PLAYER_COUNT 10
#define MAX_LOGIN_SIZE 20
#define MAX_LAB_STR_LEN 20000

void DataAcceptence(char* received);
void DataProcessing(char* received, char* transmit);
int LaunchGame();
bool InitializeGameData(enum DataType dataType);
void GameLoop(enum DataType dataType);

#endif // !ESCAPE_FROM_KAPA
