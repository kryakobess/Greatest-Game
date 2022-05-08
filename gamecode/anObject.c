#include "anObject.h"

bool initGameObject(gameObj* obj, SDL_Texture* lTexture, int* posCfg, int* srcCfg, int* hitbCfg) {
	obj->texture = lTexture;
	if (obj->texture == NULL) {
		return false;
		printf("Texture loading error!");
	}
	obj->posRect.x = posCfg[0]; obj->posRect.y = posCfg[1]; obj->posRect.w = posCfg[2]; obj->posRect.h = posCfg[3];
	obj->srcRect.x = srcCfg[0]; obj->srcRect.y = srcCfg[1]; obj->srcRect.w = srcCfg[2]; obj->srcRect.h = srcCfg[3];
	obj->hitBox.x = hitbCfg[0]; obj->hitBox.y = hitbCfg[1]; obj->hitBox.w = hitbCfg[2]; obj->hitBox.h = hitbCfg[3];
	return true;
}

void FreeObj(gameObj* obj) {
	SDL_DestroyTexture(obj->texture);
	obj->texture = NULL;
}

void RenderObject(gameObj* obj, SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, obj->texture, &(obj->srcRect), &(obj->posRect));
}