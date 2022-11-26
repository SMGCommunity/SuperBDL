#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "endianio.h"

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
	fread_e(&stringCount, 2, 1, fp);
	char** string_array = calloc((stringCount+1), sizeof(char*));
	fseek(fp, 2, SEEK_CUR);

	char** arrayptr = string_array;
	for (int i = 0; i < stringCount; i++)
	{
		fseek(fp, 4, SEEK_CUR); //Skip the hash
		char* string = malloc(MAX_STRING_ALLOCATION);
		unsigned short stringOffset;
		fread_e(&stringOffset, 2, 1, fp);
		long PausePosition = ftell(fp);
		fseek(fp, tableStart + stringOffset, SEEK_SET);
		fgets(string, MAX_STRING_ALLOCATION, fp);
		fseek(fp, PausePosition, SEEK_SET);
		string_array[i] = string;
	}

	return string_array;
}

unsigned short leswap16(unsigned short x)
{
	return le16toh(x);
}
unsigned int leswap32(unsigned int x)
{
	return le32toh(x);
}
unsigned short beswap16(unsigned short x)
{
	return be16toh(x);
}
unsigned int beswap32(unsigned int x)
{
	return be32toh(x);
}

int fread_e(void* _Buffer, size_t ElementSize, size_t ElementCount, FILE* _Stream)
{
	if (ElementSize == 1)
		return fread(_Buffer, ElementSize, ElementCount, _Stream);

	int Counter = 0;
	unsigned short (*en16toh_ptr)(unsigned short);
	unsigned short (*en32toh_ptr)(unsigned short);
	if (false)
	{
		en16toh_ptr = &leswap16;
		en32toh_ptr = &leswap32;
	}
	else
	{
		en16toh_ptr = &beswap16;
		en32toh_ptr = &beswap32;
	}

	if (ElementSize == 2)
	{
		Counter = fread(_Buffer, ElementSize, ElementCount, _Stream);
		unsigned short* ptr = (unsigned short*)_Buffer;
		for (size_t i = 0; i < ElementCount; i++)
		{
			ptr[i] = en16toh_ptr(ptr[i]);
		}
		return Counter;
	}

	if (ElementSize == 4)
	{
		Counter = fread(_Buffer, ElementSize, ElementCount, _Stream);
		unsigned int* ptr = (unsigned int*)_Buffer;
		for (size_t i = 0; i < ElementCount; i++)
		{
			ptr[i] = en32toh_ptr(ptr[i]);
		}
		return Counter;
	}
}