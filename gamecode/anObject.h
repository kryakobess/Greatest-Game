#ifndef anObject
#define anObject

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SPRITE_NUMBER 4
#define VELOCITY 3

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

typedef struct gameObj {
	SDL_Rect posRect;
	SDL_Rect srcRect;
	SDL_Rect hitBox;
	SDL_Texture* texture;
}gameObj;

typedef struct character {
	SDL_Rect posRect;
	SDL_Rect spriteClips[KEY_PRESS_SURFACE_TOTAL][SPRITE_NUMBER];
	SDL_Rect hitBox;
	SDL_Texture* texture;
}character;

bool initGameObject(gameObj* obj, SDL_Texture* lTexture, int* posCfg, int* srcCfg, int* hitbCfg);
void FreeObj(gameObj* obj);
void RenderObject(gameObj* obj, SDL_Renderer* renderer);



#endif // !anObject
