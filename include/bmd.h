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

unsigned int getChunkPosition(FILE* fp, const char* magic);
bool write_bmd(const struct aiScene *model_data, FILE *output_fp, bool write_bdl, bool sm3das);

#endif /* __BMD_H */
