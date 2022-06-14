#include "gameObjects.h"

bool initGameObject(gameObj* obj, SDL_Texture* lTexture, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect cBox, CollidersArray* colArr, enum ColliderID ID) {
	obj->texture = lTexture;
	//obj->posRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));

	BoxCollider* ptr;
	if (!AddColliderInArray(colArr, obj->body = CreateCollider(ptr = CreateBoxCollider(cBox), BOX, ID))) return false;
	obj->body->collider = ptr;
	if (obj->texture == NULL) {
		return false;
		printf("Texture loading error!");
	}
	obj->posRect = posCfg;
	obj->srcRect = srcCfg;
	return true;
}

bool initGameItem(gameItem* i, SDL_Texture* t, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect cBox, void(*func)(gameItem*, int, SDL_Rect*), CollidersArray* colArr, enum ColliderID ID)
{
	if (!initGameObject(&i->itemModel, t, posCfg, srcCfg, cBox, colArr, ID)) return false;
	i->ItemFunc = func;
	i->itemModel.body->active = false;
	Timer_Init(&i->delay);
	Timer_Start(&i->delay);
	i->isActive = false;
	return true;
}

void ItemFree(gameItem* i) {
	ObjFree(&i->itemModel);
	Timer_Stop(&i->delay);
}

void ActivateTrap(gameItem* trap, int keyFlag, SDL_Rect* posRect) {
 	if (trap->isActive && Timer_GetTicks(&trap->delay) <= 10000) {
		((BoxCollider*)trap->itemModel.body->collider)->rect = { trap->itemModel.posRect.x, trap->itemModel.posRect.y, trap->itemModel.posRect.w, trap->itemModel.posRect.h };
	}
	else if (keyFlag && Timer_GetTicks(&trap->delay) >= 20000) {
 		trap->isActive = true;
		trap->itemModel.body->active = true;
		Timer_Start(&trap->delay);
		trap->itemModel.posRect.x = posRect->x;
		trap->itemModel.posRect.y = posRect->y;
	}
	else {
		trap->isActive = false;
		trap->itemModel.body->active = false;
		trap->itemModel.posRect.x = posRect->x;
		trap->itemModel.posRect.y = posRect->y;
	}
}

void ReleaseSpikes(character* players[], int pCount, int velCoef, CollidersArray* colArr, SDL_Rect* camera, int LocalPlayer) {
	for (int i = 0; i < pCount; ++i) {
		if (players[i]->trap.isActive) {
			players[i]->trap.ItemFunc(&players[i]->trap, false, &players[i]->model.posRect);
			if (colArr->outCollisionMatrix[PLAYER_COL_ID][TRAP_COL_ID] && players[i]->trap.itemModel.body->active)
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
}

void ActivateSword(gameItem* sword, int spriteNumber, SDL_Rect* posRect) {
	sword->isActive = true;
	int y0 = posRect->y; int x0 = posRect->x;
	int w = posRect->w; int h = posRect->h;
	switch (spriteNumber) {
	case KEY_PRESS_SURFACE_DOWN:
		/*sword->collisionCircle->y = y0 + h;
		sword->collisionCircle->x = x0 + w / 2;*/
		sword->itemModel.posRect.y = y0 + h*2/3;
		sword->itemModel.posRect.x = x0-w/2;
		sword->itemModel.posRect.h = w;
		sword->itemModel.posRect.w = 2*w;
		break;
	case KEY_PRESS_SURFACE_UP:
		/*sword->collisionCircle->y = y0;
		sword->collisionCircle->x = x0 + w / 2;*/
		sword->itemModel.posRect.y = y0 ;
		sword->itemModel.posRect.x = x0 - w / 2;
		sword->itemModel.posRect.h = w;
		sword->itemModel.posRect.w = 2*w;
		break;
	case KEY_PRESS_SURFACE_LEFT:
		/*sword->collisionCircle->y = y0 + h / 2;
		sword->collisionCircle->x = x0;*/
		sword->itemModel.posRect.y = y0+w/2;
		sword->itemModel.posRect.x = x0 - 70;
		sword->itemModel.posRect.h = w;
		sword->itemModel.posRect.w = 2 * w;
		break;
	case KEY_PRESS_SURFACE_RIGHT:
		/*sword->collisionCircle->y = y0 + h / 2;
		sword->collisionCircle->x = x0 + w;*/
		sword->itemModel.posRect.y = y0 + w / 2;
		sword->itemModel.posRect.x = x0 + 10;
		sword->itemModel.posRect.h = w;
		sword->itemModel.posRect.w = 2 * w;
		break;
	}
	Timer_Start(&sword->delay);
}

void AttackSword(gameItem* sword, SDL_Renderer* gRenderer, int delay, int spriteDirect) {

	//if (sword->isActive && sword->collisionCircle != NULL) {
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
		SDL_RenderCopyEx(gRenderer, sword->itemModel.texture, &srcRect, &sword->itemModel.posRect, degree, NULL, flip);
		if (Timer_GetTicks(&sword->delay) >= 300) {
			//sword->itemModel.collisionBox = NULL;
			sword->isActive = false;
			Timer_Start(&sword->delay);
		}
	//}
}

bool characterInit(character* c, SDL_Texture* t, SDL_Rect pos, SDL_Rect cBox, SDL_Rect hitBox, SDL_Rect camera, CollidersArray* colArr)
{
	if ((c->feetCol = (Collider*)calloc(1, sizeof(Collider))) == NULL) return false;

	BoxCollider* ptr;
	if (!AddColliderInArray(colArr, (Collider*)(c->feetCol = CreateCollider(ptr = CreateBoxCollider(cBox), BOX, PLAYER_COL_ID)))) return false;
	c->feetCol->collider = ptr;

	//c->camera = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (!initGameObject(&c->model, t, pos, {}, hitBox, colArr, PLAYER_BODY_COL_ID)) return false;
	c->camera = camera;
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
	//free(obj->posRect);
	//obj->posRect = NULL;
}

void RenderObject(gameObj* obj, SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, obj->texture, &(obj->srcRect), &(obj->posRect));
}

void SaveObjPosition(gameObj* objs[], int objCount, int yShift, int xShift) 
{
	for (int i = 0; i < objCount; i++) {
		//if (objs[i]->posRect != {0, 0, 0, 0}) {
			objs[i]->posRect.y -= yShift;
			objs[i]->posRect.x -= xShift;
		//}
		if (objs[i]->body->active)	{
			((BoxCollider*)(objs[i]->body->collider))->rect.y -= yShift;
			((BoxCollider*)(objs[i]->body->collider))->rect.x -= xShift;
		}
	}
}

void SavePlayersPosition(character** p, int pCount, int yShift, int xShift, int LocalPlayer) {
	for (int i = 0; i < pCount; ++i) {
		if (i != LocalPlayer) {
			((BoxCollider*)(p[i]->model.body->collider))->rect.x -= xShift;
			((BoxCollider*)(p[i]->model.body->collider))->rect.y -= yShift;
			((BoxCollider*)(p[i]->feetCol->collider))->rect.x -= xShift;
			((BoxCollider*)(p[i]->feetCol->collider))->rect.y -= yShift;
			p[i]->model.posRect.x -= xShift;
			p[i]->model.posRect.y -= yShift;
			/*if (p[i]->sword.itemModel.body->active) {
				((BoxCollider*)(p[i]->sword.itemModel.body->collider))->rect.x -= xShift;
				((BoxCollider*)(p[i]->sword.itemModel.body->collider))->rect.y -= yShift;
			}*/
			//if (p[i]->sword.itemModel.posRect != NULL) {
				p[i]->sword.itemModel.posRect.x -= xShift;
				p[i]->sword.itemModel.posRect.y -= yShift;
			//}
		}
		//if (p[i]->trap.itemModel.posRect != NULL) {
			p[i]->trap.itemModel.posRect.x -= xShift;
			p[i]->trap.itemModel.posRect.y -= yShift;
		//}
	}
}

void moveCharacter(character* c, int xShift, int yShift, int xPosShift, int yPosShift) {
	c->model.posRect.x += xPosShift; c->model.posRect.y += yPosShift;

	((BoxCollider*)(c->feetCol->collider))->rect.x += xPosShift;
	((BoxCollider*)(c->feetCol->collider))->rect.y += yPosShift;
	((BoxCollider*)(c->model.body->collider))->rect.x += xPosShift;
	((BoxCollider*)(c->model.body->collider))->rect.y += yPosShift;

	c->camera.x += xShift; c->camera.y += yShift;
}

void HandleMovement(character* c[], const Uint8* move, gameObj* objs[], int objCount, int playersCount, double velCoef, CollidersArray* colArr, Matrix* matrix, int BG_WIDTH, int BG_HEIGHT, int LocalPlayer)
{
	if (c[LocalPlayer]->stamina < 100) {
		c[LocalPlayer]->stamina += 0.5;
		if (c[LocalPlayer]->stamina >= 100) c[LocalPlayer]->canRun = true;
	}
	c[LocalPlayer]->VelCoef = velCoef;
	ReleaseSpikes(c, playersCount, velCoef, colArr, &c[LocalPlayer]->camera, LocalPlayer);

	int yShift = 0; int xShift = 0;
	int xPosShift = 0; int yPosShift = 0;
	const Uint8* currentKeyStates = move;
	if (currentKeyStates[SDL_SCANCODE_W]) {
		if (c[LocalPlayer]->camera.y > 0 && (abs(c[LocalPlayer]->model.posRect.y - HEIGHT_w / 2) <= 10)) yShift -= VELOCITY;
		else if (c[LocalPlayer]->model.posRect.y > 0) yPosShift -= VELOCITY;
		if (velCoef != 0) {
			c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_UP; c[LocalPlayer]->spriteNumber[1]++;
		}
	}
	if (currentKeyStates[SDL_SCANCODE_S]) {
		if (c[LocalPlayer]->camera.y < BG_HEIGHT && (abs(c[LocalPlayer]->model.posRect.y - HEIGHT_w / 2 )<= 10)) yShift += VELOCITY;
		else if (c[LocalPlayer]->model.posRect.y < HEIGHT_w - c[LocalPlayer]->model.posRect.h) yPosShift += VELOCITY;
		if (velCoef != 0) {
			c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_DOWN; c[LocalPlayer]->spriteNumber[1]++;
		}
	}
	if (currentKeyStates[SDL_SCANCODE_A]) {
		if (c[LocalPlayer]->camera.x > 0 && abs(c[LocalPlayer]->model.posRect.x - WIDTH_w / 2) <= 10) xShift -= VELOCITY;
		else if (c[LocalPlayer]->model.posRect.x > 0) xPosShift -= VELOCITY;
		if (velCoef != 0) {
			c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_LEFT; c[LocalPlayer]->spriteNumber[1]++;
		}
	}
	if (currentKeyStates[SDL_SCANCODE_D]) {
		if (c[LocalPlayer]->camera.x < BG_WIDTH && abs(c[LocalPlayer]->model.posRect.x - WIDTH_w / 2) <= 10) xShift += VELOCITY;
		else if (c[LocalPlayer]->model.posRect.x < WIDTH_w - c[LocalPlayer]->model.posRect.w) xPosShift += VELOCITY;
		if (velCoef != 0) {
			c[LocalPlayer]->spriteNumber[0] = KEY_PRESS_SURFACE_RIGHT; c[LocalPlayer]->spriteNumber[1]++;
		}
	}
	if (currentKeyStates[SDL_SCANCODE_E]) {
		c[LocalPlayer]->trap.ItemFunc(&c[LocalPlayer]->trap, true, &c[LocalPlayer]->model.posRect);
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
	SavePlayersPosition(c, playersCount, yShift , xShift, LocalPlayer);
	//printf("Cam pos{%d,%d} size{%d,%d}\n", camera->x, camera->y, camera->w, camera->h);
	//Shift box Colliders
	int i = 0;
	do
	{
		i++;
		if (i >= 3) break;
		GetCollisionStates(colArr, &c[LocalPlayer]->camera);
		for (int i = 0; i < matrix->countCol; i++)
			for (int j = 0; j < matrix->countRow; j++)
			{
				if (matrix->tileArray[i][j].collider != NULL)
				{
					if (isCollided(c[LocalPlayer]->camera, matrix->tileArray[i][j].tileBox))
					{
						/*if (matrix->tileArray[i][j].collider->active)
						{*/
						((BoxCollider*)matrix->tileArray[i][j].collider->collider)->rect = { matrix->tileArray[i][j].tileBox.x - c[LocalPlayer]->camera.x,
						matrix->tileArray[i][j].tileBox.y - c[LocalPlayer]->camera.y,
						matrix->tileArray[i][j].tileBox.w,
						matrix->tileArray[i][j].tileBox.h };
						//}
						matrix->tileArray[i][j].collider->active = true;
					}
					else
						matrix->tileArray[i][j].collider->active = false;
				}
			}
		if (colArr->outCollisionMatrix[PLAYER_COL_ID][ROCK_COL_ID] || colArr->outCollisionMatrix[PLAYER_COL_ID][WALL_COL_ID])
		{
			moveCharacter(c[LocalPlayer], -xShift, -yShift, -xPosShift, -yPosShift);
			SavePlayersPosition(c, playersCount, -yShift, -xShift, LocalPlayer);
			SaveObjPosition(objs, objCount, -yShift, -xShift);
		}
	} while (colArr->outCollisionMatrix[PLAYER_COL_ID][ROCK_COL_ID] || colArr->outCollisionMatrix[PLAYER_COL_ID][WALL_COL_ID]);
}