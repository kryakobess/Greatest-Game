#include <SDL.h>
#include "GameEnv.h"

#define WIDTH_TILE 100
#define HEIGHT_TILE 100

typedef enum tileType
{
	CLEAR_STONE,
	CRAVED_STONE,
	DIRTY_STONE,

	DARK_CLEAR_STONE,
	DARK_CRAVED_STONE,
	DARK_DIRTY_STONE,

	GROUND,

	MAX_COUNT_TILETYPE
}TileType;

typedef struct tile
{
	SDL_Rect tileBox;
	TileType tileType;
}Tile;

typedef struct Matrix
{
	SDL_Texture* gTileTexture;
	size_t countRow;
	size_t countCol;
	Tile** tileArray;
	SDL_Rect gTileClips[MAX_COUNT_TILETYPE] = { {16, 0, 16, 16},
		{64, 0, 16, 16}, {32, 0, 16, 16},
		{16, 16, 16, 16}, {64, 16, 16, 16}, {32, 16, 16, 16},
		{64, 32, 16, 16} };
}Matrix;

void DrawLabirint(SDL_Renderer* render, SDL_Rect* camera, Matrix* matrix);
bool InitCreateLabirint(Matrix* matrix);
bool InitLabirintMatrix(Matrix* matrix);