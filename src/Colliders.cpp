#include "Colliders.h"

bool InitCollidersArray(CollidersArray** colArr, size_t maxIDcount)
{
	if ((*colArr = (CollidersArray*)calloc(1, sizeof(CollidersArray))) == NULL) return false;
	(*colArr)->maxIDcount = maxIDcount;
	if (((*colArr)->colliders = (Collider**)calloc(maxIDcount, sizeof(Collider*))) == NULL)return false;
	if (((*colArr)->collisionMatrix = (bool**)calloc(maxIDcount, sizeof(bool*))) == NULL)return false;
	if (((*colArr)->outCollisionMatrix = (bool**)calloc(maxIDcount, sizeof(bool*))) == NULL)return false;
	if (((*colArr)->collidersCount = (size_t*)calloc(maxIDcount, sizeof(size_t))) == NULL) return false;
	if (((*colArr)->collidersMemCount = (size_t*)calloc(maxIDcount, sizeof(size_t))) == NULL) return false;

	for (int id = 0; id < maxIDcount; id++)
	{
		if (((*colArr)->colliders[id] = (Collider*)calloc(10, sizeof(Collider))) == NULL) return false;
		if (((*colArr)->collisionMatrix[id] = (bool*)calloc(maxIDcount, sizeof(bool))) == NULL) return false;
		for (int id2 = 0; id2 < maxIDcount; id2++) (*colArr)->collisionMatrix[id][id2] = false;
		if (((*colArr)->outCollisionMatrix[id] = (bool*)calloc(maxIDcount, sizeof(bool))) == NULL) return false;
		(*colArr)->collidersCount[id] = 0;
		(*colArr)->collidersMemCount[id] = 10;
	}
	return true;
}

//bool ReallocColliders(CollidersArray* colArr, Collider* col)
//{
//
//}

bool AddColliderInArray(CollidersArray* colArr, Collider* col)
{
	if ((col == NULL) || (col->collider == NULL)) return false;
	if (colArr->collidersMemCount[col->id] <= colArr->collidersCount[col->id])
	{
		if ((colArr->colliders[col->id] = (Collider*)realloc(colArr->colliders[col->id], colArr->collidersMemCount[col->id] * 2 + 1)) == NULL) return false;
		colArr->collidersMemCount[col->id] = colArr->collidersMemCount[col->id] * 2 + 1;
		
		printf("Error Memory with ID\n");
		return false;
		/*for (int id = 0; id < colArr->maxIDcount; id++)
		{
			printf("\n{Colliders count with ID=%d} = %d:\n", id, colArr->collidersCount[id]);
			for (int i = 0; i < colArr->collidersCount[id]; i++)
			{
				printf("{%d,%d,%d,%d} active=%d\n", ((BoxCollider*)colArr->colliders[id][i].collider)->rect.x, ((BoxCollider*)colArr->colliders[id][i].collider)->rect.y,
					((BoxCollider*)colArr->colliders[id][i].collider)->rect.w, ((BoxCollider*)colArr->colliders[id][i].collider)->rect.h,
					colArr->colliders[id][i].active);
				printf("ColType = %d\n", (ColliderType)colArr->colliders[id][i].colliderType);
			}
		}
		printf("\n\n\n");*/
	}
	colArr->colliders[col->id][colArr->collidersCount[col->id]].collider = col->collider;
	colArr->colliders[col->id][colArr->collidersCount[col->id]].colliderType = col->colliderType;
	colArr->colliders[col->id][colArr->collidersCount[col->id]].id = col->id;
	colArr->colliders[col->id][colArr->collidersCount[col->id]].active = col->active;
	colArr->collidersCount[col->id]++;
}

Collider* CreateCollider(void* col, ColliderType colType, size_t id)
{
	printf("Collider[id=%d]\n", id);
	Collider* collider = (Collider*)calloc(1, sizeof(Collider));
	collider->collider = col;
	collider->colliderType = colType;
	collider->id = id;
	collider->active = true;
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
//CircleCollider* CreateCircleCollider(int x, int y, size_t r)
//{
//	CircleCollider* cCol = (CircleCollider*)calloc(1, sizeof(CircleCollider));
//	cCol->r = r;
//	cCol->x = x;
//	cCol->y = y;
//	return cCol;
//}

bool CheckBoxes(SDL_Rect* a, SDL_Rect* b)
{
	int leftA = a->x;
	int rightA = a->x+ a->w;
	int topA = a->y;
	int bottomA = a->y + a->h;

	int leftB = b->x;
	int rightB = b->x + b->w;
	int topB = b->y;
	int bottomB = b->y + b->h;

	if (bottomA <= topB) return false;
	if (topA >= bottomB) return false;
	if (rightA <= leftB) return false;
	if (leftA >= rightB) return false;
	return true;
}

//bool pointInRect(int x, int y, SDL_Rect* rect)
//{
//	return ((x >= rect->x) && (x <= rect->x + rect->w) && (y >= rect->y) && (y <= rect->y + rect->h));
//}
//
//bool insideCircle(int x, int y, CircleCollider* cirCol)
//{
//
//}
//
//bool intersectCircle(CircleCollider* circle, int x1, int y1, int x2, int y2)
//{
//
//}
//
//bool CheckBoxCircle(BoxCollider* boxCol, CircleCollider* cirCol)
//{
//
//}
//
//bool CheckCircles(CircleCollider* c1, CircleCollider* c2)
//{
//	return (sqrt((c1->x - c2->x) * (c1->x - c2->x) + (c1->y - c2->y) * (c1->y - c2->y)) <= c1->r + c2->r);
//}

bool Collision(Collider* col1, Collider* col2)
{
	
	if ((col1->colliderType == BOX) && (col2->colliderType == BOX))
	{
		return CheckBoxes(&((BoxCollider*)col1->collider)->rect, &((BoxCollider*)col2->collider)->rect);
	}
	/*if ((col1->colliderType == BOX) && (col2->colliderType == CIRCLE))
	{

	}
	if ((col1->colliderType == CIRCLE) && (col2->colliderType == BOX))
	{

	}
	if ((col1->colliderType == CIRCLE) && (col2->colliderType == CIRCLE))
	{
		return CheckCircles((CircleCollider*)col1->collider, (CircleCollider*)col2->collider);
	}*/
	printf("Error collider Type!!!\n");
	return false;
}

void GetCollisionStates(CollidersArray* colArr)
{
	/*for (int id = 0; id < colArr->maxIDcount; id++)
	{
		printf("\n{Colliders count with ID=%d} = %d:\n", id, colArr->collidersCount[id]);
		for (int i = 0; i < colArr->collidersCount[id]; i++)
		{
			printf("{%d,%d,%d,%d} active=%d\n", ((BoxCollider*)colArr->colliders[id][i].collider)->rect.x, ((BoxCollider*)colArr->colliders[id][i].collider)->rect.y,
				((BoxCollider*)colArr->colliders[id][i].collider)->rect.w, ((BoxCollider*)colArr->colliders[id][i].collider)->rect.h,
				colArr->colliders[id][i].active);
			printf("ColType = %d\n", (ColliderType)colArr->colliders[id][i].colliderType);
		}
	}*/

	bool collision;
	for (int id = 0; id < colArr->maxIDcount; id++)
	{
		for (int id2 = id; id2 < colArr->maxIDcount; id2++)
		{
			if (colArr->collisionMatrix[id][id2] || colArr->collisionMatrix[id2][id])
			{
				collision = false;
				for (int i = 0; i < colArr->collidersCount[id]; i++)
				{
					for (int j = 0; j < colArr->collidersCount[id2]; j++)
					{
						if ((!((id == id2) && (i == j))) && (colArr->colliders[id][i].active) && (colArr->colliders[id2][j].active))
						{
							if (Collision(&colArr->colliders[id][i], &colArr->colliders[id2][j]))
							{
								collision = true;
								colArr->outCollisionMatrix[id][id2] = colArr->outCollisionMatrix[id2][id] = true;
								break;
							}
						}
					}
					if (collision) break;
				}
				colArr->outCollisionMatrix[id][id2] = colArr->outCollisionMatrix[id2][id] = collision;
			}
		}
	}
}

void FreeCollidersArray(CollidersArray* colArr)
{
	for (int id = 0; id < colArr->maxIDcount; id++)
	{
		free(colArr->collisionMatrix[id]);
		free(colArr->colliders[id]);
		free(colArr->outCollisionMatrix[id]);
	}
	free(colArr->collisionMatrix);
	free(colArr->colliders);
	free(colArr->outCollisionMatrix);

	free(colArr->collidersMemCount);
	free(colArr->collidersCount);
}
