#ifndef __TEX1_H
#define __TEX1_H

#include <stdbool.h>
#include <stdio.h>
#include "bmd.h"
#include "mat3.h"

#define MAGIC_TEX1 "TEX1"

enum JUTTransparency
{
	/// <summary>
	///No Transperancy
	/// </summary>
	JUT_OPAQUE = 0x00,
	/// <summary>
	/// Only allows fully Transperant pixels to be see through
	/// </summary>
	JUT_CUTOUT = 0x01,
	/// <summary>
	/// Allows Partial Transperancy. Also known as XLUCENT
	/// </summary>
	JUT_TRANSLUCENT = 0x02,
};

enum GXWrapMode
{
	CLAMP = 0x00,
	REPEAT = 0x01,
	MIRROR = 0x02
};

enum GXTexFilter
{
	NEAR = 0,
	LINEAR = 1,
	//Minification only from here on out
	NEAR_MIP_NEAR = 2,
	LIN_MIP_NEAR = 3,
	NEAR_MIP_LIN = 4,
	LIN_MIP_LIN = 5
};

enum GXImageFormats {
	I4 = 0x00,
	I8 = 0x01,
	IA4 = 0x02,
	IA8 = 0x03,
	RGB565 = 0x04,
	RGB5A3 = 0x05,
	RGBA32 = 0x06,
	C4 = 0x08,
	C8 = 0x09,
	C14X2 = 0x0A,
	CMPR = 0x0E
};

enum GXPaletteFormats
{
	P_IA8 = 0x00,
	P_RGB565 = 0x01,
	P_RGB5A3 = 0x02
};

struct JUTTexture
{
	char* Name;
	enum JUTTransparency AlphaSetting ;
	bool ClampLODBias;
	bool EnableMipmaps;
	unsigned char MaxAnisotropy;

	//GXTexture
	enum GXImageFormats Format;
	enum GXPaletteFormats PaletteFormat;
	enum GXWrapMode WrapS;
	enum GXWrapMode WrapT;
	enum GXTexFilter MagnificationFilter;
	enum GXTexFilter MinificationFilter;
	float MinLOD;
	float MaxLOD;
	float LODBias;
	bool EnableEdgeLOD;

	unsigned short Width, Height; //Every wii format uses 16 bit values for these.
	unsigned char ImageCount;
	void* ImageData; //Image Data. Includes mipmaps
	unsigned int ImageDataSize;
	void* PaletteData; //Palette Data. Optional.
	/// <summary>
	/// The number of colours in the palette. Multiply this by 2 to get the amount of RAM inside PaletteData
	/// </summary>
	unsigned int PaletteDataSize;
};

bool readTEX1(FILE* fp, struct JUTTexture*** outputArray, unsigned int* elementCount);
bool readJUTTexture(FILE* fp, struct JUTTexture* output);
int calcImageSize(enum GXImageFormats Format, unsigned short width, unsigned short height, unsigned int MipCount);
bool writeTEX1(const struct aiScene *data);

bool texcmp(struct JUTTexture* tex1, struct JUTTexture* tex2);

#endif /* __TEX1_H */
