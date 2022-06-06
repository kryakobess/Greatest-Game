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

bool initGameItem(gameItem* i, SDL_Texture* t, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect cBox, void(*func)(gameItem*, int, SDL_Rect*))
{
	if (!initGameObject(&i->itemModel, t, posCfg, srcCfg, cBox)) return false;
	//i->collisionCircle = (Circle*)malloc(sizeof(Circle));
	i->ItemFunc = func;
	Timer_Init(&i->delay);
	Timer_Start(&i->delay);
	i->isActive = false;
	return true;
}

void ItemFree(gameItem* i) {
	ObjFree(&i->itemModel);
	Timer_Stop(&i->delay);
	free(i->collisionCircle);
}

void ActivateTrap(gameItem* trap, int keyFlag, SDL_Rect* posRect) {
 	if (trap->isActive && Timer_GetTicks(&trap->delay) <= 10000) {
		trap->itemModel.collisionBox = trap->itemModel.posRect;
	}
	else if (keyFlag && Timer_GetTicks(&trap->delay) >= 20000) {
 		trap->isActive = true;
		Timer_Start(&trap->delay);
		trap->itemModel.posRect->x = posRect->x;
		trap->itemModel.posRect->y = posRect->y;
	}
	else {
		trap->isActive = false;
		trap->itemModel.collisionBox = NULL;
		trap->itemModel.posRect->x = posRect->x;
		trap->itemModel.posRect->y = posRect->y;
	}
}

void ReleaseSpikes(character* players[], int pCount, int velCoef) {
	for (int i = 0; i < pCount; ++i) {
		if (players[i]->trap.isActive) {
			players[i]->trap.ItemFunc(&players[i]->trap, false, players[i]->model.posRect);
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
}

void ActivateSword(gameItem* sword, int spriteNumber, SDL_Rect* posRect) {
	sword->isActive = true;
	sword->collisionCircle = (Circle*)realloc(sword->collisionCircle, sizeof(Circle));
	int y0 = posRect->y; int x0 = posRect->x;
	int w = posRect->w; int h = posRect->h;
	sword->collisionCircle->r = w / 2;
	switch (spriteNumber) {
	case KEY_PRESS_SURFACE_DOWN:
		/*sword->collisionCircle->y = y0 + h;
		sword->collisionCircle->x = x0 + w / 2;*/
		sword->itemModel.posRect->y = y0 + h*2/3;
		sword->itemModel.posRect->x = x0-w/2;
		sword->itemModel.posRect->h = w;
		sword->itemModel.posRect->w = 2*w;
		break;
	case KEY_PRESS_SURFACE_UP:
		/*sword->collisionCircle->y = y0;
		sword->collisionCircle->x = x0 + w / 2;*/
		sword->itemModel.posRect->y = y0 ;
		sword->itemModel.posRect->x = x0 - w / 2;
		sword->itemModel.posRect->h = w;
		sword->itemModel.posRect->w = 2*w;
		break;
	case KEY_PRESS_SURFACE_LEFT:
		/*sword->collisionCircle->y = y0 + h / 2;
		sword->collisionCircle->x = x0;*/
		sword->itemModel.posRect->y = y0+w/2;
		sword->itemModel.posRect->x = x0 - 70;
		sword->itemModel.posRect->h = w;
		sword->itemModel.posRect->w = 2 * w;
		break;
	case KEY_PRESS_SURFACE_RIGHT:
		/*sword->collisionCircle->y = y0 + h / 2;
		sword->collisionCircle->x = x0 + w;*/
		sword->itemModel.posRect->y = y0 + w / 2;
		sword->itemModel.posRect->x = x0 + 10;
		sword->itemModel.posRect->h = w;
		sword->itemModel.posRect->w = 2 * w;
		break;
	}
	printf("%d\n", sword->collisionCircle->r);
	Timer_Start(&sword->delay);
}

void AttackSword(gameItem* sword, SDL_Renderer* gRenderer, int delay, int spriteDirect) {

	if (sword->isActive && sword->collisionCircle != NULL) {
		double degree = 0;
		SDL_RendererFlip flip = SDL_FLIP_NONE;
		if (spriteDirect == KEY_PRESS_SURFACE_DOWN) flip = SDL_FLIP_VERTICAL;
		if (spriteDirect == KEY_PRESS_SURFACE_UP) degree = 0;
		if (spriteDirect == KEY_PRESS_SURFACE_LEFT) degree = -90;
		if (spriteDirect == KEY_PRESS_SURFACE_RIGHT) { degree = 90; flip = SDL_FLIP_HORIZONTAL; }
		SDL_Rect srcRect;
		if (Timer_GetTicks(&sword->delay) < 150) {
			srcRect = { 0,0,30,30 };
		}
		else if (Timer_GetTicks(&sword->delay) >= 150) {
			srcRect = { 30, 0, 30,30 };
		}
		SDL_RenderCopyEx(gRenderer, sword->itemModel.texture, &srcRect, sword->itemModel.posRect, degree, NULL, flip);
		if (Timer_GetTicks(&sword->delay) >= 300) {
			sword->itemModel.collisionBox = NULL;
			sword->isActive = false;
			Timer_Start(&sword->delay);
		}
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
	c->spriteNumber[0] = KEY_PRESS_SURFACE_DOWN; c->spriteNumber[1] = 0;
	c->stamina = 100;
	c->HP = 100;
	c->canRun = true;
	return true;
}

void ObjFree(gameObj* obj)
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
			if (p[i]->sword.itemModel.collisionBox != NULL) {
				p[i]->sword.itemModel.collisionBox->x -= xShift;
				p[i]->sword.itemModel.collisionBox->y -= yShift;
			}
			if (p[i]->sword.itemModel.posRect != NULL) {
				p[i]->sword.itemModel.posRect->x -= xShift;
				p[i]->sword.itemModel.posRect->y -= yShift;
			}
		}
		if (p[i]->trap.itemModel.posRect != NULL) {
			p[i]->trap.itemModel.posRect->x -= xShift;
			p[i]->trap.itemModel.posRect->y -= yShift;
		}
	}
}

void moveCharacter(character* c, int xShift, int yShift, int xPosShift, int yPosShift) {
	c->model.posRect->x += xPosShift; c->model.posRect->y += yPosShift;
	c->hitBox->x += xPosShift; c->hitBox->y += yPosShift;
	c->model.collisionBox->x += xPosShift; c->model.collisionBox->y += yPosShift;
	c->camera->x += xShift; c->camera->y += yShift;
}

void HandleMovement(character* c[], const Uint8* move, gameObj* objs[], int objCount, int playersCount, double velCoef)
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
		if (velCoef != 0) {
			c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_UP; c[LocalPlayer]->spriteNumber[1]++;
		}
	}
	if (currentKeyStates[SDL_SCANCODE_S]) {
		if (c[LocalPlayer]->camera->y < BG_HEIGHT && (abs(c[LocalPlayer]->model.posRect->y - HEIGHT_w / 2 )<= 10)) yShift += VELOCITY;
		else if (c[LocalPlayer]->model.posRect->y < HEIGHT_w - c[LocalPlayer]->model.posRect->h) yPosShift += VELOCITY;
		if (velCoef != 0) {
			c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_DOWN; c[LocalPlayer]->spriteNumber[1]++;
		}
	}
	if (currentKeyStates[SDL_SCANCODE_A]) {
		if (c[LocalPlayer]->camera->x > 0 && abs(c[LocalPlayer]->model.posRect->x - WIDTH_w / 2) <= 10) xShift -= VELOCITY;
		else if (c[LocalPlayer]->model.posRect->x > 0) xPosShift -= VELOCITY;
		if (velCoef != 0) {
			c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_LEFT; c[LocalPlayer]->spriteNumber[1]++;
		}
	}
	if (currentKeyStates[SDL_SCANCODE_D]) {
		if (c[LocalPlayer]->camera->x < BG_WIDTH && abs(c[LocalPlayer]->model.posRect->x - WIDTH_w / 2) <= 10) xShift += VELOCITY;
		else if (c[LocalPlayer]->model.posRect->x < WIDTH_w - c[LocalPlayer]->model.posRect->w) xPosShift += VELOCITY;
		if (velCoef != 0) {
			c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_RIGHT; c[LocalPlayer]->spriteNumber[1]++;
		}
	}
	if (currentKeyStates[SDL_SCANCODE_E]) {
		c[LocalPlayer]->trap.ItemFunc(&c[LocalPlayer]->trap, true, c[LocalPlayer]->model.posRect);
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
		if ((abs(c[LocalPlayer]->model.posRect->x - WIDTH_w / 2) <= 10 || abs(c[LocalPlayer]->model.posRect->y - HEIGHT_w / 2) <= 10)) {
			moveCharacter(c[LocalPlayer], -xShift, -yShift, 0, 0);
			SavePlayersPosition(c, playersCount, -yShift, -xShift);
			SaveObjPosition(objs, objCount, -yShift, -xShift);
		}
	}
}