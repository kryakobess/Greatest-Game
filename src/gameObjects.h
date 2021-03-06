#ifndef anObject
#define anObject

#include "GameEnv.h"
#include "Colliders.h"
#include "gameMap.h"

#define SPRITE_NUMBER 3
#define VELOCITY 3

const enum AssetStatus {
	A_BOY_1,
	A_BOY_2,
	A_GIRL_1,
	A_GIRL_2,
	A_MALE_1,
	A_MALE_2,
	A_FMALE_1,
	A_E_ELF,
	A_E_GHOST,
	A_I_SWORD,
	A_I_SPIKES,
	ASSETS_TOTAL
};

const enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_TOTAL
};

typedef struct gameObj {
	SDL_Rect posRect;
	SDL_Rect srcRect;
	Collider* body;
	SDL_Texture* texture;
	AssetStatus asset;
}gameObj;

typedef struct gameItem {
	gameObj itemModel;
	void(*ItemFunc)(gameItem*, int, SDL_Rect*);
	Timer delay;
	bool isActive;
}gameItem;

typedef struct character {
	gameObj model;
	gameItem trap;
	gameItem sword;
	Collider* feetCol;
	SDL_Rect spriteClips[KEY_PRESS_SURFACE_TOTAL][SPRITE_NUMBER];
	SDL_Rect camera;
	size_t spriteNumber[2];
	bool hasSword;
	bool canRun;
	double stamina;
	int HP;
	double VelCoef;
}character;

bool initGameObject(gameObj * obj, SDL_Texture * lTexture, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect cBox, CollidersArray * colArr, enum ColliderID ID);
void ObjFree(gameObj* obj);
void RenderObject(gameObj* obj, SDL_Renderer* renderer);

//Functions for Items (Traps and Swords)

bool initGameItem(gameItem* i, SDL_Texture* t, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect cBox, void(*func)(gameItem*, int, SDL_Rect*), CollidersArray* colArr, enum ColliderID ID);
void ActivateTrap(gameItem* trap, int keyFlag, SDL_Rect* posRect);
void ActivateSword(gameItem* sword, int spriteNumber, SDL_Rect* posRect);
void AttackSword(gameItem* sword, SDL_Renderer* gRenderer, int delay, int spriteDirect);

//Character functions

bool characterInit(character* c, SDL_Texture* t, SDL_Rect pos, SDL_Rect cBox, SDL_Rect hitBox, SDL_Rect camera, CollidersArray* colArr);
void HandleMovement(character* c[], const Uint8* move, int objCount, int playersCount, double velCoef, CollidersArray* colArr, Matrix* matrix, int BG_WIDTH, int BG_HEIGHT, int LocalPlayer);
void SaveObjPosition(gameObj* objs[], int objCount, int yShift, int xShift);


#endif // !anObject