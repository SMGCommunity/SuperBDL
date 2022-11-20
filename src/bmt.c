#include "bmt.h"

bool readBMT(FILE* fp, struct BMT* output)
{
	//Read Magic

	fseek(fp, 0x20, SEEK_SET);

	return true;
}

struct J3DMaterial* getMaterial(struct BMT* bmt, const char* name)
{
	return NULL;
}

struct JUTTexture* getTexture(struct BMT* bmt, const char* name)
{
	return NULL;
}