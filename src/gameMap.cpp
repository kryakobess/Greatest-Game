#include "gameMap.h"

void DrawLabirint(SDL_Renderer* render, SDL_Rect& camera, const Matrix* matrix)
{
	for (int i = 0; i < matrix->countCol; i++)
	{
		for (int j = 0; j < matrix->countRow; j++)
		{
			if (isCollided(camera, matrix->tileArray[i][j].tileBox))
			{
				SDL_RenderCopy(render, gTileTexture,
					&matrix->gTileClips[matrix->tileArray[i][j].tileType],
					&matrix->tileArray[i][j].tileBox);
			}
		}
	}
}

void InitLabirintMatrix(Matrix* matrix)
{
	matrix->tileArray = (Tile**)calloc(matrix->countCol*matrix->countRow, sizeof(Tile));
	matrix->gTileClips[BRICK].x = 0;
	matrix->gTileClips[BRICK].y = 0;
	matrix->gTileClips[BRICK].w = 0;
	matrix->gTileClips[BRICK].h = 0;

	matrix->gTileClips[BROKEN_BRICK].x = 0;
	matrix->gTileClips[BROKEN_BRICK].y = 0;
	matrix->gTileClips[BROKEN_BRICK].w = 0;
	matrix->gTileClips[BROKEN_BRICK].h = 0;
	for (int i = 0; i < matrix->countCol; i++)
	{
		for (int j = 0; j < matrix->countRow; j++)
		{
			matrix->tileArray[i][j].tileBox.x = i * WIDTH_TILE;
			matrix->tileArray[i][j].tileBox.y = j * HEIGHT_TILE;
			matrix->tileArray[i][j].tileBox.w = WIDTH_TILE;
			matrix->tileArray[i][j].tileBox.h = HEIGHT_TILE;
		}
	}
}