#include <stdbool.h>
#include <stdio.h>

#include "tex1.h"

bool readTEX1(FILE* fp, struct JUTTexture** outputArray, unsigned int* elementCount)
{
	long chunkStart = ftell(fp);
	if (!isMagicMatch(fp, MAGIC_TEX1))
		return false;

	fseek(fp, 4, SEEK_CUR); //Skip Chunk Size
	unsigned short texCount;
	fread(&texCount, 2, 1, fp); //Read texture count
	*elementCount = texCount; //Set element count output
	fseek(fp, 2, SEEK_CUR); //Skip padding

	unsigned int dataOffset, stringTableOffset;
	fread(&dataOffset, 4, 1, fp); //Offset to BTI headers
	fread(&stringTableOffset, 4, 1, fp); //Offset to names

	//read the string table
	fseek(fp, chunkStart + stringTableOffset, SEEK_SET);
	char** stringTable = readStringTable(fp);

	outputArray = calloc((texCount + 1), sizeof(struct JUTTexture*));
	if (outputArray == NULL)
		return false;

	for (int i = 0; i < texCount; i++)
	{
		struct JUTTexture* currenttex = calloc(1, sizeof(struct JUTTexture));
		if (currenttex == NULL)
			return false;
		currenttex->Name = stringTable[i];
		fseek(fp, chunkStart + dataOffset + (i * 0x20), SEEK_SET);
		if (!readJUTTexture(fp, currenttex))
			return false;
		outputArray[i] = currenttex;
	}
	free(stringTable);
}

bool readJUTTexture(FILE* fp, struct JUTTexture* output)
{
	unsigned int ImageDataPos, PaletteDataPos;
	long startPosition = ftell(fp);
	fread(&(output->Format), 1, 1, fp);
	fread(&(output->AlphaSetting), 1, 1, fp);
	readReverseUint16(fp, &output->Width);
	readReverseUint16(fp, &output->Height);
	fread(&(output->WrapS), 1, 1, fp);
	fread(&(output->WrapT), 1, 1, fp);
	bool UsePalettes;
	fread(&UsePalettes, 1, 1, fp);
	fread(&(output->PaletteFormat), 1, 1, fp);
	readReverseUint16(fp, &output->PaletteDataSize);
	readReverseUint32(fp, &PaletteDataPos);
	fread(&output->EnableMipmaps, 1, 1, fp);
	fread(&output->EnableEdgeLOD, 1, 1, fp);
	fread(&output->ClampLODBias, 1, 1, fp);
	fread(&output->MaxAnisotropy, 1, 1, fp);
	fread(&output->MinificationFilter, 1, 1, fp);
	fread(&output->MagnificationFilter, 1, 1, fp);
	unsigned char minlod, maxlod, lodbias;
	fread(&minlod, 1, 1, fp);
	fread(&maxlod, 1, 1, fp);
	output->MinLOD = minlod / 8.f;
	output->MaxLOD = maxlod / 8.f;
	fread(&output->ImageCount, 1, 1, fp);
	if (output->ImageCount == 0)
		output->ImageCount = 1;
	fseek(fp, 1, SEEK_CUR); //skip reserved
	readReverseUint16(fp, &lodbias);
	output->LODBias = lodbias / 100.f;
	readReverseUint32(fp, &ImageDataPos);

	if (UsePalettes)
	{
		//each colour is a short, so 2 bytes each! This is because the formats that palettes support are ALL 2 bytes big. certified bruh moment
		fseek(fp, startPosition + PaletteDataPos, SEEK_SET);
		output->PaletteData = calloc(output->PaletteDataSize, 2);
		fread(&output->PaletteData, 2, output->PaletteDataSize, fp);
	}

	fseek(fp, startPosition + ImageDataPos, SEEK_SET);
	int imageDataSize = calcImageSize(output->Format, output->Width, output->Height, output->ImageCount);
	output->ImageData = calloc(imageDataSize, 1);
	int read = fread(output->ImageData, sizeof(char), imageDataSize, fp);

	return true;
}

int calcImageSize(enum ImageFormats Format, unsigned short width, unsigned short height, unsigned int MipCount)
{
	int blockSize = 0, BlockWidth = 0, BlockHeight = 0;

	switch (Format)
	{
	default:
		blockSize = 32;
		break;
	case RGBA32:
		blockSize = 64;
		break;
	}

	switch (Format)
	{
	case IA8:
	case RGB565:
	case RGB5A3:
	case RGBA32:
	case C14X2:
		BlockWidth = 4;
		break;

	default:
		BlockWidth = 8;
		break;
	}

	switch (Format)
	{
	default:
		BlockHeight = 4;
		break;

	case I4:
	case I8:
	case C4:
	case CMPR:
		BlockHeight = 8;
		break;
	}


	int DataSize = 0;
	unsigned short Width = width, Height = height;

	for (unsigned int i = 0; i < MipCount; i++)
	{
		float f1 = ((float)width + (float)(width % BlockWidth)) / (float)BlockWidth;
		float f2 = ((float)height + (float)(height % BlockHeight)) / (float)BlockHeight;
		float BlockCountX = (int)f1;
		float BlockCountY = (int)f2;
		int ByteCount = (BlockCountX * (BlockWidth * BlockHeight)) * BlockCountY;
		if (ByteCount == 0) //There *must* be at least one block per image
			ByteCount = BlockWidth * BlockHeight;

		switch (Format)
		{
		case I4:
		case C4:
			ByteCount /= 2;
			break;
		case I8:
		case IA4:
		case C8:
			break; //ByteCount = ByteCount; bruh;
		case IA8:
		case RGB565:
		case RGB5A3:
		case C14X2:
			ByteCount *= 2;
			break;
		case RGBA32:
			ByteCount *= 4;
			break;
		case CMPR:
			ByteCount = ByteCount * 4 / 8;
			break;
		}
		DataSize += ByteCount;

		Width /= 2;
		Height /= 2;
	}
	return DataSize;
}


bool texcmp(struct JUTTexture* tex1, struct JUTTexture* tex2) {
	if (tex1 == tex2)
		return true;

	if (strcmp(tex1->Name, tex2->Name))
		return false;

	if (tex1->Width != tex2->Width)
		return false;

	if (tex1->Height != tex2->Height)
		return false;

	if (tex1->Format != tex2->Format)
		return false;

	if (tex1->PaletteFormat != tex2->PaletteFormat)
		return false;

	if (tex1->ImageCount != tex2->ImageCount)
		return false;

	if (tex1->WrapS != tex2->WrapS)
		return false;

	if (tex1->WrapT != tex2->WrapT)
		return false;

	if (tex1->AlphaSetting != tex2->AlphaSetting)
		return false;

	if (tex1->ClampLODBias != tex2->ClampLODBias)
		return false;

	if (tex1->MaxAnisotropy != tex2->MaxAnisotropy)
		return false;

	if (tex1->MagnificationFilter != tex2->MagnificationFilter)
		return false;

	if (tex1->MinificationFilter != tex2->MinificationFilter)
		return false;

	if (tex1->EnableMipmaps != tex2->EnableMipmaps)
		return false;

	if (tex1->MinLOD != tex2->MinLOD)
		return false;

	if (tex1->MaxLOD != tex2->MaxLOD)
		return false;

	if (tex1->LODBias != tex2->LODBias)
		return false;

	if (tex1->EnableEdgeLOD != tex2->EnableEdgeLOD)
		return false;

	if (tex1->ImageDataSize != tex2->ImageDataSize ||
	    memcmp(tex1->ImageData, tex2->ImageData, tex1->ImageDataSize))
		return false;

	if (tex1->PaletteDataSize != tex2->PaletteDataSize ||
	    memcmp(tex1->PaletteData, tex2->PaletteData, tex1->PaletteDataSize))
		return false;

	return true;
}
