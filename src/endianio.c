#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "endianio.h"

bool isMagicMatch(FILE* fp, const char* target)
{
	int magic;
	int size = strlen(target);
	fread(&magic, size, 1, fp);
	return memcmp(target, &magic, size);
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
