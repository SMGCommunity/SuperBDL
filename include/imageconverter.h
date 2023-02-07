#ifndef __IMAGECONVERTER_H
#define __IMAGECONVERTER_H
#include <stdbool.h>
#include <stdio.h>

#include "read_png.h"
#include "tex1.h"



enum GXImageFormats calculateBestImageFormat(struct rgba_image* sourceImage);
unsigned int calculateImageBufferSize(struct rgba_image* sourceImage, enum GXImageFormats imageFormatTarget);
unsigned int calculateImageBufferSize_4Bit(struct rgba_image* sourceImage);
unsigned int calculateImageBufferSize_8Bit(struct rgba_image* sourceImage);
unsigned int calculateImageBufferSize_16Bit(struct rgba_image* sourceImage);
unsigned int calculateImageBufferSize_32Bit(struct rgba_image* sourceImage);
unsigned int calculateImageBufferSize_DXT1(struct rgba_image* sourceImage);

#endif /* __IMAGECONVERTER_H */
