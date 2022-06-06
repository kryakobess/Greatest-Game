#ifndef COLLIDERS
#define COLLIDERS

#include <SDL.h>
#include <math.h>
#include <stdio.h>

enum ColliderID
{
	PLAYER_BODY_COL_ID,
	PLAYER_COL_ID,
	WALL_COL_ID,
	ROCK_COL_ID,
	TRAP_COL_ID,
	MAX_COUNT_COLLIDERS_ID
};

typedef enum ColliderType
{
	BOX,
	/*CIRCLE,*/
	COUNT_COLLIDER_TYPES
}ColliderType;

typedef struct Collider
{
	bool active;
	void* collider;
	ColliderType colliderType;
	size_t id;
}Collider;

typedef struct BoxCollider
{
	SDL_Rect rect;
}BoxCollider;

//typedef struct CircleCollider
//{
//	int x, y;
//	size_t r;
//}CircleCollider;

typedef struct CollidersArray
{
	size_t maxIDcount;
	bool** collisionMatrix;
	size_t* collidersMemCount;
	size_t* collidersCount;
	Collider** colliders;
	bool** outCollisionMatrix;
}CollidersArray;

bool InitCollidersArray(CollidersArray** colArr, size_t maxIDcount);
bool AddColliderInArray(CollidersArray* colArr, Collider* col);
Collider* CreateCollider(void* col, ColliderType colType, size_t id);
BoxCollider* CreateBoxCollider(SDL_Rect rect);
//CircleCollider* CreateCircleCollider(int x, int y, size_t r);
void GetCollisionStates(CollidersArray* colArr);
void FreeCollidersArray(CollidersArray* colArr);

#endif