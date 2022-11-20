#include <stdbool.h>
#include <stdio.h>

#include "endianio.h"
#include "bmd.h"

unsigned int getChunkPosition(FILE* fp, const char* magic)
{
	unsigned int pausePosition; //The legend himself
	pausePosition = ftell(fp);

	//Assume we're at the start of a chunk right now
	while (!feof(fp))
	{
		//Read Chunk Magic
		if (isMagicMatch(fp, magic))
		{
			fseek(fp, -4, SEEK_CUR);
			return ftell(fp);
		}

		unsigned int ChunkSize;
		fread(&ChunkSize, 4, 1, fp);
		ChunkSize = be32toh(ChunkSize);
		fseek(fp, ChunkSize-0x08, SEEK_CUR); //minus 0x08 because we read 8 bytes from the current header
	}

	fseek(fp, pausePosition, SEEK_SET);
	return 0; //Zero means "Not Found"
}

bool write_bmd(const struct aiScene *model_data, FILE *output_fp, bool write_bdl, bool sm3das) {
	fwrite(MAGIC_J3D, 4, 1, output_fp);

	if (write_bdl)
		fwrite(MAGIC_BDL, 5, 1, output_fp);
	else
		fwrite(MAGIC_BMD, 5, 1, output_fp);

	// true for success, false for error
	return true;
}
