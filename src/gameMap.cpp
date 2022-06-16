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

bool InitMatrix(size_t** matr, Matrix* matrix)
{
	for (int i = 0; i < matrix->countRow; i++)
		for (int j = 0; j < matrix->countCol; j++)
			matr[i][j] = DARK_GROUND;
	
	size_t row = ((matrix->countRow - 1) / 2);
	size_t col = ((matrix->countCol - 1) / 2);
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			matr[i * 2][j * 2] = CRAVED_STONE;
			matr[i * 2 + 1][j * 2] = CRAVED_STONE;
			matr[i * 2][j * 2 + 1] = CRAVED_STONE;
		}
	for (int i = 0; i < matrix->countRow; i++)
		matr[i][matrix->countCol - 1] = DARK_CLEAR_STONE;
	for (int j = 0; j < matrix->countCol; j++)
		matr[matrix->countRow - 1][j] = DARK_CLEAR_STONE;
	for (int i = 0; i < matrix->countRow; i++)
		matr[i][0] = DARK_CLEAR_STONE;
	for (int j = 0; j < matrix->countCol; j++)
		matr[0][j] = DARK_CLEAR_STONE;
	
	//the current coordinates on holes
	size_t x = 0, y = 0;
	matr[y * 2 + 1][x * 2 + 1] = GROUND;
	//queue of labirint path on holes
	size_t size_queue = 0;
	size_t* queue_x = NULL;
	size_t* queue_y = NULL;
	if ((queue_x = (size_t*)calloc(row * col, sizeof(size_t))) == NULL) return false;
	if ((queue_y = (size_t*)calloc(row * col, sizeof(size_t))) == NULL) return false;
	//Can we went on this side
	bool right, left, up, down;
	do{
		right = ((matr[y * 2 + 1][x * 2 + 1 + 1] != DARK_CLEAR_STONE) && (matr[y * 2 + 1][x * 2 + 1 + 2] == DARK_GROUND));
		left = ((matr[y * 2 + 1][x * 2 + 1 - 1] != DARK_CLEAR_STONE) && (matr[y * 2 + 1][x * 2 + 1 - 2] == DARK_GROUND));
		up = ((matr[y * 2 + 1 + 1][x * 2 + 1] != DARK_CLEAR_STONE) && (matr[y * 2 + 1 + 2][x * 2 + 1] == DARK_GROUND));
		down = ((matr[y * 2 + 1 - 1][x * 2 + 1] != DARK_CLEAR_STONE) && (matr[y * 2 + 1 - 2][x * 2 + 1] == DARK_GROUND));
		if (right || left || up || down)
		{
			queue_x[size_queue] = x;
			queue_y[size_queue] = y;
			size_queue++;

			size_t countPaths = (((size_t)right) + ((size_t)left) + ((size_t)up) + ((size_t)down));
			size_t randPathNum = rand() % countPaths + 1;
			if (right) randPathNum--;
			if (randPathNum == 0)
			{
				matr[y * 2 + 1][x * 2 + 1 + 1] = GROUND;
				matr[y * 2 + 1][x * 2 + 1 + 2] = GROUND;
				x++;
				continue;
			}
			if (left) randPathNum--;
			if (randPathNum == 0)
			{
				matr[y * 2 + 1][x * 2 + 1 - 1] = GROUND;
				matr[y * 2 + 1][x * 2 + 1 - 2] = GROUND;
				x--;
				continue;
			}
			if (up) randPathNum--;
			if (randPathNum == 0)
			{
				matr[y * 2 + 1 + 1][x * 2 + 1] = GROUND;
				matr[y * 2 + 1 + 2][x * 2 + 1] = GROUND;
				y++;
				continue;
			}
			if (down) randPathNum--;
			if (randPathNum == 0)
			{
				matr[y * 2 + 1 - 1][x * 2 + 1] = GROUND;
				matr[y * 2 + 1 - 2][x * 2 + 1] = GROUND;
				y--;
				continue;
			}
		}
		else
		{
			//if we're at a dead end
			if (size_queue != 0)
			{
				x = queue_x[size_queue - 1];
				y = queue_y[size_queue - 1];
				size_queue--;
			}
			else
			{
				printf("Labirint Matrix creating - ERORR!\n");
				return false;
			}
		}
	} while (size_queue != 0);
	free(queue_x);
	free(queue_y);
	return true;
}

void ColoringLabirint(size_t** matr, Matrix* matrix)
{
	for (int i = 0; i < matrix->countRow; i++)
	{
		for (int j = 0; j < matrix->countCol; j++)
		{
			if (matr[i][j] == GROUND)
			{
				int r = rand() % 4;
				switch (r)
				{
				case 0:
					matr[i][j] = GROUND;
					break;
				case 1:
					matr[i][j] = DARK_GROUND;
					break;
				case 2:
					matr[i][j] = BRICKED_GROUND;
				case 3:
					matr[i][j] = DARK_BRICKED_GROUND;
					break;
				}
			}
			else if (matr[i][j] == CRAVED_STONE)
			{
				int r = rand() % 7;
				switch (r)
				{
				case 0:
					matr[i][j] = CLEAR_STONE;
					break;
				case 1:
					matr[i][j] = DIRTY_STONE;
					break;
				case 2:
					matr[i][j] = POLISHED_STONE;
					break;
				case 3:
					matr[i][j] = CRAVED_STONE;
					break;
				case 4:
					matr[i][j] = DARK_DIRTY_STONE;
					break;
				case 5:
					matr[i][j] = DARK_POLISHED_STONE;
					break;
				case 6:
					matr[i][j] = DARK_CRAVED_STONE;
					break;
				}
			}
		}
	}
}

bool InitCreateLabirint(Matrix* matrix, CollidersArray* colArr)
{
	srand((unsigned int)time(NULL));
	int r = rand() % 20;
	matrix->countCol = r * 2 + 21;
	matrix->countRow = r * 2 + 21;

	printf("Labirint with Col:%u and Row:%u\n", matrix->countCol, matrix->countRow);
	size_t** matr = NULL;
	if((matr = (size_t**)calloc(matrix->countRow, sizeof(size_t*)))==NULL) return false;
	for (int i = 0; i < matrix->countRow; i++)
		if((matr[i] = (size_t*)calloc(matrix->countCol, sizeof(size_t)))==NULL) return false;
	if(!InitMatrix(matr, matrix)) return false;
	ColoringLabirint(matr, matrix);
	if (!InitLabirintMatrix(matrix)) return false;
	if (!AddLabirintColliders(matrix, colArr, matr)) return false;
}

bool AddLabirintColliders(Matrix* matrix, CollidersArray* colArr, size_t** matr)
{
	for (int i = 0; i < matrix->countCol; i++)
		for (int j = 0; j < matrix->countRow; j++)
		{
			matrix->tileArray[i][j].tileType = (TileType)matr[j][i];
			if ((matrix->tileArray[i][j].tileType != GROUND) &&
				(matrix->tileArray[i][j].tileType != DARK_GROUND) &&
				(matrix->tileArray[i][j].tileType != BRICKED_GROUND) &&
				(matrix->tileArray[i][j].tileType != DARK_BRICKED_GROUND))
			{
				BoxCollider* ptr;
				AddColliderInArray(colArr, matrix->tileArray[i][j].collider = CreateCollider(ptr = CreateBoxCollider({ 0,0,0,0 }), BOX, WALL_COL_ID));
				matrix->tileArray[i][j].collider->collider = ptr;
				matrix->tileArray[i][j].collider->active = true;
			}
			else
				matrix->tileArray[i][j].collider = NULL;
		}
	return true;
}

bool InitLabirintMatrix(Matrix* matrix)
{
	if((matrix->tileArray = (Tile**)calloc(matrix->countCol, sizeof(Tile*)))==NULL) return false;
	for (int i = 0; i < matrix->countCol; i++)
		if((matrix->tileArray[i] = (Tile*)calloc(matrix->countRow, sizeof(Tile)))==NULL) return false;

	for (int i = 0; i < matrix->countCol; i++)
		for (int j = 0; j < matrix->countRow; j++)
			matrix->tileArray[i][j].tileBox = { i * WIDTH_TILE, j * HEIGHT_TILE, WIDTH_TILE, HEIGHT_TILE};
	return true;
}