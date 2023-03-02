#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "superbdl.h"
#include "endianio.h"

bool byte_swap = BYTE_ORDER == LITTLE_ENDIAN;

bool isMagicMatch(FILE* fp, const char* target)
{
	int magic;
	size_t size = strlen(target);
	fread(&magic, size, 1, fp);
	return memcmp(target, &magic, size) == 0;
}

char** readStringTable(FILE* fp)
{
	long tableStart = ftell(fp);
	unsigned short stringCount;
	fread_e(&stringCount, 2, 1, fp);
	char** string_array = calloc((size_t)(stringCount+1), sizeof(char*));
	fseek(fp, 2, SEEK_CUR);

	char** arrayptr = string_array;
	for (int i = 0; i < stringCount; i++)
	{
		fseek(fp, 2, SEEK_CUR); //Skip the hash
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

size_t fread_e(void* _Buffer, size_t ElementSize, size_t ElementCount, FILE* _Stream)
{
	if (!byte_swap || ElementSize == 1)
		goto Normal;

	size_t Counter = 0;

	if (ElementSize == 2)
	{
		Counter = fread(_Buffer, ElementSize, ElementCount, _Stream);
		unsigned short* ptr = (unsigned short*)_Buffer;
		for (size_t i = 0; i < ElementCount; i++)
		{
			ptr[i] = bswap_16(ptr[i]);
		}
		return Counter;
	}

	if (ElementSize == 4)
	{
		long p = ftell(_Stream);
		Counter = fread(_Buffer, ElementSize, ElementCount, _Stream);
		unsigned int* ptr = (unsigned int*)_Buffer;
		for (size_t i = 0; i < ElementCount; i++)
		{
			ptr[i] = bswap_32(ptr[i]);
		}
		return Counter;
	}

Normal:
	return fread(_Buffer, ElementSize, ElementCount, _Stream);
}

size_t fwrite_e(const void* _Buffer, size_t ElementSize, size_t ElementCount, FILE* _Stream)
{
	if (!byte_swap || ElementSize == 1)
		goto Normal; //Optimization since Bytes aren't swapped

	if (ElementSize == 2)
	{
		const unsigned short *src = _Buffer;
		unsigned short* ptr = calloc(ElementCount, ElementSize);
		if (ptr == NULL)
			return -1;
		for (size_t i = 0; i < ElementCount; i++)
		{
			ptr[i] = bswap_16(src[i]);
		}
		return fwrite(ptr, ElementSize, ElementCount, _Stream);
	}

	if (ElementSize == 4)
	{
		const unsigned int *src = _Buffer;
		unsigned int* ptr = calloc(ElementCount, ElementSize);
		if (ptr == NULL)
			return -1;
		for (size_t i = 0; i < ElementCount; i++)
		{
			ptr[i] = bswap_32(src[i]);
		}
		return fwrite(ptr, ElementSize, ElementCount, _Stream);
	}

	Normal:
	return fwrite(_Buffer, ElementSize, ElementCount, _Stream);
}
