#ifndef anEnv
#define anEnv

#include "gameObjects.h"

//Initializing game screen
bool IinitScreen(SDL_Window** gWindow, SDL_Renderer** gRenderer, Mix_Music** gMusic, const int winW, const int winH);
//loads a texture
SDL_Texture* loadTexture(const char* path, SDL_Renderer** gRenderer);

void DestroyAll(SDL_Window** gWindow, SDL_Renderer** gRenderer, Mix_Music** gMusic);

void MinotaurLoop();
#endif 