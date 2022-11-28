#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "superbdl.h"
#include "endianio.h"

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

unsigned short LE16H(unsigned short x)
{
	return le16toh(x);
}
unsigned int LE32H(unsigned int x)
{
	return le32toh(x);
}
unsigned short BE16H(unsigned short x)
{
	return be16toh(x);
}
unsigned int BE32H(unsigned int x)
{
	return be32toh(x);
}

unsigned short H16LE(unsigned short x)
{
	return htole16(x);
}
unsigned int H32LE(unsigned int x)
{
	return htole32(x);
}
unsigned short H16BE(unsigned short x)
{
	return htobe16(x);
}
unsigned int H32BE(unsigned int x)
{
	return htobe32(x);
}

size_t fread_e(void* _Buffer, size_t ElementSize, size_t ElementCount, FILE* _Stream)
{
	if (ElementSize == 1)
		goto Normal;

	size_t Counter = 0;
	unsigned short (*en16toh_ptr)(unsigned short);
	unsigned int (*en32toh_ptr)(unsigned int);
	if (false)
	{
		en16toh_ptr = &LE16H;
		en32toh_ptr = &LE32H;
	}
	else
	{
		en16toh_ptr = &BE16H;
		en32toh_ptr = &BE32H;
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
		long p = ftell(_Stream);
		Counter = fread(_Buffer, ElementSize, ElementCount, _Stream);
		unsigned int* ptr = (unsigned int*)_Buffer;
		for (size_t i = 0; i < ElementCount; i++)
		{
			ptr[i] = en32toh_ptr(ptr[i]);
		}
		return Counter;
	}

Normal:
	return fread(_Buffer, ElementSize, ElementCount, _Stream);
}

size_t fwrite_e(void* _Buffer, size_t ElementSize, size_t ElementCount, FILE* _Stream)
{
	if (ElementSize == 1)
		goto Normal; //Optimization since Bytes aren't swapped

	unsigned short (*func16_ptr)(unsigned short);
	unsigned int (*func32_ptr)(unsigned int);
	if (false)
	{
		func16_ptr = &H16LE;
		func32_ptr = &H32LE;
	}
	else
	{
		func16_ptr = &H16BE;
		func32_ptr = &H32BE;
	}

	if (ElementSize == 2)
	{
		unsigned short* ptr = calloc(ElementCount, ElementSize);
		if (ptr == NULL)
			return -1;
		memcpy(ptr, _Buffer, ElementCount * ElementSize);
		for (size_t i = 0; i < ElementCount; i++)
		{
			ptr[i] = func16_ptr(ptr[i]);
		}
		return fwrite(ptr, ElementSize, ElementCount, _Stream);
	}

	if (ElementSize == 4)
	{
		unsigned int* ptr = calloc(ElementCount, ElementSize);
		if (ptr == NULL)
			return -1;
		memcpy(ptr, _Buffer, ElementCount * ElementSize);
		for (size_t i = 0; i < ElementCount; i++)
		{
			ptr[i] = func32_ptr(ptr[i]);
		}
		return fwrite(ptr, ElementSize, ElementCount, _Stream);
	}

	Normal:
	return fwrite(_Buffer, ElementSize, ElementCount, _Stream);
}
