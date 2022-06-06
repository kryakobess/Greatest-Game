#include "gameObjects.h"

bool initGameObject(gameObj* obj, SDL_Texture* lTexture, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect cBox, CollidersArray* colArr, enum ColliderID ID) {
	obj->texture = lTexture;
	obj->posRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));

	BoxCollider* ptr;
	if (!AddColliderInArray(colArr, obj->body = CreateCollider(ptr = CreateBoxCollider(cBox), BOX, ID))) return false;
	obj->body->collider = ptr;

	if (obj->texture == NULL) {
		return false;
		printf("Texture loading error!");
	}
	*obj->posRect = posCfg;
	obj->srcRect = srcCfg;
	return true;
}

bool initGameItem(gameItem* i, SDL_Texture* t, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect cBox, void(*func)(gameItem*, bool, SDL_Rect), CollidersArray* colArr, enum ColliderID ID)
{
	if (!initGameObject(&i->itemModel, t, posCfg, srcCfg, cBox, colArr, ID)) return false;
	i->ItemFunc = func;
	Timer_Init(&i->delay);
	Timer_Start(&i->delay);
	i->isActive = false;
	return true;
}

void ActivateTrap(gameItem* trap, bool keyFlag, SDL_Rect posRect) {
 	if (trap->isActive && Timer_GetTicks(&trap->delay) <= 10000) {
		((BoxCollider*)trap->itemModel.body->collider)->rect = { trap->itemModel.posRect->x, trap->itemModel.posRect->y, trap->itemModel.posRect->w, trap->itemModel.posRect->h };
	}
	else if (keyFlag && Timer_GetTicks(&trap->delay) >= 1000) {
 		trap->isActive = true;
		Timer_Start(&trap->delay);
		trap->itemModel.posRect->x = posRect.x;
		trap->itemModel.posRect->y = posRect.y;
	}
	else {
		trap->isActive = false;
		trap->itemModel.posRect->x = posRect.x;
		trap->itemModel.posRect->y = posRect.y;
	}
}

void ReleaseSpikes(character* players[], int pCount, int velCoef, CollidersArray* colArr, SDL_Rect* camera) {
	for (int i = 0; i < pCount; ++i) {
		players[i]->trap.ItemFunc(&players[i]->trap, false, *players[i]->model.posRect);
		
		GetCollisionStates(colArr, camera);
		if (colArr->outCollisionMatrix[PLAYER_COL_ID][TRAP_COL_ID]&& players[i]->trap.isActive)
		{
			printf("Collision PLAYER WITH SPIKE\n");
			players[LocalPlayer]->VelCoef *= 0.35;
			break;
		}
		else
		{
			players[LocalPlayer]->VelCoef = velCoef;
		}
		
	}
}

bool characterInit(character* c, SDL_Texture* t, SDL_Rect pos, SDL_Rect cBox, SDL_Rect hitBox, SDL_Rect camera, CollidersArray* colArr)
{
	if ((c->feetCol = (Collider*)calloc(1, sizeof(Collider))) == NULL) return false;

	BoxCollider* ptr;
	if (!AddColliderInArray(colArr, (Collider*)(c->feetCol = CreateCollider(ptr = CreateBoxCollider(cBox), BOX, PLAYER_COL_ID)))) return false;
	c->feetCol->collider = ptr;

	c->camera = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (!initGameObject(&c->model, t, pos, {}, hitBox, colArr, PLAYER_BODY_COL_ID)) return false;
	*c->camera = camera;
	c->VelCoef = 1;
	c->sword = { 0 };
	c->trap = { 0 };
	c->spriteNumber[0] = KEY_PRESS_SURFACE_DOWN; c->spriteNumber[1] = 0;
	c->stamina = 100;
	c->canRun = true;
	return true;
}

void FreeObj(gameObj* obj)
{
	SDL_DestroyTexture(obj->texture);
	obj->texture = NULL;
	free(obj->posRect);
	obj->posRect = NULL;
}

void RenderObject(gameObj* obj, SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, obj->texture, &(obj->srcRect), (obj->posRect));
}

void SaveObjPosition(gameObj* objs[], int objCount, int yShift, int xShift) 
{
	for (int i = 0; i < objCount; i++) {
		if (objs[i]->posRect != NULL) {
			objs[i]->posRect->y -= yShift;
			objs[i]->posRect->x -= xShift;
		}
		if (objs[i]->body->active)	{
			((BoxCollider*)(objs[i]->body->collider))->rect.y -= yShift;
			((BoxCollider*)(objs[i]->body->collider))->rect.x -= xShift;
		}
	}
}

void SavePlayersPosition(character** p, int pCount, int yShift, int xShift) {
	for (int i = 0; i < pCount; ++i) {
		if (i != LocalPlayer) {
			((BoxCollider*)(p[i]->model.body->collider))->rect.x -= xShift;
			((BoxCollider*)(p[i]->model.body->collider))->rect.y -= yShift;
			((BoxCollider*)(p[i]->feetCol->collider))->rect.x -= xShift;
			((BoxCollider*)(p[i]->feetCol->collider))->rect.y -= yShift;
			p[i]->model.posRect->x -= xShift;
			p[i]->model.posRect->y -= yShift;
		}
		if (p[i]->trap.itemModel.posRect != NULL) {
			p[i]->trap.itemModel.posRect->x -= xShift;
			p[i]->trap.itemModel.posRect->y -= yShift;
		}
		/*if (p[i]->sword.itemModel.body->active) {
			((BoxCollider*)(p[i]->sword.itemModel.body->collider))->rect.x -= xShift;
			((BoxCollider*)(p[i]->sword.itemModel.body->collider))->rect.y -= yShift;
		}*/
		if (p[i]->sword.itemModel.posRect != NULL) {
			p[i]->sword.itemModel.posRect->x -= xShift;
			p[i]->sword.itemModel.posRect->y -= yShift;
		}
	}
}

void moveCharacter(character* c, int xShift, int yShift, int xPosShift, int yPosShift) {
	c->model.posRect->x += xPosShift; c->model.posRect->y += yPosShift;

	((BoxCollider*)(c->feetCol->collider))->rect.x += xPosShift;
	((BoxCollider*)(c->feetCol->collider))->rect.y += yPosShift;
	((BoxCollider*)(c->model.body->collider))->rect.x += xPosShift;
	((BoxCollider*)(c->model.body->collider))->rect.y += yPosShift;

	c->camera->x += xShift; c->camera->y += yShift;
}

void HandleMovement(character* c[], const Uint8* move, gameObj* objs[], int objCount, int playersCount, double velCoef, CollidersArray* colArr, SDL_Rect* camera)
{
	if (c[LocalPlayer]->stamina < 100) {
		c[LocalPlayer]->stamina += 0.5;
		if (c[LocalPlayer]->stamina >= 100) c[LocalPlayer]->canRun = true;
	}
	c[LocalPlayer]->VelCoef = velCoef;
	ReleaseSpikes(c, playersCount, velCoef, colArr, camera);

	int yShift = 0; int xShift = 0;
	int xPosShift = 0; int yPosShift = 0;
	const Uint8* currentKeyStates = move;
	if (currentKeyStates[SDL_SCANCODE_W]) {
		if (c[LocalPlayer]->camera->y > 0 && (abs(c[LocalPlayer]->model.posRect->y - HEIGHT_w / 2) <= 10)) yShift -= VELOCITY;
		else if (c[LocalPlayer]->model.posRect->y > 0) yPosShift -= VELOCITY;
		c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_UP; c[LocalPlayer]->spriteNumber[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_S]) {
		if (c[LocalPlayer]->camera->y < BG_HEIGHT && (abs(c[LocalPlayer]->model.posRect->y - HEIGHT_w / 2 )<= 10)) yShift += VELOCITY;
		else if (c[LocalPlayer]->model.posRect->y < HEIGHT_w - c[LocalPlayer]->model.posRect->h) yPosShift += VELOCITY;
		c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_DOWN; c[LocalPlayer]->spriteNumber[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_A]) {
		if (c[LocalPlayer]->camera->x > 0 && abs(c[LocalPlayer]->model.posRect->x - WIDTH_w / 2) <= 10) xShift -= VELOCITY;
		else if (c[LocalPlayer]->model.posRect->x > 0) xPosShift -= VELOCITY;
		c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_LEFT; c[LocalPlayer]->spriteNumber[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_D]) {
		if (c[LocalPlayer]->camera->x < BG_WIDTH && abs(c[LocalPlayer]->model.posRect->x - WIDTH_w / 2) <= 10) xShift += VELOCITY;
		else if (c[LocalPlayer]->model.posRect->x < WIDTH_w - c[LocalPlayer]->model.posRect->w) xPosShift += VELOCITY;
		c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_RIGHT; c[LocalPlayer]->spriteNumber[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_E]) {
		c[LocalPlayer]->trap.ItemFunc(&c[LocalPlayer]->trap, true, *c[LocalPlayer]->model.posRect);
	}
	if (currentKeyStates[SDL_SCANCODE_SPACE] && c[LocalPlayer]->stamina > 0 && c[LocalPlayer]->canRun) {
		c[LocalPlayer]->VelCoef *= 2;
		c[LocalPlayer]->stamina -= 1.5;
		if (c[LocalPlayer]->stamina < 5) c[LocalPlayer]->canRun = false;
	}

	xShift *= c[LocalPlayer]->VelCoef; yShift *= c[LocalPlayer]->VelCoef;
	xPosShift *= c[LocalPlayer]->VelCoef; yPosShift *= c[LocalPlayer]->VelCoef;
	moveCharacter(c[LocalPlayer], xShift, yShift, xPosShift, yPosShift);
	SaveObjPosition(objs, objCount, yShift, xShift);
	SavePlayersPosition(c, playersCount, yShift , xShift);

	GetCollisionStates(colArr, camera);
	for (int id = 0; id < colArr->maxIDcount; id++)
	{
		for (int id2 = 0; id2 < colArr->maxIDcount; id2++)
		{
			printf("%d", colArr->outCollisionMatrix[id][id2]);
		}
		printf("\n");
	}
	printf("\n\n\n");
	
	if (colArr->outCollisionMatrix[PLAYER_COL_ID][ROCK_COL_ID]) {
		if (c[LocalPlayer]->model.posRect->x != WIDTH_w / 2 || c[LocalPlayer]->model.posRect->y != HEIGHT_w / 2) {
			moveCharacter(c[LocalPlayer], 0, 0, -xPosShift, -yPosShift);
		}
		if ((abs(c[LocalPlayer]->model.posRect->x - WIDTH_w / 2) <= 10 || abs(c[LocalPlayer]->model.posRect->y - HEIGHT_w / 2) <= 10)) {
			moveCharacter(c[LocalPlayer], -xShift, -yShift, 0, 0);
			SavePlayersPosition(c, playersCount, -yShift, -xShift);
			SaveObjPosition(objs, objCount, -yShift, -xShift);
		}
	}
}