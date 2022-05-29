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
 	if (trap->isActive && Timer_GetTicks(&trap->delay) <= 10000) {
		trap->itemModel.collisionBox = trap->itemModel.posRect;
	}
	else if (keyFlag && Timer_GetTicks(&trap->delay) >= 1000) {
 		trap->isActive = true;
		Timer_Start(&trap->delay);
		trap->itemModel.posRect->x = posRect.x;
		trap->itemModel.posRect->y = posRect.y;
	}
	else {
		trap->isActive = false;
		trap->itemModel.collisionBox = {0};
		trap->itemModel.posRect->x = posRect.x;
		trap->itemModel.posRect->y = posRect.y;
	}
}

void ReleaseSpikes(character* players[], int pCount, int velCoef) {
	for (int i = 0; i < pCount; ++i) {
		players[i]->trap.ItemFunc(&players[i]->trap, false, *players[i]->model.posRect);
		if (players[i]->trap.itemModel.collisionBox != NULL) {
			if (isCollided(*players[LocalPlayer]->model.collisionBox, *players[i]->trap.itemModel.collisionBox)) {
				players[LocalPlayer]->VelCoef *= 0.35;
				break;
			}
			else {
				players[LocalPlayer]->VelCoef = velCoef;
			}
		}
		else players[LocalPlayer]->VelCoef = velCoef;
	}
}

bool characterInit(character* c, SDL_Texture* t, SDL_Rect pos, SDL_Rect cBox, SDL_Rect hitBox, SDL_Rect camera) 
{
	c->hitBox = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	c->camera = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (!initGameObject(&c->model, t, pos, {}, cBox)) return false;
	*c->hitBox = hitBox;
	*c->camera = camera;
	c->VelCoef = 1;
	c->sword = { 0 };
	c->trap = { 0 };
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
		if (objs[i]->collisionBox != NULL && flag == CollisionModel && isCollided((*c->model.collisionBox), *objs[i]->collisionBox)) return true;
		else if (flag == weaponRange && isCollided(*c->hitBox, *objs[i]->collisionBox)) return true;
	}
	return false;
}

void SaveObjPosition(gameObj* objs[], int objCount, int yShift, int xShift) 
{
	for (int i = 0; i < objCount; i++) {
		if (objs[i]->posRect != NULL) {
			objs[i]->posRect->y -= yShift;
			objs[i]->posRect->x -= xShift;
		}
		if (objs[i]->collisionBox != NULL) {
			objs[i]->collisionBox->y -= yShift;
			objs[i]->collisionBox->x -= xShift;
		}
	}
}

void SavePlayersPosition(character** p, int pCount, int yShift, int xShift) {
	for (int i = 0; i < pCount; ++i) {
		if (i != LocalPlayer) {
			p[i]->model.collisionBox->x -= xShift;
			p[i]->hitBox->x -= xShift;
			p[i]->model.posRect->x -= xShift;
			p[i]->model.collisionBox->y -= yShift;
			p[i]->hitBox->y -= yShift;
			p[i]->model.posRect->y -= yShift;
		}
		if (p[i]->trap.itemModel.posRect != NULL) {
			p[i]->trap.itemModel.posRect->x -= xShift;
			p[i]->trap.itemModel.posRect->y -= yShift;
		}
		if (p[i]->sword.itemModel.collisionBox != NULL) {
			p[i]->sword.itemModel.collisionBox->x -= xShift;
			p[i]->sword.itemModel.collisionBox->y -= yShift;
		}
		if (p[i]->sword.itemModel.posRect != NULL) {
			p[i]->sword.itemModel.posRect->x -= xShift;
			p[i]->sword.itemModel.posRect->y -= yShift;
		}
	}
}

void moveCharacter(character* c, int xShift, int yShift, int xPosShift, int yPosShift) {
	c->model.posRect->x += xPosShift; c->model.posRect->y += yPosShift;
	c->hitBox->x += xPosShift; c->hitBox->y += yPosShift;
	c->model.collisionBox->x += xPosShift; c->model.collisionBox->y += yPosShift;
	c->camera->x += xShift; c->camera->y += yShift;
}

void HandleMovement(character* c[], const Uint8* move, size_t lastEvent[2], gameObj* objs[], int objCount, int playersCount, double velCoef)
{
	if (c[LocalPlayer]->stamina < 100) {
		c[LocalPlayer]->stamina += 0.5;
		if (c[LocalPlayer]->stamina >= 100) c[LocalPlayer]->canRun = true;
	}
	c[LocalPlayer]->VelCoef = velCoef;
	ReleaseSpikes(c, playersCount, velCoef);
	int yShift = 0; int xShift = 0;
	int xPosShift = 0; int yPosShift = 0;
	const Uint8* currentKeyStates = move;
	if (currentKeyStates[SDL_SCANCODE_W]) {
		if (c[LocalPlayer]->camera->y > 0 && (abs(c[LocalPlayer]->model.posRect->y - HEIGHT_w / 2) <= 10)) yShift -= VELOCITY;
		else if (c[LocalPlayer]->model.posRect->y > 0) yPosShift -= VELOCITY;
		lastEvent[0] = KEY_PRESS_SURFACE_UP; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_S]) {
		if (c[LocalPlayer]->camera->y < BG_HEIGHT && (abs(c[LocalPlayer]->model.posRect->y - HEIGHT_w / 2 )<= 10)) yShift += VELOCITY;
		else if (c[LocalPlayer]->model.posRect->y < HEIGHT_w - c[LocalPlayer]->model.posRect->h) yPosShift += VELOCITY;
		lastEvent[0] = KEY_PRESS_SURFACE_DOWN; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_A]) {
		if (c[LocalPlayer]->camera->x > 0 && abs(c[LocalPlayer]->model.posRect->x - WIDTH_w / 2) <= 10) xShift -= VELOCITY;
		else if (c[LocalPlayer]->model.posRect->x > 0) xPosShift -= VELOCITY;
		lastEvent[0] = KEY_PRESS_SURFACE_LEFT; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_D]) {
		if (c[LocalPlayer]->camera->x < BG_WIDTH && abs(c[LocalPlayer]->model.posRect->x - WIDTH_w / 2) <= 10) xShift += VELOCITY;
		else if (c[LocalPlayer]->model.posRect->x < WIDTH_w - c[LocalPlayer]->model.posRect->w) xPosShift += VELOCITY;
		lastEvent[0] = KEY_PRESS_SURFACE_RIGHT; lastEvent[1]++;
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
	if (CheckAllCollisions(c[LocalPlayer], objs, objCount, CollisionModel)) {
		if (c[LocalPlayer]->model.posRect->x != WIDTH_w / 2 || c[LocalPlayer]->model.posRect->y != HEIGHT_w / 2) {
			moveCharacter(c[LocalPlayer], 0, 0, -xPosShift, -yPosShift);
		}
		if ((c[LocalPlayer]->model.posRect->x == WIDTH_w / 2 || c[LocalPlayer]->model.posRect->y == HEIGHT_w / 2)) {
			c[LocalPlayer]->camera->x -= xShift;
			c[LocalPlayer]->camera->y -= yShift;
			SavePlayersPosition(c, playersCount, -yShift, -xShift);
			SaveObjPosition(objs, objCount, -yShift, -xShift);
		}
	}
}