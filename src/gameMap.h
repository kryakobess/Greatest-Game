#ifndef GAME_MAP
#define GAME_MAP

#include <SDL.h>
#include "GameEnv.h"
#include <time.h>
#include "Colliders.h"

#define WIDTH_TILE 128
#define HEIGHT_TILE 128

typedef enum tileType
{
	CLEAR_STONE,
	CRAVED_STONE,
	DIRTY_STONE,
	POLISHED_STONE,

	DARK_CLEAR_STONE,
	DARK_CRAVED_STONE,
	DARK_DIRTY_STONE,
	DARK_POLISHED_STONE,

	GROUND,
	BRICKED_GROUND,

	DARK_GROUND,
	DARK_BRICKED_GROUND,

	MAX_COUNT_TILETYPE
}TileType;

typedef struct tile
{
	SDL_Rect tileBox;
	TileType tileType;
	Collider* collider;
}Tile;

typedef struct Matrix
{
	SDL_Texture* gTileTexture;
	size_t countRow;
	size_t countCol;
	Tile** tileArray;
	SDL_Rect gTileClips[MAX_COUNT_TILETYPE] = {
		{16, 0, 16, 16}, {64, 0, 16, 16}, {32, 0, 16, 16}, {48, 0, 16, 16},
		{16, 16, 16, 16}, {64, 16, 16, 16}, {32, 16, 16, 16}, {48, 16, 16, 16},
		{64, 32, 16, 16}, {0, 32, 16, 16},
		{64, 48, 16, 16}, {0, 48, 16, 16} };
}Matrix;

void DrawLabirint(SDL_Renderer* render, SDL_Rect* camera, Matrix* matrix);
bool InitCreateLabirint(Matrix* matrix, CollidersArray* colArr);
bool AddLabirintColliders(Matrix* matrix, CollidersArray* colArr, size_t** matr);
bool InitLabirintMatrix(Matrix* matrix);

#endif // !GAME_MAP