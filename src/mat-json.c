#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <json.h>

#include "superbdl.h"
#include "mat3.h"
#include "tex1.h"
#include "mat-json.h"

struct J3DMaterial **read_mat_json(FILE *fp, struct JUTTexture **tex) {
	fseek(fp, 0, SEEK_END);
	size_t mat_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *mat_str = calloc(1, mat_size + 1);

	fread(mat_str, mat_size, 1, fp);
	fclose(fp);

	struct json_object *mat_obj = json_tokener_parse(mat_str);
	free(mat_str);

	if (!mat_obj)
		return NULL;

	size_t array_size = json_object_array_length(mat_obj);

	struct J3DMaterial **mat = calloc(array_size + 1, sizeof (struct J3DMaterial *));

	for (int i = 0; i < array_size; ++i) {
		struct json_object *mat_array_obj = json_object_array_get_idx(mat_obj, i);
		if (!mat_array_obj)
			return NULL;

		mat[i] = calloc(1, sizeof (struct J3DMaterial));

		// Name
		mat[i]->Name = json_object_get_string(json_object_object_get(mat_array_obj, "Name"));

		// Flag

		// CullMode
		const char *cull_mode = json_object_get_string(json_object_object_get(mat_array_obj, "CullMode"));
		if (!strcmp(cull_mode, "None"))
			mat[i]->Culling = CULL_NONE;
		else if (!strcmp(cull_mode, "Front"))
			mat[i]->Culling = CULL_FRONT;
		else if (!strcmp(cull_mode, "Back"))
			mat[i]->Culling = CULL_BACK;
		else if (!strcmp(cull_mode, "All"))
			mat[i]->Culling = CULL_BOTH;

		// ZCompLoc
		mat[i]->BlendInfo = calloc(1, sizeof (struct Blend));
		mat[i]->BlendInfo->TestDepthBeforeTexture = json_object_get_boolean(json_object_object_get(mat_array_obj, "ZCompLoc"));

		// Dither
		mat[i]->EnableDithering = json_object_get_boolean(json_object_object_get(mat_array_obj, "Dither"));

		// TextureNames
		struct json_object *texture_array = json_object_object_get(mat_array_obj, "TextureNames");
		size_t texture_array_size = json_object_array_length(texture_array);
		for (int j = 0; j < min(texture_array_size, 8); ++j) {
			struct json_object *texture_array_item = json_object_array_get_idx(texture_array, j);

			const char *texture_name_orig = json_object_get_string(texture_array_item);
			if (!*texture_name_orig)
				continue;

			char *texture_name = strdup(texture_name_orig);

			char *colon = strchr(texture_name, ':');
			if (colon)
				*colon = '\0';

			for (int k = 0; tex[k] != NULL; ++k) {
				if (!strcmp(tex[k]->Name, texture_name)) {
					mat[i]->Textures[j] = tex[k];
					break;
				}
			}

			free(texture_name);
		}

		// IndTexEntry
	}

	return mat;
}
