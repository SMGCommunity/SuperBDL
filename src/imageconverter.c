#include "imageconverter.h"

enum GXImageFormats calculateBestImageFormat(struct rgba_image* sourceImage)
{
	//TODO: figure this out...
	return RGBA32;
}


unsigned int calculateImageBufferSize(struct rgba_image* sourceImage, enum GXImageFormats imageFormatTarget)
{
	switch (imageFormatTarget)
	{
	case I4:
	case C4:
		return calculateImageBufferSize_4Bit(sourceImage);
	case I8:
	case IA4:
	case C8:
		return calculateImageBufferSize_8Bit(sourceImage);
	case IA8:
	case RGB565:
	case RGB5A3:
	case C14X2:
		return calculateImageBufferSize_16Bit(sourceImage);
	case RGBA32:
		return calculateImageBufferSize_32Bit(sourceImage);
	case CMPR:
		return calculateImageBufferSize_DXT1(sourceImage);
	case UNDEFINED:
	default:
		//ERROR
		return 0;
	}
}

unsigned int calculateImageBufferSize_4Bit(struct rgba_image* sourceImage)
{
	unsigned int tileCols, tileRows, size;
	unsigned int width = sourceImage->width, height= sourceImage->height;

	tileCols = ((width + 7) >> 3);
	tileRows = ((height + 7) >> 3);
	size = tileCols * tileRows * 32;
	return size;
}
unsigned int calculateImageBufferSize_8Bit(struct rgba_image* sourceImage)
{
	unsigned int tileCols, tileRows, size;
	unsigned int width = sourceImage->width, height = sourceImage->height;

	tileCols = ((width + 7) >> 3);
	tileRows = ((height + 3) >> 2);
	size = tileCols * tileRows * 32;
	return size;
}
unsigned int calculateImageBufferSize_16Bit(struct rgba_image* sourceImage)
{
	unsigned int tileCols, tileRows, size;
	unsigned int width = sourceImage->width, height = sourceImage->height;

	tileCols = ((width + 3) >> 2);
	tileRows = ((height + 3) >> 2);
	size = tileCols * tileRows * 32;
	return size;
}
unsigned int calculateImageBufferSize_32Bit(struct rgba_image* sourceImage)
{
	unsigned int tileCols, tileRows, size;
	unsigned int width = sourceImage->width, height = sourceImage->height;

	tileCols = ((width + 3) >> 2);
	tileRows = ((height + 3) >> 2);
	size = tileCols * tileRows * 32 * 2;
	return size;
}
unsigned int calculateImageBufferSize_DXT1(struct rgba_image* sourceImage)
{
	unsigned int tileCols, tileRows, size;
	unsigned int width = sourceImage->width, height = sourceImage->height;

	// must pad any image < 8 texels out to 8 texel boundary
	tileCols = ((width + 7) >> 3);
	tileRows = ((height + 7) >> 3);
	size = (tileRows * tileCols * 32);
	return size;
}