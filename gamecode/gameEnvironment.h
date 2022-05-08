#ifndef anEnv
#define anEnv

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH_w 1280
#define HEIGHT_w 720
#define BG_WIDTH 3800
#define BG_HEIGHT 2170

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
Mix_Music* gMusic = NULL;
Mix_Chunk* gMove = NULL;

//Initializing game screen
bool IinitScreen();
//loads a texture
SDL_Texture* loadTexture(const char* path);

void DestroyAll();

#endif 