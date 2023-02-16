#ifndef __IMAGECONVERTER_H
#define __IMAGECONVERTER_H
#include <stdbool.h>
#include <stdio.h>

#include "read_png.h"
#include "tex1.h"

#include "stb_dxt.h"

enum GXImageFormats calculateBestImageFormat(struct rgba_image* sourceImage);

unsigned int calculateImageBufferSize(struct rgba_image* sourceImage, enum GXImageFormats imageFormatTarget);
unsigned int calculateImageBufferSize_4Bit(struct rgba_image* sourceImage);
unsigned int calculateImageBufferSize_8Bit(struct rgba_image* sourceImage);
unsigned int calculateImageBufferSize_16Bit(struct rgba_image* sourceImage);
unsigned int calculateImageBufferSize_32Bit(struct rgba_image* sourceImage);
unsigned int calculateImageBufferSize_CMPR(struct rgba_image* sourceImage);

/// <summary>
/// Input must be padded to the nearest 4x4
/// </summary>
/// <param name="sourceImage"> - The source RGBA</param>
/// <param name="dest">- the destination byte array pointer</param>
/// <param name="AlphaCutoff">- the value to use for cutting off alpha. Alpha values below this are made fully transperant</param>
/// <param name="HighQuality">- enables STB's High quality mode</param>
void createDXT1FromRGBA(struct rgba_image* sourceImage, unsigned char* dest, int AlphaCutoff, bool HighQuality);

bool writeImageBuffer(struct rgba_image* sourceImage, unsigned char* destBuffer, enum GXImageFormats targetFormat);

bool writeImageBuffer_I4(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool writeImageBuffer_I8(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool writeImageBuffer_IA4(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool writeImageBuffer_IA8(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool writeImageBuffer_R5G6B5(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool writeImageBuffer_RGB5A3(struct rgba_image* sourceImage, unsigned char* destBuffer);
bool writeImageBuffer_RGBA8(struct rgba_image* sourceImage, unsigned char* destBuffer);
//CI4 not supported
//CI8 not supported
//CI14_X2 not supported
bool writeImageBuffer_CMPR(struct rgba_image* sourceImage, unsigned char* destBuffer);


bool packTile_I4(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool packTile_I8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool packTile_IA4(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool packTile_IA8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool packTile_R5G6B5(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool packTile_RGB5A3(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
bool packTile_RGBA8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
//CI4 not supported
//CI8 not supported
//CI14_X2 not supported
bool packTile_CMPR(struct rgba_image* sourceImage, unsigned int tilex, unsigned int tiley, unsigned short* dstPtr);
void fixCMPR(unsigned short* value);

#endif /* __IMAGECONVERTER_H */
