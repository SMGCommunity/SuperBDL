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
		ZCompareTableOffset,
		ZEarlyTestTableOffset,
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
	fread_e(&ZCompareTableOffset, 4, 1, fp);
	fread_e(&ZEarlyTestTableOffset, 4, 1, fp);
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
		RETURN_FALSE_IF_NULL(current);

		current->Name = stringTable[i]; //Names ignore the Remap Table



		//Create this early since we know there will only ever be 1
		current->AlphaTest = calloc(1, sizeof(struct AlphaTest));
		RETURN_FALSE_IF_NULL(current->AlphaTest);

		//Create this early since we know there will only ever be 1
		current->BlendInfo = calloc(1, sizeof(struct Blend));
		RETURN_FALSE_IF_NULL(current->BlendInfo);



		fseek(fp, chunkStart + dataOffset + (RemapTable[i] * MATERIAL_ENTRY_SIZE), SEEK_SET);

		unsigned char LightChannelCount,
			TexGenCount,
			TevStageCount;

		fread_e(&current->MaterialMode, 1, 1, fp);

		RETURN_FALSE_IF_FALSE(readFromTable(&current->Culling, 1, 4, fp, chunkStart, CullModeInfoOffset));
		RETURN_FALSE_IF_FALSE(readFromTable(&LightChannelCount, 1, 1, fp, chunkStart, ColorChannelCountTableOffset));
		//_ASSERT(LightChannelCount <= 0x02);
		RETURN_FALSE_IF_FALSE(readFromTable(&TexGenCount, 1, 1, fp, chunkStart, TexGenCountTable));
		RETURN_FALSE_IF_FALSE(readFromTable(&TevStageCount, 1, 1, fp, chunkStart, TevStageCountTableOffset));
		RETURN_FALSE_IF_FALSE(readFromTable(&current->BlendInfo->TestDepthBeforeTexture, 1, 1, fp, chunkStart, ZEarlyTestTableOffset));
		RETURN_FALSE_IF_FALSE(readFromTableWithFunc(current->BlendInfo, 1, 4, &readZCompare, fp, chunkStart, ZCompareTableOffset)); //DO NOT USE & FOR BLENDINFO!
		RETURN_FALSE_IF_FALSE(readFromTable(&current->EnableDithering, 1, 1, fp, chunkStart, DitherInfoOffset));

		for (size_t i = 0; i < 2; i++)
		{
			RETURN_FALSE_IF_FALSE(readFromTableOrDefault(&current->ColorMatRegs[i], 2, 4, fp, chunkStart, MaterialColorTableOffset, &COLOR_DEFAULT, 4));
		}

		for (size_t i = 0; i < LightChannelCount; i++)
		{
			struct LightChannelControl* LCC = calloc(1, sizeof(struct LightChannelControl));
			RETURN_FALSE_IF_NULL(LCC);
			struct ChannelControl* Color = calloc(1, sizeof(struct ChannelControl));
			RETURN_FALSE_IF_NULL(Color);
			struct ChannelControl* Alpha = calloc(1, sizeof(struct ChannelControl));
			RETURN_FALSE_IF_NULL(Alpha);

			RETURN_FALSE_IF_FALSE(readFromTableWithFunc(Color, 2, sizeof(struct ChannelControl), &readChannelControl, fp, chunkStart, ColorChannelTableOffset));
			RETURN_FALSE_IF_FALSE(readFromTableWithFunc(Alpha, 2, sizeof(struct ChannelControl), &readChannelControl, fp, chunkStart, ColorChannelTableOffset));

			LCC->Color = Color;
			LCC->Alpha = Alpha;
			current->LightChannels[i] = LCC;
		}

		for (size_t i = 0; i < 2; i++)
		{
			RETURN_FALSE_IF_FALSE(readFromTableOrDefault(&current->ColorAmbRegs[i], 2, 4, fp, chunkStart, AmbientColorTableOffset, &COLOR_DEFAULT, 4));
		}

		for (size_t i = 0; i < 8; i++)
		{
			if (!isTableIndexNULL(2, fp, chunkStart, LightInfoOffset))
			{
				struct Light* curlight = calloc(1, sizeof(struct Light));
				RETURN_FALSE_IF_FALSE(readFromTableWithFunc(curlight, 2, sizeof(struct Light), &readLight, fp, chunkStart, LightInfoOffset));
				current->Lights[i] = curlight;
			}
			else
			{
				fseek(fp, 2, SEEK_CUR); //skip the index 'cause it should be 0xFFFF!
				current->Lights[i] = NULL; //They should be NULL by default but just in case...
			}
		}


		//TexGen
		for (size_t i = 0; i < 8; i++)
		{
			if (i >= TexGenCount)
			{
				fseek(fp, 2, SEEK_CUR); //skip the index 'cause it should be 0xFFFF!
				current->TextureGenerators[i] = NULL;
				continue;
			}

			//Don't bother checking if the index is NULL since it should not be
			unsigned short idx = 0xFFFF;
			fread_e(&idx, 2, 1, fp);

			long pausePosition = ftell(fp);
			fseek(fp, chunkStart + TexGenTableOffset + idx * 4, SEEK_SET);

			struct TextureGenerator* texgen = calloc(1, sizeof(struct TextureGenerator));
			RETURN_FALSE_IF_NULL(texgen);

			fread_e(&texgen->Type, 1, 1, fp);
			fread_e(&texgen->Source, 1, 1, fp);
			fread_e(&texgen->Matrix, 1, 1, fp);
			//1 byte padding

			current->TextureGenerators[i] = texgen;
			fseek(fp, pausePosition, SEEK_SET);
		}
		//PostTexGen
		for (size_t i = 0; i < 8; i++)
		{
			current->PostTextureGenerators[i] = NULL; //So common that it's just always going to happen
			if (i >= TexGenCount)
			{
				fseek(fp, 2, SEEK_SET); //skip the index 'cause it should be 0xFFFF!
				continue;
			}

			//Don't bother checking if the index is NULL since it should not be
			unsigned short idx = 0xFFFF;
			fread_e(&idx, 2, 1, fp);

			if (idx == 0xFFFF)
				continue;

			long pausePosition = ftell(fp);
			fseek(fp, chunkStart + PostTexGenTableOffset + idx * 4, SEEK_SET);

			struct TextureGenerator* texgen = calloc(1, sizeof(struct TextureGenerator));
			RETURN_FALSE_IF_NULL(texgen);

			fread_e(&texgen->Type, 1, 1, fp);
			fread_e(&texgen->Source, 1, 1, fp);
			fread_e(&texgen->Matrix, 1, 1, fp);
			//1 byte padding

			current->PostTextureGenerators[i] = texgen;
			fseek(fp, pausePosition, SEEK_SET);
		}
	
		//Continue with Texture Matrix and Post Texture Matrix reading
	}

	return true;
}

//Index read from the file cannot end up as NULL (-1)
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

	fread_e(_Buffer, ElementSize, 1, _Stream);

	fseek(_Stream, pausePosition, SEEK_SET);
	return true;
}

//Index read from the file can end up as NULL (-1)
bool readFromTableOrDefault(void* _Buffer, size_t IndexSize, size_t ElementSize, FILE* _Stream, long ChunkStart, long TableOffset, void* _Default, size_t DefaultSize)
{
	if (isTableIndexNULL(IndexSize, _Stream, ChunkStart, TableOffset))
	{
		memcpy(_Buffer, _Default, DefaultSize);
		fseek(_Stream, IndexSize, SEEK_CUR);
		return true;
	}

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

	fread_e(_Buffer, ElementSize, 1, _Stream);

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

	RETURN_FALSE_IF_FALSE(func_ptr(_Buffer, _Stream));

	fseek(_Stream, pausePosition, SEEK_SET);
	return true;
}

bool isTableIndexNULL(size_t IndexSize, FILE* _Stream, long ChunkStart, long TableOffset)
{
	long pausePosition = ftell(_Stream);

	void* Index = calloc(1, IndexSize);
	RETURN_FALSE_IF_NULL(Index);

	fread_e(Index, IndexSize, 1, _Stream);

	fseek(_Stream, pausePosition, SEEK_SET);

	if (IndexSize == 1)
	{
		if ((*(unsigned char*)Index) == 0xFF)
			return true;
	}
	else if (IndexSize == 2)
	{
		if ((*(unsigned short*)Index) == 0xFFFF)
			return true;
	}
	else if (IndexSize == 4)
	{
		if ((*(unsigned int*)Index) == 0xFFFFFFFF)
			return true;
	}
	return false;
}

//===============================================

bool readZCompare(void* _Buffer, FILE* fp)
{
	RETURN_FALSE_IF_NULL(_Buffer);

	struct Blend* ptr = (struct Blend*)_Buffer;

	fread_e(&ptr->EnableDepthTest, 1, 1, fp);
	fread_e(&ptr->DepthFunction, 1, 1, fp);
	fread_e(&ptr->WriteToZBuffer, 1, 1, fp);	

	return true;
}

bool readChannelControl(void* _Buffer,FILE* fp)
{
	RETURN_FALSE_IF_NULL(_Buffer);

	struct ChannelControl* ptr = (struct ChannelControl*)_Buffer;

	fread_e(&ptr->LightingEnabled, 1, 1, fp);
	fread_e(&ptr->MaterialColorSource, 1, 1, fp);
	fread_e(&ptr->LightMask, 1, 1, fp);
	fread_e(&ptr->DiffuseFunc, 1, 1, fp);
	fread_e(&ptr->AttenuationFunc, 1, 1, fp);
	fread_e(&ptr->AmbientColorSource, 1, 1, fp);

	return true;
}

bool readLight(void* _Buffer, FILE* fp)
{
	RETURN_FALSE_IF_NULL(_Buffer);

	struct Light* ptr = (struct Light*)_Buffer;

	fread_e(&ptr->Position, 4, 3, fp);
	fread_e(&ptr->Direction, 4, 3, fp);
	fread_e(&ptr->Color, 1, 4, fp);
	fread_e(&ptr->A0, 4, 1, fp);
	fread_e(&ptr->A1, 4, 1, fp);
	fread_e(&ptr->A2, 4, 1, fp);
	fread_e(&ptr->K0, 4, 1, fp);
	fread_e(&ptr->K1, 4, 1, fp);
	fread_e(&ptr->K2, 4, 1, fp);

	return true;
}

//===============================================

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
		           sizeof (struct ChannelControl)))
			return false;

		if (memcmp(mat1->LightChannels[i]->Alpha, mat2->LightChannels[i]->Alpha,
		           sizeof (struct ChannelControl)))
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
