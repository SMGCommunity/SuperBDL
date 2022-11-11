#ifndef __SHP1_H
#define __SHP1_H

#include <stdbool.h>
#include <bmd.h>

#define MAGIC_SHP1 "SHP1"

bool writeSHP1(model_data data);

enum DisplayFlags
{
	SINGLE = 0x00,
	BILLBOARD = 0x01,
	BILLBOARD_Y = 0x02,
	MULTI = 0x03
};

enum PrimitiveType
{
	// Don't need to support
	QUADRILATERAL = 0x80,

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