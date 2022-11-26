#include <stdbool.h>
#include <stdio.h>

#include "mat3.h"
#include "tex1.h"

bool readMAT3(FILE* fp, struct J3DMaterial** outputArray, unsigned int* elementCount, struct JUTTexture** textureArray)
{
	long chunkStart = ftell(fp);
	if (!isMagicMatch(fp, MAGIC_MAT3))
		return false;
	fseek(fp, 4, SEEK_CUR); //Skip Chunk Size

	unsigned short elements;
	fread_e(&elements, 2, 1, fp); //Read Material count
	*elementCount = elements; //Set element count output
	fseek(fp, 2, SEEK_CUR); //Skip padding

	unsigned int dataOffset, remapTableOffset, stringTableOffset;
	fread_e(&dataOffset, 4, 1, fp); //Offset to BTI headers
	fread_e(&remapTableOffset, 4, 1, fp); //Offset to the remap table
	fread_e(&stringTableOffset, 4, 1, fp); //Offset to names

	unsigned int IndTexInfoOffset,
		CullModeInfoOffset,
		MaterialColorTableOffset,
		ColorChannelCountTableOffset,
		ColorChannelTableOffset,
		AmbientColorTableOffset,
		LightInfoOffset,
		TexGenCountTable,
		TexGenTableOffset,
		PostTexGenTableOffset,
		TexMtxTableOffset,
		PostTexMtxTableOffset,
		TextureRemapTableOffset,
		TevOrderTableOffset,
		ColorRegisterTableOffset,
		ColorConstantTableOffset,
		TevStageCountTableOffset,
		TevStageTableOffset,
		TevSwapModeInfoOffset,
		TevSwapModeTableOffset,
		FogInfoTableOffset,
		AlphaTestTableOffset,
		BlendModeTableOffset,
		ZModeTableOffset,
		ZCompareOffset,
		DitherInfoOffset,
		NBTScaleOffset;

	fread_e(&IndTexInfoOffset, 4, 1, fp);
	fread_e(&CullModeInfoOffset, 4, 1, fp);
	fread_e(&MaterialColorTableOffset, 4, 1, fp);
	fread_e(&ColorChannelCountTableOffset, 4, 1, fp);
	fread_e(&ColorChannelTableOffset, 4, 1, fp);
	fread_e(&AmbientColorTableOffset, 4, 1, fp);
	fread_e(&LightInfoOffset, 4, 1, fp);
	fread_e(&TexGenCountTable, 4, 1, fp);
	fread_e(&TexGenTableOffset, 4, 1, fp);
	fread_e(&PostTexGenTableOffset, 4, 1, fp);
	fread_e(&TexMtxTableOffset, 4, 1, fp);
	fread_e(&PostTexMtxTableOffset, 4, 1, fp);
	fread_e(&TextureRemapTableOffset, 4, 1, fp);
	fread_e(&TevOrderTableOffset, 4, 1, fp);
	fread_e(&ColorRegisterTableOffset, 4, 1, fp);
	fread_e(&ColorConstantTableOffset, 4, 1, fp);
	fread_e(&TevStageCountTableOffset, 4, 1, fp);
	fread_e(&TevStageTableOffset, 4, 1, fp);
	fread_e(&TevSwapModeInfoOffset, 4, 1, fp);
	fread_e(&TevSwapModeTableOffset, 4, 1, fp);
	fread_e(&FogInfoTableOffset, 4, 1, fp);
	fread_e(&AlphaTestTableOffset, 4, 1, fp);
	fread_e(&BlendModeTableOffset, 4, 1, fp);
	fread_e(&ZModeTableOffset, 4, 1, fp);
	fread_e(&ZCompareOffset, 4, 1, fp);
	fread_e(&DitherInfoOffset, 4, 1, fp);
	fread_e(&NBTScaleOffset, 4, 1, fp);

	//Handle the Remap Table first
	fseek(fp, chunkStart + remapTableOffset, SEEK_SET);
	unsigned short* RemapTable = calloc(*elementCount, 2);
	if (RemapTable == NULL)
		return false;

#if RELEASE
	fread_e(RemapTable, 2, *elementCount, fp);
#else
	bool isCompressed = false;
	for (size_t i = 0; i < elementCount; i++)
	{
		fread_e(&RemapTable[i], 2, 1, fp);
		if (!isCompressed && RemapTable[i] != i)
			isCompressed = true;
	}
	if (isCompressed)
	{
		//Output to the console
	}
#endif

	//read the string table
	fseek(fp, chunkStart + stringTableOffset, SEEK_SET);
	char** stringTable = readStringTable(fp);
	
	outputArray = calloc((size_t)(*elementCount + 1), sizeof(struct J3DMaterial*));
	if (outputArray == NULL)
		return false;

	for (size_t i = 0; i < *elementCount; i++)
	{
		struct J3DMaterial* current = calloc(1, sizeof(struct J3DMaterial));
		if (current == NULL)
			return false;

		current->Name = stringTable[i]; //Names ignore the Remap Table

		fseek(fp, chunkStart + dataOffset + (RemapTable[i] * MATERIAL_ENTRY_SIZE), SEEK_SET);

		fread_e(&current->MaterialMode, 1, 1, fp);
		readFromTable(&current->Culling, 1, 4, fp, chunkStart, CullModeInfoOffset);
	}
}

bool readFromTable(void* _Buffer, size_t IndexSize, size_t ElementSize, FILE* _Stream, long ChunkStart, long TableOffset)
{
	void* Index = calloc(1, IndexSize);
	if (Index == NULL)
		return false;

	fread_e(Index, IndexSize, 1, _Stream);
	long pausePosition = ftell(_Stream);

	if (IndexSize == 1)
		fseek(_Stream, ChunkStart + TableOffset + (*(char*)Index) * ElementSize, SEEK_SET);
	else if (IndexSize == 2)
		fseek(_Stream, ChunkStart + TableOffset + (*(unsigned short*)Index) * ElementSize, SEEK_SET);
	else if (IndexSize == 4)
		fseek(_Stream, ChunkStart + TableOffset + (*(unsigned int*)Index) * ElementSize, SEEK_SET);

	fread_e(_Buffer, ElementSize, 1, _Stream);
	fseek(_Stream, pausePosition, SEEK_SET);
	return true;
}

bool matcmp(struct J3DMaterial* mat1, struct J3DMaterial* mat2) {
	if (mat1 == mat2)
		return true;

	if (strcmp(mat1->Name, mat2->Name))
		return false;

	if (mat1->MaterialMode != mat2->MaterialMode)
		return false;

	if (mat1->EnableDithering != mat2->EnableDithering)
		return false;

	for (int i = 0; i < 8; ++i) {
		if (!texcmp(mat1->Textures[i], mat2->Textures[i]))
			return false;
	}

	for (int i = 0; i < 10; ++i) {
		if (memcmp(mat1->TextureMatricies, mat2->TextureMatricies,
		           sizeof (struct TextureMatrix)))
			return false;
	}

	for (int i = 0; i < 3; ++i) {
		if (memcmp(&mat1->IndirectTextureMatricies, &mat2->IndirectTextureMatricies,
                    sizeof (union Matrix2x4f)))
			return false;
	}

	for (int i = 0; i < 2; ++i) {
		if (memcmp(&mat1->ColorMatRegs, &mat2->ColorMatRegs, sizeof (union Vector4uc)))
			return false;
	}

	for (int i = 0; i < 2; ++i) {
		if (memcmp(&mat1->ColorAmbRegs, &mat2->ColorAmbRegs, sizeof (union Vector4uc)))
			return false;
	}

	for (int i = 0; i < 4; ++i) {
		if (memcmp(&mat1->ColorConstants, &mat2->ColorConstants, sizeof (union Vector4uc)))
			return false;
	}

	for (int i = 0; i < 4; ++i) {
		if (memcmp(&mat1->ColorRegisters, &mat2->ColorRegisters, sizeof (union Vector4s)))
			return false;
	}

	if (memcmp(mat1->FogInfo, mat2->FogInfo, sizeof (struct Fog)))
		return false;

	if (mat1->Culling != mat2->Culling)
		return false;

	for (int i = 0; i < 2; ++i) {
		if (memcmp(mat1->LightChannels[i]->Color, mat2->LightChannels[i]->Color,
		           sizeof (struct ColorChannelControl)))
			return false;

		if (memcmp(mat1->LightChannels[i]->Alpha, mat2->LightChannels[i]->Alpha,
		           sizeof (struct ColorChannelControl)))
			return false;
	}

	for (int i = 0; i < 8; ++i) {
		if (memcmp(mat1->TextureGenerators[i], mat2->TextureGenerators[i],
		           sizeof (struct TextureGenerator)))
			return false;
	}

	for (int i = 0; i < 16; ++i) {
		if (memcmp(mat1->TEVStages, mat2->TEVStages, sizeof (struct TextureEnvironmentStage)))
			return false;
	}

	for (int i = 0; i < 4; ++i) {
		if (memcmp(mat1->IndirectStages, mat2->IndirectStages, sizeof (struct IndirectTextureStage)))
			return false;
	}

	if (memcmp(mat1->AlphaTest, mat2->AlphaTest, sizeof (struct AlphaTest)))
		return false;

	if (memcmp(mat1->BlendInfo, mat2->BlendInfo, sizeof (struct Blend)))
		return false;

	if (memcmp(mat1->NBTScale, mat2->NBTScale, sizeof (struct NBT)))
		return false;

	return true;
}


