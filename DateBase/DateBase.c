#include "DateBase.h"

int SaveStructureToString(void* structure, const int sizeStructure, char** str)
{
	if (!(*str = (char*)calloc(sizeStructure, sizeof(char))))
		return -1;
	char* cin = (char*)structure;
	for (int i = 0; i < sizeStructure; i++)
		(*str)[i] = cin[i];
	return 0;
}
int LoadStructureFromString(void** structure, const int sizeStructure, char* str)
{
	if (!(*structure = (void*)calloc(sizeStructure, 1)))
		return -1;
	for (int i = 0; i < sizeStructure; i++)
		((char*)*structure)[i] = str[i];
	return 0;
}
int getIDStructure(const char* name, DateBase* dateBase)
{
	for (int i = 0; i < dateBase->countStructure; i++)
	{
		if (!strcmp(dateBase->nameStructure[i], name))
			return i;
	}
	return -1;
}

void InitDateBase(DateBase* dateBase)
{
	dateBase->countStructure = 0;
}
int AddStructureInDateBase(const char* name, void* structure, const int sizeStructure, DateBase* dateBase)
{
	if (dateBase->countStructure == MAX_STRUCTURE_COUNT)
		return -2;
	if (!(dateBase->nameStructure[dateBase->countStructure] = (char*)calloc(strlen(name) + 1, sizeof(char))))
		return -1;
	strcpy(dateBase->nameStructure[dateBase->countStructure], name);
	SaveStructureToString(structure, sizeStructure, &(dateBase->stringStructure[dateBase->countStructure]));
	dateBase->countStructure++;
	return 0;
}
int RewriteStructureInDateBase(const char* name, void* structure, const int sizeStructure, DateBase* dateBase)
{
	int id;
	if ((id = getIDStructure(name, dateBase)) == -1)
		return -1;
	free(dateBase->stringStructure[id]);
	SaveStructureToString(structure, sizeStructure, &(dateBase->stringStructure[id]));
	return 0;
}
int GetStructureFromDateBase(const char* name, void** structure, const int sizeStructure, DateBase* dateBase)
{
	int id;
	if ((id = getIDStructure(name, dateBase)) == -1)
		return -1;
	LoadStructureFromString(structure, sizeStructure, dateBase->stringStructure[id]);
	return 0;
}
int ClearDateBase(DateBase* dateBase)
{
	for (int i = 0; i < dateBase->countStructure; i++)
	{
		free(dateBase->nameStructure[i]);
		free(dateBase->stringStructure[i]);
	}
	dateBase->countStructure = 0;
}