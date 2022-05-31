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
	int countStructure;
}DateBase;

//������������� ��
void InitDateBase(DateBase* dateBase);
//��������� ��������� � ������, ���� �� ������� ����� ������������ -1, ���� ���������� ����������� � MAX_STRUCTURE_COUNT ������������ -2
int AddStructureInDateBase(const char* name, void* structure, const int sizeStructure, DateBase* dateBase);
//���������� ������ ��������� � ������ name �� ����� ��������� structure
int RewriteStructureInDateBase(const char* name, void* structure, const int sizeStructure, DateBase* dateBase);
//�������� ��������� �� ����� name � structure, ��� ���������� name ���������� -1
int GetStructureFromDateBase(const char* name, void** structure, const int sizeStructure, DateBase* dateBase);
//������������ ������ �� ��
int ClearDateBase(DateBase* dateBase);
#endif
