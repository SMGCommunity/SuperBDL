#ifndef __BMD_H
#define __BMD_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <assimp/scene.h>
#include "endianio.h"

#define MAGIC_J3D "J3D2"
#define MAGIC_BDL "bdl4"
#define MAGIC_BMD "bmd3"

enum J3DImportOptimizations
{
	/// <summary>
	/// No Optimizations
	/// </summary>
	OPTIMIZE_NONE = 0b00000000000000000000000000000000,
	/// <summary>
	/// [PERFORMANCE &amp; FILESIZE]<para/>Optimize the geometry into Triangle Strips/Triangle Fans.
	/// </summary>
	OPTIMIZE_TRIANGLES	= 0b00000000000000000000000000000001,
	/// <summary>
	/// [FILESIZE]<para/>Optimize the GX Attributes to use different data types (like short instead of float)
	/// </summary>
	OPTIMIZE_DATATYPES	= 0b00000000000000000000000000000010,
	/// <summary>
	/// [FILESIZE]<para/>Optimize remap tables (EXPERIMENTAL) Basically this means that if there are two identical sets of data we can store only 1 and use the Remap table to tell the game to use it twice... I THINK that's how that works...<para/> <para/>[JNT, SHP, MAT x2]
	/// </summary>
	OPTIMIZE_REMAP	= 0b00000000000000000000000000000100,
	/// <summary>
	/// [FILESIZE]<para/>Optimizes duplicate material data into storing one copy only. (in regards to the individual material blocks)
	/// </summary>
	OPTIMIZE_MATDATA	= 0b00000000000000000000000000001000,
	/// <summary>
	/// [PERFORMANCE]<para/>Optimize TEV Stages for rendering speed.
	/// </summary>
	OPTIMIZE_TEVSTAGE	= 0b00000000000000000000000000010000,
	/// <summary>
	/// [PERFORMANCE]<para/>Optimize the Alpha blending and comparison. Mostly for Opaque Materials.
	/// </summary>
	OPTIMIZE_BLENDING	= 0b00000000000000000000000000100000,
	/// <summary>
	/// [FILESIZE]<para/>Optimize the image data. If two textures are identical, they will be merged together (with separate headers, of course)
	/// </summary>
	OPTIMIZE_TEX_IMG	= 0b00000000000000000000000001000000,
	/// <summary>
	/// [FILESIZE]<para/>Optimize the palette data. If Two textures have the same palette data, they will be merged together (with separate headers, of course). This may be useless...
	/// </summary>
	OPTIMIZE_TEX_PAL	= 0b00000000000000000000000010000000,


	OPTIMIZE_GEOMETRY	= OPTIMIZE_TRIANGLES | OPTIMIZE_DATATYPES,
	OPTIMIZE_MATERIAL	= OPTIMIZE_MATDATA | OPTIMIZE_TEVSTAGE | OPTIMIZE_BLENDING,
	OPTIMIZE_TEXTURE	= OPTIMIZE_TEX_IMG | OPTIMIZE_TEX_PAL,

	OPTIMIZE_SIZE		= OPTIMIZE_GEOMETRY | OPTIMIZE_TEXTURE | OPTIMIZE_REMAP | OPTIMIZE_MATDATA,
	OPTIMIZE_SPEED		= OPTIMIZE_TRIANGLES | OPTIMIZE_TEVSTAGE | OPTIMIZE_BLENDING,
	OPTIMIZE_ALL		= OPTIMIZE_TRIANGLES | OPTIMIZE_DATATYPES | OPTIMIZE_MATDATA | OPTIMIZE_TEVSTAGE | OPTIMIZE_BLENDING | OPTIMIZE_TEX_IMG | OPTIMIZE_TEX_PAL | OPTIMIZE_REMAP
};

unsigned int getChunkPosition(FILE* fp, const char* magic);
bool write_bmd(const struct aiScene *model_data, FILE *output_fp, bool write_bdl);

#endif /* __BMD_H */
