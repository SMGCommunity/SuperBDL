#ifndef __SHP1_H
#define __SHP1_H

#include <stdbool.h>
#include "bmd.h"
#include "vector.h"

#define MAGIC_SHP1 "SHP1"

bool writeSHP1(const struct aiScene *data);

struct Shape
{
	enum DisplayFlags displayFlags;

	float boundingRadius;
	union Vector3f boundingMin;
	union Vector3f boundingMax;
};

enum DisplayFlags
{
	SINGLE = 0x00,
	BILLBOARD = 0x01,
	BILLBOARD_Y = 0x02,
	MULTI = 0x03
};

enum PrimitiveType
{
	// ????
	NONE = 0x00,
	LOAD_INDX_A = 0x20,
	LOAD_INDX_B = 0x28,
	LOAD_INDX_C = 0x30,
	LOAD_INDX_D = 0x38,

	LOAD_BP_REG = 0x61,
	LOAD_CP_REG = 0x08,
	LOAD_XF_REG = 0x10,

	// Don't need to support
	QUADRILATERAL = 0x80,
	// Duplicate seen in Luigi's mansion?
	QUADRILATERAL_2 = 0x88,
	// Required to support
	TRIANGLE = 0x90,
	// Required to support
	TRIANGLE_STRIP = 0x98,
	// Required to support
	TRIANGLE_FAN = 0xA0,

	// Don't need to support
	LINE = 0xA8,
	// Don't need to support
	LINE_STRIP = 0xB0,
	// Don't need to support
	POINT = 0xB8
};

#endif /* __SHP1_H */