#ifndef __READ_PNG_H
#define __READ_PNG_H

#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

struct rgba_image {
	size_t size;
	unsigned int width;
	unsigned int height;
	union Vector4uc **pixels;
};

struct rgba_image read_png(FILE *png_fp);

#endif /* __READ_PNG_H */
