#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <json.h>

#include "tex1.h"
#include "tex-json.h"

struct JUTTexture **read_tex_json(FILE *fp) {
	fseek(fp, 0, SEEK_END);
	size_t tex_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *tex_str = calloc(1, tex_size + 1);

	fread(tex_str, tex_size, 1, fp);
	fclose(fp);

	struct json_object *tex_obj = json_tokener_parse(tex_str);
	free(tex_str);

	if (!tex_obj)
		return NULL;

	size_t array_size = json_object_array_length(tex_obj);

	struct JUTTexture **tex = calloc(array_size + 1, sizeof (struct JUTTexture *));

	for (int i = 0; i < array_size; ++i) {
		struct json_object *tex_array_obj = json_object_array_get_idx(tex_obj, i);
		if (!tex_array_obj)
			return NULL;

		tex[i] = calloc(1, sizeof (struct JUTTexture));

		// Name
		tex[i]->Name = strdup(json_object_get_string(json_object_object_get(tex_array_obj, "Name")));

		// Format
		const char *format = json_object_get_string(json_object_object_get(tex_array_obj, "Format"));
		if (!strcmp(format, "I4"))
			tex[i]->Format = I4;
		else if (!strcmp(format, "I8"))
			tex[i]->Format = I8;
		else if (!strcmp(format, "IA4"))
			tex[i]->Format = IA4;
		else if (!strcmp(format, "IA8"))
			tex[i]->Format = IA8;
		else if (!strcmp(format, "RGB565"))
			tex[i]->Format = RGB565;
		else if (!strcmp(format, "RGB5A3"))
			tex[i]->Format = RGB5A3;
		else if (!strcmp(format, "RGBA32"))
			tex[i]->Format = RGBA32;
		else if (!strcmp(format, "C4"))
			tex[i]->Format = C4;
		else if (!strcmp(format, "C8"))
			tex[i]->Format = C8;
		else if (!strcmp(format, "C14X2"))
			tex[i]->Format = C14X2;
		else if (!strcmp(format, "CMPR"))
			tex[i]->Format = CMPR;
		else
			return NULL;

		// AlphaSetting
		tex[i]->AlphaSetting = json_object_get_int(json_object_object_get(tex_array_obj, "AlphaSetting"));

		// WrapS
		const char *wrap_s = json_object_get_string(json_object_object_get(tex_array_obj, "WrapS"));
		if (!strcmp(wrap_s, "ClampToEdge"))
			tex[i]->WrapS = WRAP_MODE_CLAMP;
		else if (!strcmp(wrap_s, "Repeat"))
			tex[i]->WrapS = WRAP_MODE_REPEAT;
		else if (!strcmp(wrap_s, "MirroredRepeat"))
			tex[i]->WrapS = WRAP_MODE_MIRROR;
		else
			return NULL;

		// WrapT
		const char *wrap_t = json_object_get_string(json_object_object_get(tex_array_obj, "WrapT"));
		if (!strcmp(wrap_t, "ClampToEdge"))
			tex[i]->WrapT = WRAP_MODE_CLAMP;
		else if (!strcmp(wrap_t, "Repeat"))
			tex[i]->WrapT = WRAP_MODE_REPEAT;
		else if (!strcmp(wrap_t, "MirroredRepeat"))
			tex[i]->WrapT = WRAP_MODE_MIRROR;
		else
			return NULL;

		// PaletteFormat
		const char *palette_format = json_object_get_string(json_object_object_get(tex_array_obj, "PaletteFormat"));
		if (!strcmp(palette_format, "IA8"))
			tex[i]->PaletteFormat = P_IA8;
		else if (!strcmp(palette_format, "RGB565"))
			tex[i]->PaletteFormat = P_RGB565;
		else if (!strcmp(palette_format, "RGB5A3"))
			tex[i]->PaletteFormat = P_RGB5A3;
		else
			return NULL;

		// MipMap
		tex[i]->EnableMipmaps = json_object_get_int(json_object_object_get(tex_array_obj, "MipMap"));

		// EdgeLOD
		tex[i]->EnableEdgeLOD = json_object_get_boolean(json_object_object_get(tex_array_obj, "EdgeLOD"));

		// BiasClamp
		tex[i]->ClampLODBias = json_object_get_boolean(json_object_object_get(tex_array_obj, "BiasClamp"));

		// MaxAniso
		tex[i]->MaxAnisotropy = json_object_get_int(json_object_object_get(tex_array_obj, "MaxAniso"));

		// MinFilter
		const char *minification_filter = json_object_get_string(json_object_object_get(tex_array_obj, "MinFilter"));
		if (!strcmp(minification_filter, "Nearest"))
			tex[i]->MinificationFilter = TEX_FILTER_NEAR;
		else if (!strcmp(minification_filter, "Linear"))
			tex[i]->MinificationFilter = TEX_FILTER_LINEAR;
		else if (!strcmp(minification_filter, "NearestMipmapNearest"))
			tex[i]->MinificationFilter = TEX_FILTER_NEAR_MIP_NEAR;
		else if (!strcmp(minification_filter, "NearestMipmapLinear"))
			tex[i]->MinificationFilter = TEX_FILTER_NEAR_MIP_LIN;
		else if (!strcmp(minification_filter, "LinearMipmapNearest"))
			tex[i]->MinificationFilter = TEX_FILTER_LIN_MIP_NEAR;
		else if (!strcmp(minification_filter, "LinearMipmapLinear"))
			tex[i]->MinificationFilter = TEX_FILTER_LIN_MIP_LIN;
		else
			return NULL;

		// MagFilter
		const char *magnification_filter = json_object_get_string(json_object_object_get(tex_array_obj, "MagFilter"));
		if (!strcmp(magnification_filter, "Nearest"))
			tex[i]->MagnificationFilter = TEX_FILTER_NEAR;
		else if (!strcmp(magnification_filter, "Linear"))
			tex[i]->MagnificationFilter = TEX_FILTER_LINEAR;
		else
			return NULL;

		// MinLOD
		tex[i]->MinLOD = json_object_get_double(json_object_object_get(tex_array_obj, "MinLOD"));

		// MaxLOD
		tex[i]->MaxLOD = json_object_get_double(json_object_object_get(tex_array_obj, "MaxLOD"));

		// LodBias
		tex[i]->LODBias = json_object_get_double(json_object_object_get(tex_array_obj, "LodBias"));

		// TODO: read the texture image into memory
		// TODO: maybe some error checking
	}

	return tex;
}
