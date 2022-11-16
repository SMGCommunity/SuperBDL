#ifndef __TEX1_H
#define __TEX1_H

#include <stdbool.h>
#include <bmd.h>
#include <mat3.h>

#define MAGIC_TEX1 "TEX1"

bool writeTEX1(const struct aiScene *data);

enum JUTTransparency
{
	/// <summary>
	///No Transperancy
	/// </summary>
	OPAQUE = 0x00,
	/// <summary>
	/// Only allows fully Transperant pixels to be see through
	/// </summary>
	CUTOUT = 0x01,
	/// <summary>
	/// Allows Partial Transperancy. Also known as XLUCENT
	/// </summary>
	TRANSLUCENT = 0x02,
};

struct JUTTexture
{
	char* Name;
	enum JUTTransparency AlphaSetting = OPAQUE;
	bool ClampLODBias = true, EnableMipmaps = true;
	unsigned char MaxAnisotropy = 0;

	//GXTexture
	enum GXTexFormat Format;
	enum GXTexPalette PaletteFormat;
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
	void* PaletteData; //Palette Data. Optional.
};

#endif /* __TEX1_H */