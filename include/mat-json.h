#ifndef __MAT_JSON_H
#define __MAT_JSON_H

#include "mat3.h"
#include "tex1.h"

struct J3DMaterial **read_mat_json(FILE *fp, struct JUTTexture **tex);

#endif
