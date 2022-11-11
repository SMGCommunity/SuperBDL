#ifndef __DRW1_H
#define __DRW1_H

#include <stdbool.h>
#include <bmd.h>

#define MAGIC_DRW1 "DRW1"

bool writeDRW1(model_data data);

enum DRWMatrixType
{
	JOINT = 0x00,
	ENVELOPE = 0x01
};

#endif /* __DRW1_H */