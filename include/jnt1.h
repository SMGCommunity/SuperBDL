#ifndef __JNT1_H
#define __JNT1_H

#include <stdbool.h>
#include <bmd.h>

#define MAGIC_JNT1 "JNT1"

bool writeJNT1(model_data data);
bool writeJoint(Joint joint);

struct Joint
{
	//TODO: Fill this out with tuple values like Vector3?
};

#endif /* __JNT1_H */