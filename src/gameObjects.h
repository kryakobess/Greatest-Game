#ifndef anObject
#define anObject

#include "GameEnv.h"

#define SPRITE_NUMBER 4
#define VELOCITY 3

const enum CollisionStatus {
	CollisionModel,
	Item,
	bodyDamage,
	weaponRange
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
	SDL_Rect* collisionBox;
	SDL_Texture* texture;
}gameObj;

typedef struct gameItem {
	gameObj itemModel;
	void(*ItemFunc)(gameItem*, bool, SDL_Rect);
	Timer delay;
	bool isActive;
}gameItem;

typedef struct character {
	gameObj model;
	gameItem trap;
	gameItem sword;
	SDL_Rect* hitBox;
	SDL_Rect spriteClips[KEY_PRESS_SURFACE_TOTAL][SPRITE_NUMBER];
	SDL_Rect* camera;
	bool canRun;
	double stamina;
	double VelCoef;
}character;


bool initGameObject(gameObj* obj, SDL_Texture* lTexture, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect cBox);
void FreeObj(gameObj* obj);
void RenderObject(gameObj* obj, SDL_Renderer* renderer);
bool isCollided(SDL_Rect a, SDL_Rect b);

bool initGameItem(gameItem* i, SDL_Texture* t, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect cBox, void(*func)(gameItem*, bool, SDL_Rect));
void ActivateTrap(gameItem* trap, bool keyFlag, SDL_Rect);

bool characterInit(character* c, SDL_Texture* t, SDL_Rect pos, SDL_Rect cBox, SDL_Rect hitBox, SDL_Rect camera);
void HandleMovement(character* c[], const Uint8* move, size_t lastEvent[2], gameObj* objs[], int objCount, int playersCount, double velCoef);
void SaveObjPosition(gameObj* objs[], int objCount, int yShift, int xShift);
bool CheckAllCollisions(character* c, gameObj* objs[], int objCount, int flag);

#endif // !anObject
