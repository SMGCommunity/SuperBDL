#ifndef __JNT1_H
#define __JNT1_H

#include <stdbool.h>
#include "bmd.h"
#include "vector.h"
#include "matrix.h"

#define MAGIC_JNT1 "JNT1"

bool writeJNT1(const struct aiScene *data);
bool writeJoint(struct Joint* joint);

short floatToShortRotation(float val);
float shortTofloatRotation(short val);

struct Joint
{
	short matrixFlags;
	char calculationFlags;
	//Need a padding byte here somehow (0xFF)
	union Vector3f scale;
	union Vector3s rotation; //Rotation is stored in signed units that range from -0x7FFF to 0x7FFF, corresponding to -180 degrees and 180 degrees, respectively.
	//2 bytes of padding here (0xFFFF)
	union Vector3f translation;
	float boundingRadius;
	union Vector3f boundingMin;
	union Vector3f boundingMax;

	//size should equal 0x40
};

#endif /* __JNT1_H */