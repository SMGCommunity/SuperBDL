#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <png.h>

#include "vector.h"

union Vector4uc **read_png(FILE *png_fp, size_t *image_size, unsigned int *width, unsigned int *height) {
	unsigned char header[8];

	fread(header, 1, 8, png_fp);

	if (png_sig_cmp(header, 0, 8))
		return NULL;

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		return NULL;

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		return NULL;

	if (setjmp(png_jmpbuf(png_ptr)))
		return NULL;

	png_init_io(png_ptr, png_fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	int color_type = png_get_color_type(png_ptr, info_ptr);
	int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	if (color_type == PNG_COLOR_TYPE_RGB || (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8))
		png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);

	png_read_update_info(png_ptr, info_ptr);

	*width = png_get_image_width(png_ptr, info_ptr);
	*height = png_get_image_height(png_ptr, info_ptr);

	if (setjmp(png_jmpbuf(png_ptr)))
		return NULL;

	*image_size = 0;
	size_t *sizes_array = malloc(sizeof (size_t *) * *height);

	for (int y = 0; y < *height; ++y) {
		sizes_array[y] = png_get_rowbytes(png_ptr, info_ptr);
		*image_size += sizes_array[y];
	}

	void *buf = malloc(sizeof (png_bytep) * *height + *image_size);
	png_bytepp pixels_array = buf;
	png_bytep pixels = buf + sizeof (png_bytep) * *height;

	for (int y = 0; y < *height; ++y) {
		pixels_array[y] = pixels;
		pixels += sizes_array[y];
	}

	free(sizes_array);

	png_read_image(png_ptr, pixels_array);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	return buf;
}
