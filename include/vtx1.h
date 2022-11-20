#ifndef __VTX1_H
#define __VTX1_H

#include <stdbool.h>
#include "bmd.h"

#define MAGIC_VTX1 "VTX1"

bool writeVTX1(const struct aiScene *data);
bool writeGXVertexAttribute(GXVertexAttribute attribute);

struct GXVertexAttribute
{
	/// <summary>
	/// The vertex attribute being defined
	/// </summary>
	GXAttr attribute;
	/// <summary>
	/// The number of values
	/// </summary>
	GXCompCount count;
	/// <summary>
	/// The type of values
	/// </summary>
	GXCompType type;
	/// <summary>
	/// Shift for fixed-point attribute types
	/// </summary>
	char Shift;
};

enum GXAttr
{
	POSITION_MTX = 0x00,
	TEXTURE0_MTX = 0x01,
	TEXTURE1_MTX = 0x02,
	TEXTURE2_MTX = 0x03,
	TEXTURE3_MTX = 0x04,
	TEXTURE4_MTX = 0x05,
	TEXTURE5_MTX = 0x06,
	TEXTURE6_MTX = 0x07,
	TEXTURE7_MTX = 0x08,
	POSITION	 = 0x09,
	NORMAL		 = 0x0A,
	COLOR0		 = 0x0B,
	COLOR1		 = 0x0C,
	TEXTURE0	 = 0x0D,
	TEXTURE1	 = 0x0E,
	TEXTURE2	 = 0x0F,
	TEXTURE3	 = 0x10,
	TEXTURE4	 = 0x11,
	TEXTURE5	 = 0x12,
	TEXTURE6	 = 0x13,
	TEXTURE7	 = 0x14,

	//Don't use this.
	NORMAL_BINORMAL_TANGENT = 0x19,

	NULL = 0xFF
};

enum GXCompType
{
	UNSIGNED_BYTE	= 0x00,
	SIGNED_BYTE		= 0x01,
	UNSIGNED_SHORT	= 0x02,
	SIGNED_SHORT	= 0x03,
	FLOAT			= 0x05,

	RGB565 = 0x00,
	RGB8   = 0x01,
	RGBX8  = 0x02,
	RGBA4  = 0x03,
	RGBA6  = 0x04,
	RGBA8  = 0x05
};

enum GXCompCount
{
	POSITION_XY = 0,
	POSITION_XYZ,

	NORMAL_XYZ = 0,
	NORMAL_NBT,
	NORMAL_NBT3,

	COLOR_RGB = 0,
	COLOR_RGBA,

	TEXCOORD_S = 0,
	TEXCOORD_ST
};

#endif /* __VTX1_H */