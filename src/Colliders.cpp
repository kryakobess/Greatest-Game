#include "Colliders.h"

void InitCollidersArray(CollidersArray* colArr)
{
	colArr->collidersCount = 0;
}
bool AddColliderInArray(CollidersArray* colArr, Collider* col)
{
	if ((col == NULL) || (col->collider == NULL)) return false;

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
}
CircleCollider* CreateCircleCollider(int x, int y, size_t r)
{
	CircleCollider* cCol = (CircleCollider*)calloc(1, sizeof(CircleCollider));
	cCol->r = r;
	cCol->x = x;
	cCol->y = y;
	return cCol;
}