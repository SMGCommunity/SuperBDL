#ifndef __IMAGECONVERTER_H
#define __IMAGECONVERTER_H
#include <stdbool.h>
#include <stdio.h>

#include "image.h"
#include "tex1.h"

#include "stb_dxt.h"

extern enum GXImageFormats calculateBestImageFormat(struct rgba_image* sourceImage);
extern enum JUTTransparency calculateBestTransparency(struct rgba_image* sourceImage);

extern unsigned int calculateImageBufferSize(unsigned int width, unsigned int height, enum GXImageFormats imageFormatTarget);
extern unsigned int calculateImageBufferSize_4Bit(unsigned int width, unsigned int height);
extern unsigned int calculateImageBufferSize_8Bit(unsigned int width, unsigned int height);
extern unsigned int calculateImageBufferSize_16Bit(unsigned int width, unsigned int height);
extern unsigned int calculateImageBufferSize_32Bit(unsigned int width, unsigned int height);
extern unsigned int calculateImageBufferSize_CMPR(unsigned int width, unsigned int height);

/// <summary>
/// Input must be padded to the nearest 4x4
/// </summary>
/// <param name="sourceImage"> - The source RGBA</param>
/// <param name="dest">- the destination byte array pointer</param>
/// <param name="AlphaCutoff">- the value to use for cutting off alpha. Alpha values below this are made fully transperant</param>
/// <param name="HighQuality">- enables STB's High quality mode</param>
extern struct dxt1_image rgba_to_dxt1(struct rgba_image *src, int alpha_cutoff, bool high_quality);

extern bool writeImageBuffer(struct rgba_image* sourceImage, unsigned char* destBuffer, enum GXImageFormats targetFormat);

extern bool writeImageBuffer_I4(struct rgba_image* sourceImage, unsigned char* destBuffer);
extern bool writeImageBuffer_I8(struct rgba_image* sourceImage, unsigned char* destBuffer);
extern bool writeImageBuffer_IA4(struct rgba_image* sourceImage, unsigned char* destBuffer);
extern bool writeImageBuffer_IA8(struct rgba_image* sourceImage, unsigned char* destBuffer);
extern bool writeImageBuffer_R5G6B5(struct rgba_image* sourceImage, unsigned char* destBuffer);
extern bool writeImageBuffer_RGB5A3(struct rgba_image* sourceImage, unsigned char* destBuffer);
extern bool writeImageBuffer_RGBA8(struct rgba_image* sourceImage, unsigned char* destBuffer);
//CI4 not supported
//CI8 not supported
//CI14_X2 not supported
extern bool writeImageBuffer_CMPR(struct dxt1_image* sourceImage, unsigned char* destBuffer);


extern bool packTile_I4(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
extern bool packTile_I8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
extern bool packTile_IA4(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
extern bool packTile_IA8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
extern bool packTile_R5G6B5(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
extern bool packTile_RGB5A3(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
extern bool packTile_RGBA8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr);
//CI4 not supported
//CI8 not supported
//CI14_X2 not supported
extern bool packTile_CMPR(struct dxt1_image* sourceImage, unsigned int tilex, unsigned int tiley, unsigned short* dstPtr);
extern void fixCMPR(unsigned short* value);


/// <summary>
/// Creates a JUTTexture's data from a RGBA source
/// </summary>
/// <param name="sourceImage">- the original source</param>
/// <param name="dest">- the destination struct.</param>
/// <returns></returns>
extern bool createJUTTextureData(struct rgba_image* sourceImage, struct JUTTexture* dest);

#endif /* __IMAGECONVERTER_H */
