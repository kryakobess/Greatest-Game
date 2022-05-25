#include "gameObjects.h"

bool initGameObject(gameObj* obj, SDL_Texture* lTexture, SDL_Rect posCfg, SDL_Rect srcCfg, SDL_Rect hitbCfg) {
	obj->texture = lTexture;
	if (obj->texture == NULL) {
		return false;
		printf("Texture loading error!");
	}
	obj->posRect = posCfg;
	obj->srcRect = srcCfg;
	obj->hitBox = hitbCfg;
	return true;
}

void FreeObj(gameObj* obj) {
	SDL_DestroyTexture(obj->texture);
	obj->texture = NULL;
}

void RenderObject(gameObj* obj, SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, obj->texture, &(obj->srcRect), &(obj->posRect));
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

bool characterInit(character* c, SDL_Texture* t, SDL_Rect pos, SDL_Rect hitBox, SDL_Rect camera) {
	c->camera = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	c->posRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	c->hitBox = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	c->camera = (SDL_Rect*)malloc(sizeof(SDL_Rect));

	
	c->texture = t;
	c->posRect = &pos;
	c->hitBox = &hitBox;
	c->camera = &camera;
	return true;
}

void SaveObjPosition(gameObj* freezedObjs[], int objCount, int yShift, int xShift) {
	for (int i = 0; i < objCount; i++) {
		freezedObjs[i]->posRect.y += yShift;
		freezedObjs[i]->posRect.x += xShift;
		freezedObjs[i]->hitBox.y += yShift;
		freezedObjs[i]->hitBox.x += xShift;
	}
}

void HandleMovement(character* c, const Uint8* move, int HEIGHT_w, int WIDTH_w, int BG_HEIGHT,  int BG_WIDTH, int* yShift, int* xShift, size_t lastEvent[2]) {
	int dSp = 0; int rSp = 0; int lSp = 0; int uSp = 0;
	const Uint8* currentKeyStates = move;
	if (currentKeyStates[SDL_SCANCODE_W]) {
		if (c->camera->y > 0 && (c->posRect->y == HEIGHT_w / 2)) {
			c->camera->y -= VELOCITY;
			*yShift += VELOCITY;
		}
		else if (c->posRect->y > 0) {
			c->posRect->y -= VELOCITY;
			c->hitBox->y -= VELOCITY;
		}
		dSp = 0; rSp = 0; lSp = 0;
		lastEvent[0] = KEY_PRESS_SURFACE_UP; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_S]) {
		if (c->camera->y < BG_HEIGHT && (c->posRect->y == HEIGHT_w / 2)) {
			c->camera->y += VELOCITY;
			*yShift -= VELOCITY;
		}
		else if (c->posRect->y < HEIGHT_w - c->posRect->h) {
			c->posRect->y += VELOCITY;
			c->hitBox->y += VELOCITY;
		}
		uSp = 0; rSp = 0; lSp = 0;
		lastEvent[0] = KEY_PRESS_SURFACE_DOWN; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_A]) {
		if (c->camera->x > 0 && c->posRect->x == WIDTH_w / 2) {
			c->camera->x -= VELOCITY;
			*xShift += VELOCITY;
		}
		else if (c->posRect->x > 0) {
			c->posRect->x -= VELOCITY;
			c->hitBox->x -= VELOCITY;
		}
		uSp = 0; rSp = 0; dSp = 0;
		lastEvent[0] = KEY_PRESS_SURFACE_LEFT; lastEvent[1]++;
	}
	if (currentKeyStates[SDL_SCANCODE_D]) {
		if (c->camera->x < BG_WIDTH && c->posRect->x == WIDTH_w / 2) {
			c->camera->x += VELOCITY;
			*xShift -= VELOCITY;
		}
		else if (c->posRect->x < WIDTH_w - c->posRect->w) {
			c->posRect->x += VELOCITY;
			c->hitBox->x += VELOCITY;
		}
		uSp = 0; dSp = 0; lSp = 0;
		lastEvent[0] = KEY_PRESS_SURFACE_RIGHT; lastEvent[1]++;
	}
	//SDL_RenderCopy(gRenderer, gSpriteTexture, &gSpriteClips[lastEvent[0]][((lastEvent[1]) / 8) % 4], &c->posRect);
}


//void Move(character* c, float velocity, gameObj* obj[], int maxHeight, int maxWidth)
//{
//}
//
//void MoveCamera(character* camera, float velocity, int maxBgHeight, int maxBgWidth) {
//	c->camera.x += c->curVelX;
//	if (c->camera.y < 0 || (c->posRect.y != maxBgHeight / 2 || c->camera.y >= BG_HEIGHT) {
//		c->camera.y
//	}
//}
