#ifndef __INF1_H
#define __INF1_H

#include <stdbool.h>
#include <bmd.h>

#define MAGIC_INF1 "INF1"

bool writeINF1(const struct aiScene *data);
bool writeINF1Node(HierarchyNodeType type, unsigned short index);

enum HierarchyNodeType
{
	FINISH = 0x00,
	OPEN = 0x01,
	CLOSE = 0x02,

	JOINT = 0x10,
	MATERIAL = 0x11,
	SHAPE = 0x12
};

#endif /* __INF1_H */