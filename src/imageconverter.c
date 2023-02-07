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




//====================================================================================== by far the worst part about image conversion...

bool WriteImageBuffer(struct rgba_image* sourceImage, unsigned char* destBuffer, enum GXImageFormats targetFormat)
{
	switch (targetFormat)
	{
	case I4:
		return WriteImageBuffer_I4(sourceImage, destBuffer);
	case I8:
		return WriteImageBuffer_I8(sourceImage, destBuffer);
	case IA4:
		return WriteImageBuffer_IA4(sourceImage, destBuffer);
	case IA8:
		return WriteImageBuffer_IA8(sourceImage, destBuffer);
	case RGB565:
		return WriteImageBuffer_R5G6B5(sourceImage, destBuffer);
	case RGB5A3:
		return WriteImageBuffer_RGB5A3(sourceImage, destBuffer);
	case RGBA32:
		return WriteImageBuffer_RGBA8(sourceImage, destBuffer);
	case CMPR:
		return WriteImageBuffer_CMPR(sourceImage, destBuffer);

	case C4:
	case C8:
	case C14X2:
	case UNDEFINED:
	default:
		//ERROR
		return false;
	}
}




bool WriteImageBuffer_I4(struct rgba_image* sourceImage, unsigned char* destBuffer)
{
	// number of 8x8 texel tile cols, rows including any partial tiles
	unsigned int width= sourceImage->width, height= sourceImage->height;
	unsigned int numTileRows = ((height + 7) >> 3), tileRow;
	unsigned int numTileCols = ((width + 7) >> 3), tileCol;
	unsigned char* dstPtr = destBuffer;

	// numTileRows, numTileCols includes any partial tiles
	for (tileRow = 0; tileRow < numTileRows; tileRow++)
	{
		for (tileCol = 0; tileCol < numTileCols; tileCol++)
		{
			PackTile_I4(sourceImage, (tileCol * 8), (tileRow * 8), dstPtr);
			dstPtr += 32;
		}
	}
}
bool PackTile_I4(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr)
{
	unsigned char* tilePtr;
	unsigned int realRows = sourceImage->height - y;
	unsigned int realCols = sourceImage->width - x;

	if (realRows > 8)
		realRows = 8;

	if (realCols > 8)
		realCols = 8;

	for (unsigned int row = 0; row < realRows; row++)
	{
		tilePtr = dstPtr + (row * 4);
		for (unsigned int col = 0; col < realCols; col++)
		{
			union Vector4uc currentPixel = sourceImage->PIXELACCESS(x + col, y + row);

			//Only supporting RGBA32
			unsigned short val = (currentPixel.r + currentPixel.g + currentPixel.b) / 3;

			if ((col % 2) == 0) // Even iterations, pack the high 4-bits
			{
				*tilePtr = (val & 0x00F0);
			}
			else // Odd iterations, pack the high 4-bits and move to the next byte
			{
				*tilePtr |= ((val & 0x00F0) >> 4);
				tilePtr++;
			}
		}
	}
}

bool WriteImageBuffer_I8(struct rgba_image* sourceImage, unsigned char* destBuffer)
{
	// number of 4x8 texel tile cols, rows including any partial tiles
	unsigned int width = sourceImage->width, height = sourceImage->height;
	unsigned int numTileRows = ((height + 3) >> 2), tileRow;
	unsigned int numTileCols = ((width + 7) >> 3), tileCol;
	unsigned char* dstPtr = destBuffer;

	// numTileRows, numTileCols includes any partial tiles
	for (tileRow = 0; tileRow < numTileRows; tileRow++)
	{
		for (tileCol = 0; tileCol < numTileCols; tileCol++)
		{
			PackTile_I8(sourceImage, (tileCol * 8), (tileRow * 4), dstPtr);
			dstPtr += 32;
		}
	}
}
bool PackTile_I8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr)
{
	unsigned char* tilePtr;

	unsigned int realRows = sourceImage->height - y;
	unsigned int realCols = sourceImage->width - x;

	if (realRows > 4)
		realRows = 4;

	if (realCols > 8)
		realCols = 8;

	for (unsigned int row = 0; row < realRows; row++)
	{
		tilePtr = dstPtr + (row * 8);
		for (unsigned int col = 0; col < realCols; col++)
		{
			union Vector4uc currentPixel = sourceImage->PIXELACCESS(x + col, y + row);

			//Only supporting RGBA32
			unsigned short val = (currentPixel.r + currentPixel.g + currentPixel.b) / 3;

			*tilePtr = (unsigned char)val;
			tilePtr++;
		}
	}
}

bool WriteImageBuffer_IA4(struct rgba_image* sourceImage, unsigned char* destBuffer)
{
	unsigned int width = sourceImage->width, height = sourceImage->height;
	unsigned int numTileRows, tileRow;
	unsigned int numTileCols, tileCol;
	unsigned char* dstPtr = destBuffer;

	// number of 4x8 texel tile cols, rows including any partial tiles
	numTileCols = ((width + 7) >> 3);
	numTileRows = ((height + 3) >> 2);

	// numTileRows, numTileCols includes any partial tiles
	for (tileRow = 0; tileRow < numTileRows; tileRow++)
	{
		for (tileCol = 0; tileCol < numTileCols; tileCol++)
		{
			PackTile_IA4(sourceImage, (tileCol * 8), (tileRow * 4), dstPtr);
			dstPtr += 32;
		}
	}
}
bool PackTile_IA4(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr)
{
	unsigned char* tilePtr;
	unsigned int realRows = sourceImage->height - y;
	unsigned int realCols = sourceImage->width - x;

	if (realRows > 4)
		realRows = 4;

	if (realCols > 8)
		realCols = 8;

	// pack 32B tile 
	for (unsigned int row = 0; row < realRows; row++)
	{
		tilePtr = dstPtr + (row * 8);                       // move 8 bytes (8 8-bit texels) per row
		// need to reset ptr each row to account for
		// column padding
		for (unsigned int col = 0; col < realCols; col++)
		{
			union Vector4uc currentPixel = sourceImage->PIXELACCESS(x + col, y + row);

			//Only supporting RGBA32
			unsigned short val = (currentPixel.r + currentPixel.g + currentPixel.b) / 3;

			*tilePtr = ((currentPixel.a & 0xF0) | ((val & 0x00F0) >> 4));	// high 4-bits are alpha, low 4-bits are intensity  
			tilePtr++;
		}
	}
}

bool WriteImageBuffer_IA8(struct rgba_image* sourceImage, unsigned char* destBuffer)
{
	unsigned int width = sourceImage->width, height = sourceImage->height;
	unsigned int numTileRows, tileRow;
	unsigned int numTileCols, tileCol;
	unsigned char* dstPtr = destBuffer;

	// number of 4x4 texel tile cols, rows including any partial tiles
	numTileCols = ((width + 3) >> 2);
	numTileRows = ((height + 3) >> 2);

	// numTileRows, numTileCols includes any partial tiles
	for (tileRow = 0; tileRow < numTileRows; tileRow++)
	{
		for (tileCol = 0; tileCol < numTileCols; tileCol++)
		{
			PackTile_IA8(sourceImage, (tileCol * 4), (tileRow * 4), dstPtr);
			dstPtr += 32;
		}
	}
}
bool PackTile_IA8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr)
{
	unsigned char* tilePtr;
	unsigned int realRows = sourceImage->height - y;
	unsigned int realCols = sourceImage->width - x;

	if (realRows > 4)
		realRows = 4;

	if (realCols > 4)
		realCols = 4;

	// pack 32B tile 
	for (unsigned int row = 0; row < realRows; row++)
	{
		tilePtr = dstPtr + (row * 8);                       // move 8 bytes (4 16-bit texels) per row
		// need to reset ptr each row to account for
		// column padding
		for (unsigned int col = 0; col < realCols; col++)
		{
			union Vector4uc currentPixel = sourceImage->PIXELACCESS(x + col, y + row);
			*tilePtr = currentPixel.a;
			unsigned short val = (currentPixel.r + currentPixel.g + currentPixel.b) / 3;
			*(tilePtr + 1) = (unsigned char)val;
			tilePtr += 2;
		}
	}
}

bool WriteImageBuffer_R5G6B5(struct rgba_image* sourceImage, unsigned char* destBuffer)
{
	unsigned int width = sourceImage->width, height = sourceImage->height;
	unsigned int numTileRows, tileRow;
	unsigned int numTileCols, tileCol;
	unsigned char* dstPtr = destBuffer;

	// number of 4x4 texel tile cols, rows including any partial tiles
	numTileCols = ((width + 3) >> 2);
	numTileRows = ((height + 3) >> 2);

	// numTileRows, numTileCols includes any partial tiles
	for (tileRow = 0; tileRow < numTileRows; tileRow++)
	{
		for (tileCol = 0; tileCol < numTileCols; tileCol++)
		{
			PackTile_R5G6B5(sourceImage, (tileCol * 4), (tileRow * 4), dstPtr);
			dstPtr += 32;
		}
	}
}
bool PackTile_R5G6B5(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr)
{
	unsigned char* tilePtr;
	unsigned int realRows = sourceImage->height - y;
	unsigned int realCols = sourceImage->width - x;

	if (realRows > 4)
		realRows = 4;

	if (realCols > 4)
		realCols = 4;

	// pack 32B tile 
	for (unsigned int row = 0; row < realRows; row++)
	{
		tilePtr = dstPtr + (row * 8);                       // move 8 bytes (4 16-bit texels) per row

		for (unsigned int col = 0; col < realCols; col++)
		{
			union Vector4uc currentPixel = sourceImage->PIXELACCESS(x + col, y + row);
			*tilePtr = ((currentPixel.r & 0x00F8) | ((currentPixel.g & 0xE0) >> 5));  // byte0 is upper 5 bits of red, upper 3 of green
			*(tilePtr + 1) = (((currentPixel.g & 0x1C) << 3) | (currentPixel.b >> 3));  // byte1 is lower 3 bits of green, upper 5 of blue

			tilePtr += 2;
		}
	}
}

bool WriteImageBuffer_RGB5A3(struct rgba_image* sourceImage, unsigned char* destBuffer)
{
	unsigned int width = sourceImage->width, height = sourceImage->height;
	unsigned int numTileRows, tileRow;
	unsigned int numTileCols, tileCol;
	unsigned char* dstPtr = destBuffer;

	// number of 4x4 texel tile cols, rows including any partial tiles
	numTileCols = ((width + 3) >> 2);
	numTileRows = ((height + 3) >> 2);

	// numTileRows, numTileCols includes any partial tiles
	for (tileRow = 0; tileRow < numTileRows; tileRow++)
	{
		for (tileCol = 0; tileCol < numTileCols; tileCol++)
		{
			PackTile_RGB5A3(sourceImage, (tileCol * 4), (tileRow * 4), dstPtr);
			dstPtr += 32;
		}
	}
}
bool PackTile_RGB5A3(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr)
{
	unsigned char* tilePtr;
	unsigned int realRows = sourceImage->height - y;
	unsigned int realCols = sourceImage->width - x;

	if (realRows > 4)
		realRows = 4;

	if (realCols > 4)
		realCols = 4;

	// pack 32B tile 
	for (unsigned int row = 0; row < realRows; row++)
	{
		tilePtr = dstPtr + (row * 8);                       // move 8 bytes (4 16-bit texels) per row
		// need to reset ptr each row to account for
		// column padding
		for (unsigned int col = 0; col < realCols; col++)
		{
			union Vector4uc currentPixel = sourceImage->PIXELACCESS(x + col, y + row);

			// check alpha to determine whether to pack color 5551 or 4443 

			// since hw replicates msbs, everything >= 224 of original alpha 
			// will unpack as 255 ( 1110 0000 unpacks as 1111 1111 )

			if (currentPixel.a >= 224) // pixel is opaque
			{
				// pack in 5551 format, msb is set to 1
				// byte0 is 1 bit alpha, upper 5-bits
				// of red, upper 2-bits of green
				// byte1 is bits 3-5 of green, upper 5 of blue
				*tilePtr = ((0x0080) | ((currentPixel.r & 0xF8) >> 1) | (currentPixel.g >> 6)); 
				*(tilePtr + 1) = (((currentPixel.g & 0x38) << 2) | (currentPixel.b >> 3));
			}
			else // pixel is translucent
			{
				// pack in 4443 format,  shift alpha by 1 and set msb to 0
				// byte0 is 1 bit 0, 3 alpha, 4-bits red
				// 4-bits green, 4-bits blue
				*tilePtr = (((currentPixel.a >> 1) & 0x70) | ((currentPixel.r & 0xF0) >> 4));
				*(tilePtr + 1) = ((currentPixel.g & 0xF0) | ((currentPixel.b & 0xF0) >> 4));
			}

			tilePtr += 2;

		}
	}
}

bool WriteImageBuffer_RGBA8(struct rgba_image* sourceImage, unsigned char* destBuffer)
{
	unsigned int width = sourceImage->width, height = sourceImage->height;
	unsigned int numTileRows, tileRow;
	unsigned int numTileCols, tileCol;
	unsigned char* dstPtr = destBuffer;

	// number of 4x4 texel tile cols, rows including any partial tiles
	numTileCols = ((width + 3) >> 2);
	numTileRows = ((height + 3) >> 2);

	// numTileRows, numTileCols includes any partial tiles
	for (tileRow = 0; tileRow < numTileRows; tileRow++)
	{
		for (tileCol = 0; tileCol < numTileCols; tileCol++)
		{
			PackTile_RGBA8(sourceImage, (tileCol * 4), (tileRow * 4), dstPtr);
			dstPtr += 64;
		}
	}
}
bool PackTile_RGBA8(struct rgba_image* sourceImage, unsigned int x, unsigned int y, unsigned char* dstPtr)
{
	unsigned int realRows = sourceImage->height - y;
	unsigned int realCols = sourceImage->width - x;

	if (realRows > 4)
		realRows = 4;

	if (realCols > 4)
		realCols = 4;

	// pack 2 32B tiles
	for (unsigned int row = 0; row < realRows; row++)
	{
		// pack 2 cache lines at once
		unsigned char* arPtr = dstPtr + (row * 8);                   // move 8 bytes (4 16-bit texels) per row
		unsigned char* gbPtr = dstPtr + 32 + (row * 8);                   // need to reset ptr each row to account for
		// column padding

		for (unsigned int col = 0; col < realCols; col++)
		{
			union Vector4uc currentPixel = sourceImage->PIXELACCESS(x + col, y + row);

			*arPtr = currentPixel.a; // alpha is byte 0, red is byte 1
			*(arPtr + 1) = currentPixel.r;

			*gbPtr = currentPixel.g; // green is byte 0, blue is byte 1
			*(gbPtr + 1) = currentPixel.b;

			arPtr += 2;
			gbPtr += 2;
		}
	}
}



bool WriteImageBuffer_CMPR(struct rgba_image* sourceImage, unsigned char* destBuffer)
{
	unsigned int srcTileRows, srcTileCols;
	unsigned short* dstPtr = (unsigned short*)(destBuffer);

	// each source tile is 4x4 texels, 8B
	srcTileRows = ((sourceImage->height + 3) >> 2);
	srcTileCols = ((sourceImage->width + 3) >> 2);

	// each dst tile is 2x2 source tiles, so move by 2 each iteration
	for (unsigned int tileRow = 0; tileRow < srcTileRows; tileRow += 2)
	{
		for (unsigned int tileCol = 0; tileCol < srcTileCols; tileCol += 2)
		{
			PackTile_CMP(sourceImage, tileCol, tileRow, dstPtr);
			dstPtr += 16;
		}
	}
}
bool PackTile_CMPR(struct rgba_image* sourceImage, unsigned int tileX, unsigned int tileY, unsigned char* dstPtr)
{
	unsigned int  x, y;
	unsigned short* srcPtr;
	unsigned short  tmp;
	unsigned int  srcTileOffset;
	unsigned int  subTileRows, subRowShorts;    // number of DXT1 4x4 tiles
	unsigned int  srcPadWidth, srcPadHeight;
	unsigned short* buffPtr;

	// set the padded size of the s3 source image out to a 4-texel boundary
	srcPadWidth = ((sourceImage->width + 3) >> 2);
	srcPadHeight = ((sourceImage->height + 3) >> 2);

	// number of bytes in a single row of 4x4 texel source tiles
	srcTileOffset = srcPadWidth * 8;

	// number of 4x4 (source) tile rows to copy ( will be 1 or 2 )
	subTileRows = 2;
	if ((srcPadHeight - tileY) < 2)
		subTileRows = 1;

	// number of 4x4 tile cols to copy translated into number of short values
	// ( will be 4 or 8 )
	subRowShorts = 8;
	if ((srcPadWidth - tileX) < 2)
		subRowShorts = 4;

	for (y = 0; y < subTileRows; y++)
	{
		srcPtr = (unsigned short*)((unsigned char*)(*(sourceImage->pixels)) + ((tileY + y) * srcTileOffset) + (tileX * 8));
		buffPtr = (dstPtr + (y * 8));        // 16 bytes per subRow = 8 shorts

		// process one or both 4x4 row tiles at once- 4 short each
		for (x = 0; x < subRowShorts; x++)
		{
			switch (x)
			{

				// color table entries - switch bytes within a 16-bit world only
			case 0:
			case 1:
			case 4:
			case 5:
				tmp = *srcPtr++;
				TCFixEndian((unsigned char*)(&tmp), 2);
				*buffPtr++ = tmp;
				break;

				// 2-bit color tuples;
				// reverse tuple order within bytes of a word
			case 2:
			case 3:
			case 6:
			case 7:
				tmp = *srcPtr++;
				TCFixCMPWord(&tmp);
				*buffPtr++ = tmp;
				break;

			} // end switch
		} // end for( subRowShorts )			
	} // end for( subTileRows )
}