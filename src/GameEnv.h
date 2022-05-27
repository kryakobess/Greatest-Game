#ifndef anEnv
#define anEnv

#define WIDTH_w 1280
#define HEIGHT_w 720
#define BG_WIDTH 3800
#define BG_HEIGHT 2170


#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Initializing game screen
bool IinitScreen(SDL_Window** gWindow, SDL_Renderer** gRenderer, Mix_Music** gMusic, const int winW, const int winH);
//loads a texture
SDL_Texture* loadTexture(const char* path, SDL_Renderer** gRenderer);

void DestroyAll(SDL_Window** gWindow, SDL_Renderer** gRenderer, Mix_Music** gMusic);

void MinotaurLoop();
#endif 