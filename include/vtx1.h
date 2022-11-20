#ifndef __VTX1_H
#define __VTX1_H

#include <stdbool.h>
#include "bmd.h"

#define MAGIC_VTX1 "VTX1"

bool writeVTX1(const struct aiScene *data);
bool writeGXVertexAttribute(struct GXVertexAttribute* attribute);

struct GXVertexAttribute
{
	/// <summary>
	/// The vertex attribute being defined
	/// </summary>
	enum GXAttr attribute;
	/// <summary>
	/// The number of values
	/// </summary>
	enum GXCompCount count;
	/// <summary>
	/// The type of values
	/// </summary>
	enum GXCompType type;
	/// <summary>
	/// Shift for fixed-point attribute types
	/// </summary>
	char Shift;
};

enum GXAttr
{
	GXATTR_POSITION_MTX = 0x00,
	GXATTR_TEXTURE0_MTX = 0x01,
	GXATTR_TEXTURE1_MTX = 0x02,
	GXATTR_TEXTURE2_MTX = 0x03,
	GXATTR_TEXTURE3_MTX = 0x04,
	GXATTR_TEXTURE4_MTX = 0x05,
	GXATTR_TEXTURE5_MTX = 0x06,
	GXATTR_TEXTURE6_MTX = 0x07,
	GXATTR_TEXTURE7_MTX = 0x08,
	GXATTR_POSITION	 = 0x09,
	GXATTR_NORMAL		 = 0x0A,
	GXATTR_COLOR0		 = 0x0B,
	GXATTR_COLOR1		 = 0x0C,
	GXATTR_TEXTURE0	 = 0x0D,
	GXATTR_TEXTURE1	 = 0x0E,
	GXATTR_TEXTURE2	 = 0x0F,
	GXATTR_TEXTURE3	 = 0x10,
	GXATTR_TEXTURE4	 = 0x11,
	GXATTR_TEXTURE5	 = 0x12,
	GXATTR_TEXTURE6	 = 0x13,
	GXATTR_TEXTURE7	 = 0x14,

	//Don't use this.
	GXATTR_NORMAL_BINORMAL_TANGENT = 0x19,

	GXATTR_NULL = 0xFF
};

enum GXCompType
{
	GXCOMP_UNSIGNED_BYTE	= 0x00,
	GXCOMP_SIGNED_BYTE		= 0x01,
	GXCOMP_UNSIGNED_SHORT	= 0x02,
	GXCOMP_SIGNED_SHORT	= 0x03,
	GXCOMP_FLOAT			= 0x05,

	GXCOMP_RGB565 = 0x00,
	GXCOMP_RGB8   = 0x01,
	GXCOMP_RGBX8  = 0x02,
	GXCOMP_RGBA4  = 0x03,
	GXCOMP_RGBA6  = 0x04,
	GXCOMP_RGBA8  = 0x05
};

enum GXCompCount
{
	GXCOUNT_POSITION_XY = 0,
	GXCOUNT_POSITION_XYZ,

	GXCOUNT_NORMAL_XYZ = 0,
	GXCOUNT_NORMAL_NBT,
	GXCOUNT_NORMAL_NBT3,

	GXCOUNT_COLOR_RGB = 0,
	GXCOUNT_COLOR_RGBA,

	GXCOUNT_TEXCOORD_S = 0,
	GXCOUNT_TEXCOORD_ST
};

#endif /* __VTX1_H */