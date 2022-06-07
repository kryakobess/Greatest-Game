#ifndef anEnv
#define anEnv

#define WIDTH_w 1280
#define HEIGHT_w 720
#define LocalPlayer 0

#include <SDL.h>
#include <math.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Circle {
	int x, y;
	int r;
}Circle;

typedef struct Timer {
	Uint32 startTicks;
	Uint32 pausedTicks;
	bool paused;
	bool started;
}Timer;

void Timer_Init(Timer* t);
void Timer_Start(Timer* t);
void Timer_Stop(Timer* t);
void Timer_Pause(Timer* t);
void Timer_Unpause(Timer* t);
Uint32 Timer_GetTicks(Timer* t);

bool isCollided(SDL_Rect a, SDL_Rect b);
bool isCollided(Circle c, SDL_Rect r);

//Initializing game screen
bool IinitScreen(SDL_Window** gWindow, SDL_Renderer** gRenderer, Mix_Music** gMusic, const int winW, const int winH);
//loads a texture
SDL_Texture* loadTexture(const char* path, SDL_Renderer** gRenderer);

void DestroyAll(SDL_Window** gWindow, SDL_Renderer** gRenderer, Mix_Music** gMusic);

void MinotaurLoop();
#endif 