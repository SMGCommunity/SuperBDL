#ifndef __BMD_H
#define __BMD_H

#include <stdbool.h>

#include <assimp/scene.h>

bool write_bmd(const struct aiScene *model_data, FILE *output_fp, bool write_bdl, bool sm3das);

#endif /* __BMD_H */
