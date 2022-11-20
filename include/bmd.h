#ifndef __BMD_H
#define __BMD_H

#include <stdbool.h>
#include <stdio.h>

#include <assimp/scene.h>

#define MAGIC_J3D "J3D2"
#define MAGIC_BDL "bdl4"
#define MAGIC_BMD "bmd3"

bool write_bmd(const struct aiScene *model_data, FILE *output_fp, bool write_bdl, bool sm3das);

#endif /* __BMD_H */
