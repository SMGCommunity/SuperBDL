#include "bmt.h"

bool readBMT(FILE* fp, struct BMT* output)
{
	//Read Magic

	fseek(fp, 0x20, SEEK_SET);
	unsigned int MAT3 = getChunkPosition(fp, MAGIC_MAT3);
	unsigned int TEX1 = getChunkPosition(fp, MAGIC_TEX1);

	//Reset element counts (in case they were not initlized to 0...Do we need to do this?)
	output->MaterialCount = 0;
	output->TextureCount = 0;

	if (MAT3 != 0)
	{
		fseek(fp, MAT3, SEEK_SET);
		if (!readMAT3(fp, output->Materials, &(output->MaterialCount)))
		{
			return false; //Is this really it for Error handling?
		}
	}
	if (TEX1 != 0)
	{
		fseek(fp, TEX1, SEEK_SET);
		if (!readTEX1(fp, output->Textures, &(output->TextureCount)))
		{
			return false; //Is this really it for Error handling?
		}
	}

	return true;
}

struct J3DMaterial* getMaterial(struct BMT* bmt, const char* name)
{
	struct J3DMaterial* ptr = bmt->Materials;
	for (size_t i = 0; i < bmt->MaterialCount; i++)
	{
		if (!strcmp(name, ptr->Name))
			return ptr;
	}

	return NULL;
}

struct JUTTexture* getTexture(struct BMT* bmt, const char* name)
{
	struct JUTTexture* ptr = bmt->Textures;
	for (size_t i = 0; i < bmt->TextureCount; i++)
	{
		if (!strcmp(name, ptr->Name))
			return ptr;
	}

	return NULL;
}