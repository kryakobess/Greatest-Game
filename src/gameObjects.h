#ifndef anObject
#define anObject

#include "GameEnv.h"

#define SPRITE_NUMBER 4
#define VELOCITY 8

const enum CollisionStatus {

};

const enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

typedef struct gameObj {
	SDL_Rect* posRect;
	SDL_Rect srcRect;
	SDL_Rect** hitBoxes;
	SDL_Texture* texture;
}gameObj;

typedef struct character {
	gameObj model;
	SDL_Rect spriteClips[KEY_PRESS_SURFACE_TOTAL][SPRITE_NUMBER];
	SDL_Rect* camera;
	float curVelY, curVelX;
}character;

typedef struct Timer {
	Uint32 startTicks;
	Uint32 pausedTicks;
	bool paused;
	bool started;
}Timer;

void Timer_Init(Timer* t);
void Timer_Start(Timer* t);
void Timer_Stop(Timer* t);
void Timer_Pause(Timer* t);
void Timer_Unpause(Timer* t);
Uint32 Timer_GetTicks(Timer* t);

bool initGameObject(gameObj* obj, SDL_Texture* lTexture, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect hitbCfg);
void FreeObj(gameObj* obj);
void RenderObject(gameObj* obj, SDL_Renderer* renderer);
bool isCollided(SDL_Rect a, SDL_Rect b);

bool characterInit(character* c, SDL_Texture* t, SDL_Rect pos, SDL_Rect hitBox, SDL_Rect camera);
void HandleMovement(character* c, const Uint8* move, size_t lastEvent[2], gameObj* objs[], int objCount);
void SaveObjPosition(gameObj* objs[], int objCount, int yShift, int xShift);
bool CheckAllCollisions(character* c, gameObj* objs[], int objCount);

#endif // !anObject
