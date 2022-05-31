#include "GameEnv.h"

#define TILE_WIDTH 80
#define TILE_HEIGHT 80
#define TOTAL_TILES 192

const enum TileType {
	SmoothBrick,
	MoldyBrick,
	Dirt, 
	Flowers
};

typedef struct Tile {
	SDL_Rect box;
	char type;
}Tile;

bool TileInit(Tile* tile);
void TileRender(Tile* tile, SDL_Renderer* render);
