#include <stdbool.h>
#include <stdio.h>

#include "mat3.h"
#include "tex1.h"

bool readMAT3(FILE* fp, struct J3DMaterial** outputArray, unsigned int* elementCount, struct JUTTexture** textureArray)
{
	if (elementCount == NULL)
		return false; //Why

	long chunkStart = ftell(fp);
	if (!isMagicMatch(fp, MAGIC_MAT3))
		return false;
	fseek(fp, 4, SEEK_CUR); //Skip Chunk Size

	unsigned short matCount;
	fread_e(&matCount, 2, 1, fp); //Read Material count
	*elementCount = matCount; //Set element count output
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
	unsigned short* RemapTable = calloc(matCount, 2);
	if (RemapTable == NULL)
		return false;

#if RELEASE
	fread_e(RemapTable, 2, matCount, fp);
#else
	bool isCompressed = false;
	for (size_t i = 0; i < matCount; i++)
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
	
	outputArray = calloc((size_t)(matCount + 1), sizeof(struct J3DMaterial*));
	if (outputArray == NULL)
		return false;

	//Now to read the materials themselves
	for (size_t i = 0; i < matCount; i++)
	{
		struct J3DMaterial* current = calloc(1, sizeof(struct J3DMaterial));
		if (current == NULL)
			return false;

		current->Name = stringTable[i]; //Names ignore the Remap Table

		fseek(fp, chunkStart + dataOffset + (RemapTable[i] * MATERIAL_ENTRY_SIZE), SEEK_SET);

		unsigned char LightChannelCount,
			TexGenNum,
			TevStageNum,
			ZCompLocIndex,
			ZModeIndex;

		RETURN_FALSE_IF_FAIL(fread_e(&current->MaterialMode, 1, 1, fp), 1);

		RETURN_FALSE_IF_FALSE(readFromTable(&current->Culling, 1, 4, fp, chunkStart, CullModeInfoOffset));
		RETURN_FALSE_IF_FALSE(readFromTable(&LightChannelCount, 1, 1, fp, chunkStart, ColorChannelCountTableOffset));
		assert(LightChannelCount <= 0x02);
		RETURN_FALSE_IF_FALSE(readFromTable(&TexGenNum, 1, 1, fp, chunkStart, TexGenCountTable));
		RETURN_FALSE_IF_FALSE(readFromTable(&TevStageNum, 1, 1, fp, chunkStart, TevStageCountTableOffset));
		RETURN_FALSE_IF_FALSE(readFromTable(&ZCompLocIndex, 1, 1, fp, chunkStart, ZCompareOffset));
		RETURN_FALSE_IF_FALSE(readFromTable(&ZModeIndex, 1, 1, fp, chunkStart, ZModeTableOffset));
		RETURN_FALSE_IF_FALSE(readFromTable(&current->EnableDithering, 1, 1, fp, chunkStart, DitherInfoOffset));








		for (size_t i = 0; i < LightChannelCount; i++)
		{
			struct LightChannelControl* LCC = calloc(1, sizeof(struct LightChannelControl));
			struct ColorChannelControl* Color = calloc(1, sizeof(struct ColorChannelControl));
			struct ColorChannelControl* Alpha = calloc(1, sizeof(struct ColorChannelControl));
			RETURN_FALSE_IF_NULL(Color);
			RETURN_FALSE_IF_NULL(Alpha);
			
			RETURN_FALSE_IF_FALSE(readFromTableWithFunc(Color, 2, sizeof(struct ColorChannelControl), &readColorChannel, fp, chunkStart, ColorChannelTableOffset));
			RETURN_FALSE_IF_FALSE(readFromTableWithFunc(Alpha, 2, sizeof(struct ColorChannelControl), &readColorChannel, fp, chunkStart, ColorChannelTableOffset));

			LCC->Color = Color;
			LCC->Alpha = Alpha;
			current->LightChannels[i] = LCC;
		}

	}

	return true;
}

bool readFromTable(void* _Buffer, size_t IndexSize, size_t ElementSize, FILE* _Stream, long ChunkStart, long TableOffset)
{
	void* Index = calloc(1, IndexSize);
	RETURN_FALSE_IF_NULL(Index);

	fread_e(Index, IndexSize, 1, _Stream);
	long pausePosition = ftell(_Stream);

	if (IndexSize == 1)
		fseek(_Stream, ChunkStart + TableOffset + (*(char*)Index) * (long)ElementSize, SEEK_SET);
	else if (IndexSize == 2)
		fseek(_Stream, ChunkStart + TableOffset + (*(unsigned short*)Index) * (long)ElementSize, SEEK_SET);
	else if (IndexSize == 4)
		fseek(_Stream, ChunkStart + TableOffset + (*(unsigned int*)Index) * (long)ElementSize, SEEK_SET);

	RETURN_FALSE_IF_FAIL(fread_e(_Buffer, ElementSize, 1, _Stream), ElementSize);

	fseek(_Stream, pausePosition, SEEK_SET);
	return true;
}

bool readFromTableWithFunc(void* _Buffer, size_t IndexSize, size_t ElementSize, bool (*func_ptr)(void*, FILE*), FILE* _Stream, long ChunkStart, long TableOffset)
{
	void* Index = calloc(1, IndexSize);
	RETURN_FALSE_IF_NULL(Index);

	fread_e(Index, IndexSize, 1, _Stream);
	long pausePosition = ftell(_Stream);

	if (IndexSize == 1)
		fseek(_Stream, ChunkStart + TableOffset + (*(char*)Index) * (long)ElementSize, SEEK_SET);
	else if (IndexSize == 2)
		fseek(_Stream, ChunkStart + TableOffset + (*(unsigned short*)Index) * (long)ElementSize, SEEK_SET);
	else if (IndexSize == 4)
		fseek(_Stream, ChunkStart + TableOffset + (*(unsigned int*)Index) * (long)ElementSize, SEEK_SET);

	RETURN_FALSE_IF_FALSE(func_Ptr(_Buffer, _Stream));

	fseek(_Stream, pausePosition, SEEK_SET);
	return true;
}

bool readColorChannel(void* _Buffer,FILE* fp)
{
	RETURN_FALSE_IF_NULL(_Buffer);

	struct ColorChannelControl* ptr = (struct ColorChannelControl*)_Buffer;

	fread_e(&ptr->LightingEnabled, 1, 1, fp);
	fread_e(&ptr->MaterialColorSource, 1, 1, fp);
	fread_e(&ptr->LightMask, 1, 1, fp);
	fread_e(&ptr->DiffuseFunc, 1, 1, fp);
	fread_e(&ptr->AttenuationFunc, 1, 1, fp);
	fread_e(&ptr->AmbientColorSource, 1, 1, fp);

	return true;
}

bool matcmp(struct J3DMaterial* mat1, struct J3DMaterial* mat2) {
	if (mat1 == mat2)
		return true;

	if (mat1 == NULL || mat2 == NULL) //Don't bother checking to see if the other is NOT NULL because if both are NULL they would've already exited above
		return false;

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
