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
	*output = _byteswap_ulong(*output);
	return result;
}
int readReverseUint16(FILE* fp, unsigned short* output)
{
	size_t result = fread(output, 2, 1, fp);
	*output = _byteswap_ushort(*output);
	return result;
}
int writeReverseUint32(FILE* fp, unsigned int* output)
{
	unsigned int v = *output;
	v = _byteswap_ulong(v);
	size_t result = fwrite(&v, 4, 1, fp);
	return result;
}
int writeReverseUint16(FILE* fp, unsigned short* output)
{
	unsigned short v = *output;
	v = _byteswap_ushort(v);
	size_t result = fwrite(&v, 2, 1, fp);
	return result;
}