#include "gameMap.h"

void DrawLabirint(SDL_Renderer* render, SDL_Rect* camera, Matrix* matrix)
{
	for (int i = 0; i < matrix->countCol; i++)
		for (int j = 0; j < matrix->countRow; j++)
			if (isCollided(*camera, matrix->tileArray[i][j].tileBox))
			{
				SDL_Rect rect = { matrix->tileArray[i][j].tileBox.x - camera->x,
					matrix->tileArray[i][j].tileBox.y - camera->y,
					matrix->tileArray[i][j].tileBox.w,
					matrix->tileArray[i][j].tileBox.h};
				SDL_RenderCopy(render, matrix->gTileTexture,
					&matrix->gTileClips[matrix->tileArray[i][j].tileType], &rect);
			}
}

bool InitCreateLabirint(Matrix* matrix)
{
	matrix->countCol = 4;
	matrix->countRow = 3;
	size_t matr[3][4] = { {0, 3, 6, 6},
						  {1, 4, 6, 6},
						  {2, 5, 6, 6} };
	if (!InitLabirintMatrix(matrix)) return false;
	for (int i = 0; i < matrix->countCol; i++)
		for (int j = 0; j < matrix->countRow; j++)
			matrix->tileArray[i][j].tileType = (TileType)matr[j][i];
}

bool InitLabirintMatrix(Matrix* matrix)
{
	if((matrix->tileArray = (Tile**)calloc(matrix->countCol, sizeof(Tile)))==NULL) return false;
	for (int i = 0; i < matrix->countCol; i++)
		if((matrix->tileArray[i] = (Tile*)calloc(matrix->countRow, sizeof(Tile)))==NULL) return false;

	for (int i = 0; i < matrix->countCol; i++)
		for (int j = 0; j < matrix->countRow; j++)
			matrix->tileArray[i][j].tileBox = { i * WIDTH_TILE, j * HEIGHT_TILE, WIDTH_TILE, HEIGHT_TILE};
	return true;
}