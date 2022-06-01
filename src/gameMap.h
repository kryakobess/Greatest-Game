#include <SDL.h>
#include "GameEnv.h"

#define WIDTH_TILE 80
#define HEIGHT_TILE 80

SDL_Texture* gTileTexture;

typedef enum tileType
{
	BRICK,
	BROKEN_BRICK,
	MAX_COUNT_TILETYPE
}TileType;

typedef struct tile
{
	SDL_Rect tileBox;
	TileType tileType;
}Tile;

typedef struct Matrix
{
	SDL_Rect gTileClips[MAX_COUNT_TILETYPE];
	size_t countRow;
	size_t countCol;
	Tile** tileArray;
}Matrix;

void DrawLabirint(SDL_Rect& camera, const Matrix* matrix);
void InitLabirintMatrix(Matrix* matrix);