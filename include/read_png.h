#ifndef __READ_PNG_H
#define __READ_PNG_H

#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

//pixels[y][x] for a given pixel
//*pixels for a straight array
struct rgba_image {
	size_t size;
	unsigned int width;
	unsigned int height;
	union Vector4uc **pixels;

	struct rgba_image* nextMipmap;
};

struct rgba_image read_png(FILE *png_fp);

#define PIXELACCESS(x, y) pixels[y][x]

#endif /* __READ_PNG_H */
