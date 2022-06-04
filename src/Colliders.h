#ifndef COLLIDERS
#define COLLIDERS

#include <SDL.h>

typedef enum ColliderType
{
	BOX,
	CIRCLE,
	COUNT_COLLIDER_TYPES
}ColliderType;

typedef struct Collider
{
	void* collider;
	ColliderType colliderType;
	size_t id;
}Collider;

typedef struct BoxCollider
{
	SDL_Rect rect;
}BoxCollider;

typedef struct CircleCollider
{
	int x, y;
	size_t r;
}CircleCollider;

typedef struct CollidersArray
{
	bool** collisionMatrix;
	size_t* collidersMemCount;
	size_t* collidersCount;
	Collider** colliders;
}CollidersArray;

bool InitCollidersArray(CollidersArray* colArr);
bool AddColliderInArray(CollidersArray* colArr, Collider* col);
Collider* CreateCollider(void* col, ColliderType colType, size_t id);
BoxCollider* CreateBoxCollider(SDL_Rect rect);
CircleCollider* CreateCircleCollider(int x, int y, size_t r);


#endif