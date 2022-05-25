#ifndef anObject
#define anObject

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SPRITE_NUMBER 4
#define VELOCITY 3

const enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

typedef struct gameObj {
	SDL_Rect posRect;
	SDL_Rect srcRect;
	SDL_Rect hitBox;
	SDL_Texture* texture;
}gameObj;

typedef struct character {
	SDL_Rect* posRect;
	SDL_Rect spriteClips[KEY_PRESS_SURFACE_TOTAL][SPRITE_NUMBER];
	SDL_Rect* hitBox;
	SDL_Texture* texture;
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
void HandleMovement(character* c, const Uint8* move,  int HEIGHT_w, int WIDTH_w, int BG_HEIGHT, int BG_WIDTH, int* yShift, int* xShift, size_t lastEvent[2]);
void MoveCamera(character* camera, float velocity, int maxBgHeight, int maxBgWidth);
void SaveObjPosition(gameObj* freezedObjs[], int objCount, int yShift, int xShift);

#endif // !anObject
