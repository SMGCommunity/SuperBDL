#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "endianio.h"

#define MAX_STRING_ALLOCATION 256

bool isMagicMatch(FILE* fp, const char* target)
{
	int magic;
	int size = strlen(target);
	fread(&magic, size, 1, fp);
	return memcmp(target, &magic, size) == 0;
}

int readReverseUint32(FILE* fp, unsigned int* output)
{
	size_t result = fread(output, 4, 1, fp);
	*output = be32toh(*output);
	return result;
}
int readReverseUint16(FILE* fp, unsigned short* output)
{
	size_t result = fread(output, 2, 1, fp);
	*output = be16toh(*output);
	return result;
}
int writeReverseUint32(FILE* fp, unsigned int* output)
{
	unsigned int v = *output;
	v = htobe32(v);
	size_t result = fwrite(&v, 4, 1, fp);
	return result;
}
int writeReverseUint16(FILE* fp, unsigned short* output)
{
	unsigned short v = *output;
	v = htobe16(v);
	size_t result = fwrite(&v, 2, 1, fp);
	return result;
}

char** readStringTable(FILE* fp)
{
	long tableStart = ftell(fp);
	unsigned short stringCount;
	fread(&stringCount, 2, 1, fp);
	char** string_array = calloc((stringCount+1), sizeof(char*));
	fseek(fp, 2, SEEK_CUR);

	char** arrayptr = string_array;
	for (int i = 0; i < stringCount; i++)
	{
		fseek(fp, 4, SEEK_CUR); //Skip the hash
		char* string = malloc(MAX_STRING_ALLOCATION);
		unsigned short stringOffset;
		fread(&stringOffset, 2, 1, fp);
		long PausePosition = ftell(fp);
		fseek(fp, tableStart + stringOffset, SEEK_SET);
		fgets(string, MAX_STRING_ALLOCATION, fp);
		fseek(fp, PausePosition, SEEK_SET);
		string_array[i] = string;
	}

	return string_array;
}
