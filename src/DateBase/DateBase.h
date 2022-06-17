#ifndef DATEBASELIB
#define DATEBASELIB

#define _CRT_SECURE_NO_WARNINGS
#define MAX_STRUCTURE_COUNT 256

#include <stdlib.h>
#include <string.h>

enum DataType
{
	HOST,
	CLIENT
};

typedef struct DateBase
{
	char* nameStructure[MAX_STRUCTURE_COUNT];
	char* stringStructure[MAX_STRUCTURE_COUNT];
	int sizesStructure[MAX_STRUCTURE_COUNT];
	int countStructure;
}DateBase;

int SaveStructureToString(void* structure, const int sizeStructure, char** str);
int LoadStructureFromString(void** structure, const int sizeStructure, char* str);
int getIDStructure(const char* name, DateBase* dateBase);

//Инициализация БД
void InitDateBase(DateBase* dateBase);
//Добавляет структуру в массив, если не хватает места возвращается -1, если достигнуто ограничение в MAX_STRUCTURE_COUNT возвращается -2
int AddStructureInDateBase(const char* name, void* structure, const int sizeStructure, DateBase* dateBase);
//Перезапись ячейки структуры с именем name на новую структуру structure
int RewriteStructureInDateBase(const char* name, void* structure, const int sizeStructure, DateBase* dateBase);
//Получает структуру по имени name в structure, при отсутствии name возвращает -1
int GetStructureFromDateBase(const char* name, void** structure, const int sizeStructure, DateBase* dateBase);
//Освобождение памяти от БД
int ClearDateBase(DateBase* dateBase);
#endif
