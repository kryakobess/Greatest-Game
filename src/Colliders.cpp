#include "Colliders.h"

bool InitCollidersArray(CollidersArray* colArr, size_t maxIDcount)
{
	for (int id = 0; id < maxIDcount; id++)
	{
		colArr->collidersCount[id] = 0;
		colArr->collidersMemCount[id] = 1;
		if ((colArr->colliders[id] = (Collider*)calloc(1, sizeof(Collider))) == NULL) return false;
		if ((colArr->collisionMatrix[id] = (bool*)calloc(maxIDcount, sizeof(bool))) == NULL) return false;
	}
	return true;
}
bool AddColliderInArray(CollidersArray* colArr, Collider* col)
{
	if ((col == NULL) || (col->collider == NULL)) return false;
	if (colArr->collidersMemCount[col->id] <= colArr->collidersCount[col->id])
	{
		if ((colArr->colliders[col->id] = (Collider*)realloc(colArr->colliders[col->id], colArr->collidersMemCount[col->id] * 2 + 1)) == NULL) return false;
		colArr->collidersMemCount[col->id] = colArr->collidersMemCount[col->id] * 2 + 1;
	}
	colArr->colliders[col->id][colArr->collidersCount[col->id]].collider = col->collider;
	colArr->colliders[col->id][colArr->collidersCount[col->id]].colliderType = col->colliderType;
	colArr->colliders[col->id][colArr->collidersCount[col->id]].id = col->id;
	colArr->collidersCount[col->id]++;
}
Collider* CreateCollider(void* col, ColliderType colType, size_t id)
{
	Collider* collider = (Collider*)calloc(1, sizeof(Collider));
	collider->collider = col;
	collider->colliderType = colType;
	collider->id = id;
	return collider;
}
BoxCollider* CreateBoxCollider(SDL_Rect rect)
{
	BoxCollider* bCol = (BoxCollider*)calloc(1, sizeof(BoxCollider));
	bCol->rect.x = rect.x;
	bCol->rect.y = rect.y;
	bCol->rect.w = rect.w;
	bCol->rect.h = rect.h;
	return bCol;
}
CircleCollider* CreateCircleCollider(int x, int y, size_t r)
{
	CircleCollider* cCol = (CircleCollider*)calloc(1, sizeof(CircleCollider));
	cCol->r = r;
	cCol->x = x;
	cCol->y = y;
	return cCol;
}