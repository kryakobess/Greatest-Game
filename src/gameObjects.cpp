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

void FreeObj(gameObj* obj) {
	SDL_DestroyTexture(obj->texture);
	obj->texture = NULL;
}

void RenderObject(gameObj* obj, SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, obj->texture, &(obj->srcRect), (obj->posRect));
}

void Timer_Init(Timer* t) {
	t->startTicks = 0;
	t->pausedTicks = 0;
	t->paused = false;
	t->started = false;
}

void Timer_Start(Timer* t) {
	t->started = true;
	t->paused = false;
	t->startTicks = SDL_GetTicks();
	t->pausedTicks = 0;
}

void Timer_Stop(Timer* t) {
	t->started = false;
	t->paused = false;
	t->startTicks = 0;
	t->pausedTicks = 0;
}

void Timer_Pause(Timer* t) {
	if (t->started && !t->paused)
	{
		t->paused = true;
		t->pausedTicks = SDL_GetTicks() - t->startTicks;
		t->startTicks = 0;
	}
}

void Timer_Unpause(Timer* t) {
	if (t->started && t->paused)
	{
		t->paused = false;
		t->startTicks = SDL_GetTicks() - t->pausedTicks;
		t->pausedTicks = 0;
	}
}

Uint32 Timer_GetTicks(Timer* t) {
	Uint32 time = 0;
	if (t->started)
	{
		if (t->paused)
		{
			time = t->pausedTicks;
		}
		else
		{
			time = SDL_GetTicks() - t->startTicks;
		}
	}
	return time;
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

bool characterInit(character* c, SDL_Texture* t, SDL_Rect pos, SDL_Rect cBox, SDL_Rect hitBox, SDL_Rect camera) {
	c->hitBox = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	c->camera = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	initGameObject(&c->model, t, pos, {}, cBox);
	*c->hitBox = hitBox;
	*c->camera = camera;
	return true;
}

bool CheckAllCollisions(character* c, gameObj* objs[], int objCount, int flag) {
	for (int i = 0; i < objCount; ++i) {
		if (flag == CollisionModel && isCollided((*c->model.collisionBox), *objs[i]->collisionBox)) return true;
		else if (flag == weaponRange && isCollided(*c->hitBox, *objs[i]->collisionBox)) return true;
	}
	return false;
}

void SaveObjPosition(gameObj* objs[], int objCount, int yShift, int xShift) {
	for (int i = 0; i < objCount; i++) {
		objs[i]->posRect->y -= yShift;
		objs[i]->posRect->x -= xShift;
		objs[i]->collisionBox->y -= yShift;
		objs[i]->collisionBox->x -= xShift;
	}
}

void HandleMovement(character* c, const Uint8* move, size_t lastEvent[2], gameObj* objs[], int objCount) {
	int yShift = 0; int xShift = 0;
	int xPosShift = 0; int yPosShift = 0;
	const Uint8* currentKeyStates = move;
	if (currentKeyStates[SDL_SCANCODE_W]) {
		if (c->camera->y > 0 && (c->model.posRect->y == HEIGHT_w / 2)) {
			yShift -= VELOCITY;
		}
		else if (c->model.posRect->y > 0) {
			yPosShift -= VELOCITY;
		}
		lastEvent[0] = KEY_PRESS_SURFACE_UP; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_S]) {
		if (c->camera->y < BG_HEIGHT && (c->model.posRect->y == HEIGHT_w / 2)) {
			yShift += VELOCITY;
		}
		else if (c->model.posRect->y < HEIGHT_w - c->model.posRect->h) {
			yPosShift += VELOCITY;
		}
		lastEvent[0] = KEY_PRESS_SURFACE_DOWN; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_A]) {
		if (c->camera->x > 0 && c->model.posRect->x == WIDTH_w / 2) {
			xShift -= VELOCITY;
		}
		else if (c->model.posRect->x > 0) {
			xPosShift -= VELOCITY;
		}
		lastEvent[0] = KEY_PRESS_SURFACE_LEFT; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_D]) {
		if (c->camera->x < BG_WIDTH && c->model.posRect->x == WIDTH_w / 2) {
			xShift += VELOCITY;
		}
		else if (c->model.posRect->x < WIDTH_w - c->model.posRect->w) {
			xPosShift += VELOCITY;
		}
		lastEvent[0] = KEY_PRESS_SURFACE_RIGHT; lastEvent[1]++;
	}
	c->model.posRect->x += xPosShift; c->model.posRect->y += yPosShift;
	c->hitBox->x += xPosShift; c->hitBox->y += yPosShift;
	c->model.collisionBox->x += xPosShift; c->model.collisionBox->y += yPosShift;
	c->camera->x += xShift; c->camera->y += yShift;
	SaveObjPosition(objs, objCount, yShift, xShift);
	if (CheckAllCollisions(c, objs, objCount, CollisionModel)) {
		if (c->model.posRect->x != WIDTH_w / 2 || c->model.posRect->y != HEIGHT_w / 2) {
			c->model.posRect->x -= xPosShift; c->model.posRect->y -= yPosShift;
			c->hitBox->x -= xPosShift; c->hitBox->y -= yPosShift;
			c->model.collisionBox->x -= xPosShift; c->model.collisionBox->y -= yPosShift;
		}
		if ((c->model.posRect->x == WIDTH_w / 2 || c->model.posRect->y == HEIGHT_w / 2)) {
			c->camera->x -= xShift;
			c->camera->y -= yShift;
			SaveObjPosition(objs, objCount, -yShift, -xShift);
		}
	}
}

