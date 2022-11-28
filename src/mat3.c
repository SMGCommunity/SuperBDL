#include <stdbool.h>
#include <stdio.h>

#include "superbdl.h"
#include "mat3.h"
#include "tex1.h"

bool readMAT3(FILE* fp, struct J3DMaterial*** outputArray, unsigned int* elementCount, struct JUTTexture*** textureArray)
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
		TextureIDTableOffset,
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
	fread_e(&TextureIDTableOffset, 4, 1, fp);
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
	
	*outputArray = calloc((size_t)(matCount + 1), sizeof(struct J3DMaterial*));
	if (*outputArray == NULL)
		return false;

	//Now to read the materials themselves
	for (size_t mid = 0; mid < matCount; mid++)
	{
		struct J3DMaterial* current = calloc(1, sizeof(struct J3DMaterial));
		RETURN_FALSE_IF_NULL(current);

		current->Name = stringTable[mid]; //Names ignore the Remap Table



		//Create this early since we know there will only ever be 1
		current->AlphaTest = calloc(1, sizeof(struct AlphaTest));
		RETURN_FALSE_IF_NULL(current->AlphaTest);

		//Create this early since we know there will only ever be 1
		current->BlendInfo = calloc(1, sizeof(struct Blend));
		RETURN_FALSE_IF_NULL(current->BlendInfo);

		//Create this early since we know there will only ever be 1
		current->FogInfo = calloc(1, sizeof(struct Fog));
		RETURN_FALSE_IF_NULL(current->FogInfo);

		//Create this early since we know there will only ever be 1
		current->NBTScale = calloc(1, sizeof(struct NBT));
		RETURN_FALSE_IF_NULL(current->NBTScale);


		fseek(fp, chunkStart + dataOffset + (RemapTable[mid] * MATERIAL_ENTRY_SIZE), SEEK_SET);

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

			RETURN_FALSE_IF_FALSE(readFromTableWithFunc(Color, 2, 8, &readChannelControl, fp, chunkStart, ColorChannelTableOffset));
			RETURN_FALSE_IF_FALSE(readFromTableWithFunc(Alpha, 2, 8, &readChannelControl, fp, chunkStart, ColorChannelTableOffset));

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
			if (!isTableIndexNULL(2, fp))
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
				fseek(fp, 2, SEEK_CUR); //skip the index 'cause it should be 0xFFFF!
				continue;
			}

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
	
		long TMP = ftell(fp);
		//Continue with Texture Matrix and Post Texture Matrix reading
		//TexMtx
		if (TexMtxTableOffset != 0)
			for (size_t i = 0; i < 10; i++)
			{
				unsigned short idx = 0xFFFF;
				fread_e(&idx, 2, 1, fp);

				if (idx == 0xFFFF)
					continue;

				long pausePosition = ftell(fp);
				fseek(fp, chunkStart + TexMtxTableOffset + idx * 0x64, SEEK_SET);

				struct TextureMatrix* ptr = calloc(1, sizeof(struct TextureMatrix));
				RETURN_FALSE_IF_NULL(ptr);

				unsigned char Info;
				short rot;
				fread_e(&ptr->MatrixType, 1, 1, fp);
				fread_e(&Info, 1, 1, fp);
				ptr->Mode = (Info & 0x3F);
				ptr->IsMaya = (Info >> 7 == 1);
				fseek(fp, 2, SEEK_CUR); //Padding
				fread_e(&ptr->Center, 4, 3, fp);
				fread_e(&ptr->Scale, 4, 2, fp);
				fread_e(&rot, 2, 1, fp);
				ptr->Rotation = ((float)rot / 0x7FFF);
				fseek(fp, 2, SEEK_CUR); //Padding
				fread_e(&ptr->Translation, 4, 2, fp);
				fread_e(&ptr->ProjectionMatrix, 4, 16, fp);

				current->TextureMatricies[i] = ptr;
				fseek(fp, pausePosition, SEEK_SET);
			}
		else
			fseek(fp, 20, SEEK_CUR); //Skip all the 0xFFFF
		//PostTexMtx
		if (PostTexMtxTableOffset != 0)
			for (size_t i = 0; i < 20; i++)
			{
				unsigned short idx = 0xFFFF;
				fread_e(&idx, 2, 1, fp);

				if (idx == 0xFFFF)
					continue;

				long pausePosition = ftell(fp);
				fseek(fp, chunkStart + PostTexMtxTableOffset + idx * 0x64, SEEK_SET);

				struct TextureMatrix* ptr = calloc(1, sizeof(struct TextureMatrix));
				RETURN_FALSE_IF_NULL(ptr);

				unsigned char Info;
				short rot;
				fread_e(&ptr->MatrixType, 1, 1, fp);
				fread_e(&Info, 1, 1, fp);
				ptr->Mode = (Info & 0x3F);
				ptr->IsMaya = (Info >> 7 == 1);
				fseek(fp, 2, SEEK_CUR); //Padding
				fread_e(&ptr->Center, 4, 3, fp);
				fread_e(&ptr->Scale, 4, 2, fp);
				fread_e(&rot, 2, 1, fp);
				ptr->Rotation = ((float)rot / 0x7FFF);
				fseek(fp, 2, SEEK_CUR); //Padding
				fread_e(&ptr->Translation, 4, 2, fp);
				fread_e(&ptr->ProjectionMatrix, 4, 16, fp);

				current->PostTextureMatricies[i] = ptr;
				fseek(fp, pausePosition, SEEK_SET);
			}
		else
			fseek(fp, 40, SEEK_CUR); //Skip all the 0xFFFF

		//Texture time!!
		for (size_t i = 0; i < 8; i++)
		{
			current->Textures[i] = NULL; //so common we always do it

			if (isTableIndexNULL(2, fp))
			{
				fseek(fp, 2, SEEK_CUR);
				continue;
			}

			unsigned short idx = 0;

			RETURN_FALSE_IF_FALSE(readFromTable(&idx, 2, 2, fp, chunkStart, TextureIDTableOffset));
			current->Textures[i] = *textureArray[idx];
		}

		//TEV CONSTANT Colors (0-255)
		for (size_t i = 0; i < 4; i++)
		{
			RETURN_FALSE_IF_FALSE(readFromTableOrDefault(&current->ColorConstants[i], 2, 4, fp, chunkStart, ColorConstantTableOffset, &COLOR_DEFAULT, 4));
		}

		//Need to do it this way because TEV Stages should be built all at once
		unsigned char TevConstColSel[16] = { 0 }, TevConstAlpSel[16] = { 0 };
		unsigned short TevOrderIndicies[16] = { 0 }, TevStageIndicies[16] = { 0 }, TevSwapModeSelectionIndicies[16] = { 0 }, UNKNOWN[12] = { 0 };
		fread_e(&TevConstColSel[0], 1, 16, fp);
		fread_e(&TevConstAlpSel[0], 1, 16, fp);
		fread_e(&TevOrderIndicies[0], 2, 16, fp);

		//TEV REGISTER Colors (-short to +short)
		for (size_t i = 0; i < 4; i++)
		{
			if (isTableIndexNULL(2, fp))
			{
				memcpy(&current->ColorRegisters[i], &COLOR_REGISTER_DEFAULT, 8);
				fseek(fp, 2, SEEK_CUR);
				return true;
			}

			unsigned short Index;

			fread_e(&Index, 2, 1, fp);
			long pausePosition = ftell(fp);

				fseek(fp, chunkStart + ColorRegisterTableOffset + Index * 8, SEEK_SET);

			fread_e(&current->ColorRegisters[i], 2, 4, fp);

			fseek(fp, pausePosition, SEEK_SET);
		}
		fread_e(&TevStageIndicies[0], 2, 16, fp);
		fread_e(&TevSwapModeSelectionIndicies[0], 2, 16, fp);

		//Swap Tables
		for (size_t i = 0; i < 4; i++)
		{
			if (isTableIndexNULL(2, fp))
			{
				memcpy(&current->SwapTables[i], &DEFAULT_SWAPTABLE, 4);
				fseek(fp, 2, SEEK_CUR);
				return true;
			}

			unsigned short idx;
			fread_e(&idx, 2, 1, fp);
			long pausePosition = ftell(fp);
			fseek(fp, chunkStart + TevSwapModeTableOffset + idx * 4, SEEK_SET);
			fread_e(&current->SwapTables[i], 1, 4, fp);
			fseek(fp, pausePosition, SEEK_SET);
		}

		//No clue what this is for...
		fread_e(&UNKNOWN[0], 2, 12, fp);
		long materialfinishpos = ftell(fp);

		//Time to make the Indirect TEV Stages
		long IndTexEntryBase = chunkStart + IndTexInfoOffset + (long)(mid * 0x138);
		fseek(fp, IndTexEntryBase, SEEK_SET);
		bool HasIndirect = false;
		if (IndTexInfoOffset != stringTableOffset) //I guess if these are equal, then there is no indirect information...
			fread_e(&HasIndirect, 1, 1, fp);

		if (HasIndirect)
		{
			unsigned char StageNum;
			fread_e(&StageNum, 1, 1, fp);

			for (size_t i = 0; i < 3; i++)
			{
				//IndTexMtx
				fseek(fp, (size_t)IndTexEntryBase + 0x04 + (size_t)(0x04 * 4) + i * 0x1C, SEEK_SET);
				union Matrix2x3f matrix;
				fread(&matrix, 4, 6, fp);
				unsigned char scalechar;
				fread(&scalechar, 1, 1, fp);
				float Scale = powf(2, scalechar);

				current->IndirectTextureMatricies[i].m00 = matrix.m00 * Scale;
				current->IndirectTextureMatricies[i].m01 = matrix.m01 * Scale;
				current->IndirectTextureMatricies[i].m02 = matrix.m02 * Scale;
				current->IndirectTextureMatricies[i].m03 = Scale;
				current->IndirectTextureMatricies[i].m10 = matrix.m10 * Scale;
				current->IndirectTextureMatricies[i].m11 = matrix.m11 * Scale;
				current->IndirectTextureMatricies[i].m12 = matrix.m12 * Scale;
				current->IndirectTextureMatricies[i].m13 = 0.f;
			}

			for (size_t i = 0; i < StageNum; i++)
			{
				struct IndirectTextureStage* ptr = calloc(1, sizeof(struct IndirectTextureStage));

				//IndTexOrder
				fseek(fp, (size_t)IndTexEntryBase + 0x04 + (i * 0x04), SEEK_SET);
				fread_e(&ptr->TexCoordID, 1, 1, fp);
				fread_e(&ptr->TexMapID, 1, 1, fp);

				//IndTexScale
				fseek(fp, (size_t)IndTexEntryBase + 0x04 + (size_t)(0x04 * 4) + (size_t)(0x1C * 3) + i * 0x04, SEEK_SET);
				fread_e(&ptr->ScaleS, 1, 1, fp);
				fread_e(&ptr->ScaleT, 1, 1, fp);

				current->IndirectStages[i] = ptr;
			}
		}

		//Time to create the TEV Stages.
		for (size_t i = 0; i < TevStageCount; i++)
		{
			struct TextureEnvironmentStage* tev = calloc(1, sizeof(TEVStage));

			RETURN_FALSE_IF_NULL(tev);

			fseek(fp, chunkStart + TevStageTableOffset + (TevStageIndicies[i] * 0x14), SEEK_SET);
			unsigned char UNK0;
			fread_e(&UNK0, 1, 1, fp); //should always just be 0xFF?

			fread_e(&tev->ColorInA, 1, 1, fp);
			fread_e(&tev->ColorInB, 1, 1, fp);
			fread_e(&tev->ColorInC, 1, 1, fp);
			fread_e(&tev->ColorInD, 1, 1, fp);
			fread_e(&tev->ColorOperation, 1, 1, fp);
			fread_e(&tev->ColorBias, 1, 1, fp);
			fread_e(&tev->ColorScale, 1, 1, fp);
			fread_e(&tev->ColorClamp, 1, 1, fp);
			fread_e(&tev->ColorOutRegister, 1, 1, fp);

			fread_e(&tev->AlphaInA, 1, 1, fp);
			fread_e(&tev->AlphaInB, 1, 1, fp);
			fread_e(&tev->AlphaInC, 1, 1, fp);
			fread_e(&tev->AlphaInD, 1, 1, fp);
			fread_e(&tev->AlphaOperation, 1, 1, fp);
			fread_e(&tev->AlphaBias, 1, 1, fp);
			fread_e(&tev->AlphaScale, 1, 1, fp);
			fread_e(&tev->AlphaClamp, 1, 1, fp);
			fread_e(&tev->AlphaOutRegister, 1, 1, fp);

			//TEV Orders
			fseek(fp, chunkStart + TevOrderTableOffset + (TevOrderIndicies[i] * 0x04), SEEK_SET);
			fread_e(&tev->TextureCoordID, 1, 1, fp);
			fread_e(&tev->TextureMapID, 1, 1, fp);
			fread_e(&tev->ChannelID, 1, 1, fp);

			tev->ConstantColorSelection = TevConstColSel[i];
			tev->ConstantAlphaSelection = TevConstAlpSel[i];

			//Swap Tables
			fseek(fp, chunkStart + TevSwapModeInfoOffset + TevSwapModeSelectionIndicies[i] * 0x04, SEEK_SET);
			unsigned char RasSwapSel, TexSwapSel;
			fread(&RasSwapSel, 1, 1, fp);
			fread(&TexSwapSel, 1, 1, fp);
			tev->RasterizerSwapTable = &current->SwapTables[RasSwapSel];
			tev->TextureSwapTable = &current->SwapTables[TexSwapSel];

			//TevIndirect
			fseek(fp, chunkStart + IndTexEntryBase + 0x04 + (0x04 * 4) + (0x1C * 3) + (0x04 * 4) + i * 0x0C, SEEK_SET);
			if (HasIndirect)
			{
				unsigned char IndStageID;
				fread(&IndStageID, 1, 1, fp);
				RETURN_FALSE_IF_NULL(current->IndirectStages[IndStageID]);
				tev->IndirectStagePtr = &current->IndirectStages[IndStageID];
				fread(&tev->IndirectTexFormat, 1, 1, fp);
				fread(&tev->IndirectTexBias, 1, 1, fp);
				fread(&tev->IndirectTexMtxID, 1, 1, fp);
				fread(&tev->IndirectTexWrapS, 1, 1, fp);
				fread(&tev->IndirectTexWrapT, 1, 1, fp);
				fread(&tev->IndirectAddPrevious, 1, 1, fp);
				fread(&tev->IndirectUseOriginalLoD, 1, 1, fp);
				fread(&tev->IndirectTexAlpha, 1, 1, fp);
			}

			current->TEVStages[i] = tev; //Tev Stages must exist in order so this is fine
		}

		fseek(fp, materialfinishpos, SEEK_SET);

		RETURN_FALSE_IF_FALSE(readFromTableWithFunc(current->FogInfo, 2, 0x2C, &readFog, fp, chunkStart, FogInfoTableOffset));
		RETURN_FALSE_IF_FALSE(readFromTableWithFunc(current->AlphaTest, 2, 0x08, &readAlphaCompare, fp, chunkStart, AlphaTestTableOffset));
		RETURN_FALSE_IF_FALSE(readFromTableWithFunc(current->BlendInfo, 2, 0x04, &readBlend, fp, chunkStart, BlendModeTableOffset));
		RETURN_FALSE_IF_FALSE(readFromTableWithFunc(current->NBTScale, 2, 0x10, &readNBTScale, fp, chunkStart, NBTScaleOffset));

		*outputArray[mid] = current;
	}

	return true;
}

//Index read from the file cannot end up as NULL (-1)
bool readFromTable(void* _Buffer, size_t IndexSize, size_t ElementSize, FILE* _Stream, long ChunkStart, long TableOffset)
{
	if (isTableIndexNULL(IndexSize, _Stream))
	{
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

//Index read from the file can end up as NULL (-1)
bool readFromTableOrDefault(void* _Buffer, size_t IndexSize, size_t ElementSize, FILE* _Stream, long ChunkStart, long TableOffset, const void* _Default, size_t DefaultSize)
{
	if (isTableIndexNULL(IndexSize, _Stream))
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

bool isTableIndexNULL(size_t IndexSize, FILE* _Stream)
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

bool readFog(void* _Buffer, FILE* fp)
{
	RETURN_FALSE_IF_NULL(_Buffer);

	struct Fog* ptr = (struct Fog*)_Buffer;

	fread_e(&ptr->Type, 1, 1, fp);
	fread_e(&ptr->AdjustEnabled, 1, 1, fp);
	fread_e(&ptr->AdjustCenter, 2, 1, fp);
	fread_e(&ptr->StartZ, 4, 1, fp);
	fread_e(&ptr->EndZ, 4, 1, fp);
	fread_e(&ptr->NearZ, 4, 1, fp);
	fread_e(&ptr->FarZ, 4, 1, fp);
	fread_e(&ptr->StartZ, 4, 1, fp);
	fread_e(&ptr->Color, 1, 4, fp);
	fread_e(&ptr->AdjustTable[0], 2, 10, fp);

	return true;
}

bool readAlphaCompare(void* _Buffer, FILE* fp)
{
	RETURN_FALSE_IF_NULL(_Buffer);

	struct AlphaTest* ptr = (struct AlphaTest*)_Buffer;

	fread_e(&ptr->CompareA, 1, 1, fp);
	fread_e(&ptr->ReferenceA, 1, 1, fp);
	fread_e(&ptr->Operation, 1, 1, fp);
	fread_e(&ptr->CompareB, 1, 1, fp);
	fread_e(&ptr->ReferenceB, 1, 1, fp);

	return true;
}

bool readBlend(void* _Buffer, FILE* fp)
{
	RETURN_FALSE_IF_NULL(_Buffer);

	struct Blend* ptr = (struct Blend*)_Buffer;

	fread_e(&ptr->BlendMode, 1, 1, fp);
	fread_e(&ptr->BlendSourceFactor, 1, 1, fp);
	fread_e(&ptr->BlendDestFactor, 1, 1, fp);
	fread_e(&ptr->BlendLogicOperation, 1, 1, fp);

	return true;
}

bool readNBTScale(void* _Buffer, FILE* fp)
{
	RETURN_FALSE_IF_NULL(_Buffer);

	struct NBT* ptr = (struct NBT*)_Buffer;

	fread_e(&ptr->UNKNOWN, 1, 1, fp);
	fseek(fp, 3, SEEK_CUR); //padding
	fread_e(&ptr->Scale, 4, 3, fp);

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
