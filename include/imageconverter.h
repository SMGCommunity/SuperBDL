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


bool WriteImageBuffer(struct rgba_image* sourceImage, unsigned char* destBuffer, enum GXImageFormats targetFormat);

bool WriteImageBuffer_I4(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool WriteImageBuffer_I8(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool WriteImageBuffer_IA4(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool WriteImageBuffer_IA8(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool WriteImageBuffer_R5G6B5(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool WriteImageBuffer_RGB5A3(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool WriteImageBuffer_RGBA8(struct rgba_image* sourceImage, unsigned char* destBuffer);
//CI4 not supported
//CI8 not supported
//CI14_X2 not supported
bool WriteImageBuffer_CMPR(struct rgba_image* sourceImage, unsigned char* destBuffer);


bool PackTile_I4(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool PackTile_I8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool PackTile_IA4(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool PackTile_IA8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool PackTile_R5G6B5(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool PackTile_RGB5A3(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool PackTile_RGBA8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
//CI4 not supported
//CI8 not supported
//CI14_X2 not supported
bool PackTile_CMPR(struct rgba_image* sourceImage, unsigned int tilex, unsigned int tiley, unsigned short* dstPtr);
void FixCMPR(unsigned short* value);

#endif /* __IMAGECONVERTER_H */
