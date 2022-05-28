#include "gameObjects.h"

bool initGameObject(gameObj* obj, SDL_Texture* lTexture, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect cBox) {
	obj->texture = lTexture;
	obj->posRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	obj->collisionBox = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (obj->texture == NULL) {
		return false;
		printf("Texture loading error!");
	}
	*obj->posRect = posCfg;
	obj->srcRect = srcCfg;
	*obj->collisionBox = cBox;
	return true;
}

bool initGameItem(gameItem* i, SDL_Texture* t, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect cBox, void(*func)(gameItem*, bool, SDL_Rect))
{
	if (!initGameObject(&i->itemModel, t, posCfg, srcCfg, cBox)) return false;
	i->ItemFunc = func;
	Timer_Init(&i->delay);
	Timer_Start(&i->delay);
	i->isActive = false;
	return true;
}

void ActivateTrap(gameItem* trap, bool keyFlag, SDL_Rect posRect) {
	if (trap->isActive && Timer_GetTicks(&trap->delay) <= 20000) {
		trap->itemModel.collisionBox = trap->itemModel.posRect;
	}
	else if (keyFlag && Timer_GetTicks(&trap->delay) >= 40000) {
		trap->isActive = true;
		Timer_Init(&trap->delay);
	}
	else {
		trap->isActive = false;
		trap->itemModel.collisionBox = {};
		trap->itemModel.posRect->x = posRect.x;
		trap->itemModel.posRect->y = posRect.y;
	}
}

bool characterInit(character* c, SDL_Texture* t, SDL_Rect pos, SDL_Rect cBox, SDL_Rect hitBox, SDL_Rect camera) 
{
	c->hitBox = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	c->camera = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (!initGameObject(&c->model, t, pos, {}, cBox)) return false;
	*c->hitBox = hitBox;
	*c->camera = camera;
	return true;
}

void FreeObj(gameObj* obj)
{
	SDL_DestroyTexture(obj->texture);
	obj->texture = NULL;
	free(obj->posRect);
	obj->posRect = NULL;
	free(obj->collisionBox);
	obj->collisionBox = NULL;
}

void RenderObject(gameObj* obj, SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, obj->texture, &(obj->srcRect), (obj->posRect));
}

bool isCollided(SDL_Rect a, SDL_Rect b)
{
	int leftA = a.x;
	int rightA = a.x + a.w;
	int topA = a.y;
	int bottomA = a.y + a.h;

	int leftB = b.x;
	int rightB = b.x + b.w;
	int topB = b.y;
	int bottomB = b.y + b.h;

	if (bottomA <= topB) return false;
	if (topA >= bottomB) return false;
	if (rightA <= leftB) return false;
	if (leftA >= rightB) return false;
	return true;
}

bool CheckAllCollisions(character* c, gameObj* objs[], int objCount, int flag) 
{
	for (int i = 0; i < objCount; ++i) {
		if (flag == CollisionModel && isCollided((*c->model.collisionBox), *objs[i]->collisionBox)) return true;
		else if (flag == weaponRange && isCollided(*c->hitBox, *objs[i]->collisionBox)) return true;
	}
	return false;
}

void SaveObjPosition(gameObj* objs[], int objCount, int yShift, int xShift) 
{
	for (int i = 0; i < objCount; i++) {
		objs[i]->posRect->y -= yShift;
		objs[i]->posRect->x -= xShift;
		objs[i]->collisionBox->y -= yShift;
		objs[i]->collisionBox->x -= xShift;
	}
}

void moveCharacter(character* c, int xShift, int yShift, int xPosShift, int yPosShift, double k) {
	c->model.posRect->x += xPosShift * k; c->model.posRect->y += yPosShift * k;
	c->hitBox->x += xPosShift * k; c->hitBox->y += yPosShift * k;
	c->model.collisionBox->x += xPosShift * k; c->model.collisionBox->y += yPosShift * k;
	c->camera->x += xShift * k; c->camera->y += yShift * k;
}

void HandleMovement(character** c, const Uint8* move, size_t lastEvent[2], gameObj* objs[], int objCount, int playersCount) 
{
	int yShift = 0; int xShift = 0;
	int xPosShift = 0; int yPosShift = 0;
	int moveCoef = 1;
	const Uint8* currentKeyStates = move;
	if (currentKeyStates[SDL_SCANCODE_W]) {
		if (c[LocalPlayer]->camera->y > 0 && (c[LocalPlayer]->model.posRect->y == HEIGHT_w / 2)) yShift -= VELOCITY;
		else if (c[LocalPlayer]->model.posRect->y > 0) yPosShift -= VELOCITY;
		lastEvent[0] = KEY_PRESS_SURFACE_UP; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_S]) {
		if (c[LocalPlayer]->camera->y < BG_HEIGHT && (c[LocalPlayer]->model.posRect->y == HEIGHT_w / 2)) yShift += VELOCITY;
		else if (c[LocalPlayer]->model.posRect->y < HEIGHT_w - c[LocalPlayer]->model.posRect->h) yPosShift += VELOCITY;
		lastEvent[0] = KEY_PRESS_SURFACE_DOWN; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_A]) {
		if (c[LocalPlayer]->camera->x > 0 && c[LocalPlayer]->model.posRect->x == WIDTH_w / 2) xShift -= VELOCITY;
		else if (c[LocalPlayer]->model.posRect->x > 0) xPosShift -= VELOCITY;
		lastEvent[0] = KEY_PRESS_SURFACE_LEFT; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_D]) {
		if (c[LocalPlayer]->camera->x < BG_WIDTH && c[LocalPlayer]->model.posRect->x == WIDTH_w / 2) xShift += VELOCITY;
		else if (c[LocalPlayer]->model.posRect->x < WIDTH_w - c[LocalPlayer]->model.posRect->w) xPosShift += VELOCITY;
		lastEvent[0] = KEY_PRESS_SURFACE_RIGHT; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_SPACE]) {
		c[LocalPlayer]->trap.ItemFunc(&c[LocalPlayer]->trap, true, {});
	}
	for (int i = 0; i < playersCount; ++i) {
		if (c[i]->trap.isActive) {
			c[i]->trap.ItemFunc(&c[i]->trap, false, *c[i]->model.posRect);
			if (isCollided(*c[LocalPlayer]->hitBox, *c[i]->trap.itemModel.collisionBox)) {
				moveCoef = 0.35;
			}
			c[i]->trap.itemModel.collisionBox->x -= xShift * moveCoef;
			c[i]->trap.itemModel.collisionBox->y -= yShift * moveCoef;
			c[i]->trap.itemModel.posRect->x -= xShift * moveCoef;
			c[i]->trap.itemModel.posRect->y -= yShift * moveCoef;
		}
	}
	//if (CheckAllCollisions(c, ))
	moveCharacter(c[LocalPlayer], xShift, yShift, xPosShift, yPosShift,moveCoef);
	SaveObjPosition(objs, objCount, yShift*moveCoef, xShift*moveCoef);
	if (CheckAllCollisions(c[LocalPlayer], objs, objCount, CollisionModel)) {
		if (c[LocalPlayer]->model.posRect->x != WIDTH_w / 2 || c[LocalPlayer]->model.posRect->y != HEIGHT_w / 2) {
			moveCharacter(c[LocalPlayer], 0, 0, -xPosShift, -yPosShift, moveCoef);
		}
		if ((c[LocalPlayer]->model.posRect->x == WIDTH_w / 2 || c[LocalPlayer]->model.posRect->y == HEIGHT_w / 2)) {
			c[LocalPlayer]->camera->x -= xShift*moveCoef;
			c[LocalPlayer]->camera->y -= yShift*moveCoef;
			SaveObjPosition(objs, objCount, -yShift*moveCoef, -xShift*moveCoef);
		}
	}
}