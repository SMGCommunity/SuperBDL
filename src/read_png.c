#include <stdlib.h>
#include <stdio.h>

#include <png.h>

#include "vector.h"
#include "read_png.h"

struct rgba_image read_png(FILE *png_fp) {
	unsigned char header[8];

	fread(header, 1, 8, png_fp);

	if (png_sig_cmp(header, 0, 8))
		return (struct rgba_image) { 0 };

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		return (struct rgba_image) { 0 };

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		return (struct rgba_image) { 0 };

	if (setjmp(png_jmpbuf(png_ptr)))
		return (struct rgba_image) { 0 };

	png_init_io(png_ptr, png_fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	int color_type = png_get_color_type(png_ptr, info_ptr);
	int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	if (color_type == PNG_COLOR_TYPE_RGB || (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8))
		png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);

	png_read_update_info(png_ptr, info_ptr);

	unsigned int width = png_get_image_width(png_ptr, info_ptr);
	unsigned int height = png_get_image_height(png_ptr, info_ptr);

	if (setjmp(png_jmpbuf(png_ptr)))
		return (struct rgba_image) { 0 };

	size_t size = 0;
	size_t *sizes_array = malloc(sizeof (size_t *) * height);

	for (int y = 0; y < height; ++y) {
		sizes_array[y] = png_get_rowbytes(png_ptr, info_ptr);
		size += sizes_array[y];
	}

	void *buf = malloc(sizeof (png_bytep) * height + size);
	png_bytepp pixels_array = buf;
	png_bytep pixels = buf;
	pixels += sizeof (png_bytep) * height;

	for (int y = 0; y < height; ++y) {
		pixels_array[y] = pixels;
		pixels += sizes_array[y];
	}

	free(sizes_array);

	png_read_image(png_ptr, pixels_array);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	return (struct rgba_image) {
		.size = size,
		.width = width,
		.height = height,
		.pixels = buf,
		.nextMipmap = NULL
	};
}
