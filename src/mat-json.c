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
		mat[i]->Name = strdup(json_object_get_string(json_object_object_get(mat_array_obj, "Name")));

		// Flag
		mat[i]->MaterialMode = json_object_get_int(json_object_object_get(mat_array_obj, "Flag"));

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
		struct json_object *indtexentry_object = json_object_object_get(mat_array_obj, "IndTexEntry");

		struct json_object *tevorders_object = json_object_object_get(indtexentry_object, "TevOrders");
		size_t tevorders_object_length = json_object_array_length(tevorders_object);

		for (int j = 0; j < min(tevorders_object_length, 4); ++j) {
			mat[i]->IndirectStages[j] = malloc(sizeof (struct IndirectTextureStage));

			struct json_object *tevorders_array_item = json_object_array_get_idx(tevorders_object, j);

			const char *texcoord_str = json_object_get_string(json_object_object_get(tevorders_array_item, "TexCoord"));
			if (!strcmp(texcoord_str, "Null")) {
				mat[i]->IndirectStages[j]->TexCoordID = TEXTURE_NULL;
			} else if (!memcmp(texcoord_str, "TexCoord", 8)) {
				mat[i]->IndirectStages[j]->TexCoordID = texcoord_str[8] - '0';
			}

			const char *texmap_str = json_object_get_string(json_object_object_get(tevorders_array_item, "TexMap"));
			if (!strcmp(texmap_str, "Null")) {
				mat[i]->IndirectStages[j]->TexMapID = TEXTURE_NULL;
			} else if (!memcmp(texmap_str, "TexMap", 6)) {
				mat[i]->IndirectStages[j]->TexMapID = texmap_str[6] - '0';
			}
		}

		struct json_object *scale_object = json_object_object_get(indtexentry_object, "Scales");
		size_t scale_object_length = json_object_array_length(scale_object);

		for (int j = 0; j < min(scale_object_length, 4); ++j) {
			if (!mat[i]->IndirectStages[j])
				mat[i]->IndirectStages[j] = malloc(sizeof (struct IndirectTextureStage));

			struct json_object *scale_array_item = json_object_array_get_idx(scale_object, j);

			const char *scales_str = json_object_get_string(json_object_object_get(scale_array_item, "ScaleS"));
			if (!memcmp(scales_str, "ITS_", 4)) {
				scales_str += 4;
				if (!strcmp(scales_str, "1"))
					mat[i]->IndirectStages[j]->ScaleS = IND_SCALE_ONE;
				else if (!strcmp(scales_str, "2"))
					mat[i]->IndirectStages[j]->ScaleS = IND_SCALE_TWO;
				else if (!strcmp(scales_str, "4"))
					mat[i]->IndirectStages[j]->ScaleS = IND_SCALE_FOUR;
				else if (!strcmp(scales_str, "8"))
					mat[i]->IndirectStages[j]->ScaleS = IND_SCALE_EIGHT;
				else if (!strcmp(scales_str, "16"))
					mat[i]->IndirectStages[j]->ScaleS = IND_SCALE_SIXTEEN;
				else if (!strcmp(scales_str, "32"))
					mat[i]->IndirectStages[j]->ScaleS = IND_SCALE_THIRTY_TWO;
				else if (!strcmp(scales_str, "64"))
					mat[i]->IndirectStages[j]->ScaleS = IND_SCALE_SIXTY_FOUR;
				else if (!strcmp(scales_str, "128"))
					mat[i]->IndirectStages[j]->ScaleS = IND_SCALE_HUNDRED_TWENTY_EIGHT;
				else if (!strcmp(scales_str, "256"))
					mat[i]->IndirectStages[j]->ScaleS = IND_SCALE_TWO_HUNDRED_FIFTY_SIX;
			}

			const char *scalet_str = json_object_get_string(json_object_object_get(scale_array_item, "ScaleT"));
			if (!memcmp(scalet_str, "ITS_", 4)) {
				scalet_str += 4;
				if (!strcmp(scalet_str, "1"))
					mat[i]->IndirectStages[j]->ScaleT = IND_SCALE_ONE;
				else if (!strcmp(scalet_str, "2"))
					mat[i]->IndirectStages[j]->ScaleT = IND_SCALE_TWO;
				else if (!strcmp(scalet_str, "4"))
					mat[i]->IndirectStages[j]->ScaleT = IND_SCALE_FOUR;
				else if (!strcmp(scalet_str, "8"))
					mat[i]->IndirectStages[j]->ScaleT = IND_SCALE_EIGHT;
				else if (!strcmp(scalet_str, "16"))
					mat[i]->IndirectStages[j]->ScaleT = IND_SCALE_SIXTEEN;
				else if (!strcmp(scalet_str, "32"))
					mat[i]->IndirectStages[j]->ScaleT = IND_SCALE_THIRTY_TWO;
				else if (!strcmp(scalet_str, "64"))
					mat[i]->IndirectStages[j]->ScaleT = IND_SCALE_SIXTY_FOUR;
				else if (!strcmp(scalet_str, "128"))
					mat[i]->IndirectStages[j]->ScaleT = IND_SCALE_HUNDRED_TWENTY_EIGHT;
				else if (!strcmp(scalet_str, "256"))
					mat[i]->IndirectStages[j]->ScaleT = IND_SCALE_TWO_HUNDRED_FIFTY_SIX;
			}
		}

		struct json_object *ind_matrices = json_object_object_get(indtexentry_object, "Matrices");
		size_t ind_matrices_length = json_object_array_length(ind_matrices);
		for (int j = 0; j < min(ind_matrices_length, 3); ++j) {
			struct json_object *ind_matrices_array_item = json_object_array_get_idx(ind_matrices, j);

			mat[i]->IndirectTextureMatricies[j].values[0][3] = json_object_get_int(json_object_object_get(ind_matrices_array_item, "Exponent"));

			struct json_object *ind_matrices_matrix = json_object_object_get(ind_matrices_array_item, "Matrix");

			for (int k = 0; k < 2; ++k) {
				struct json_object *ind_matrices_matrix_item = json_object_array_get_idx(ind_matrices_matrix, k);
				for (int l = 0; l < 3; ++l)
					mat[i]->IndirectTextureMatricies[j].values[k][l] = json_object_get_double(json_object_array_get_idx(ind_matrices_matrix_item, l));
			}
		}

		// TevStages
		struct json_object *tev_stages_array = json_object_object_get(indtexentry_object, "TevStages");
		size_t tev_stages_array_length = json_object_array_length(tev_stages_array);
		for (int j = 0; j < min(tev_stages_array_length, 16); ++j) {
			mat[i]->TEVStages[j] = malloc(sizeof (struct TextureEnvironmentStage));

			struct json_object *tev_stages_array_item = json_object_array_get_idx(tev_stages_array, j);
			struct json_object *indirect_tex_format_str_obj = json_object_object_get(tev_stages_array_item, "IndTexFormat");

			const char *indirect_tex_format_str = json_object_get_string(indirect_tex_format_str_obj);
			if (!strcmp(indirect_tex_format_str, "ITF_8"))
				mat[i]->TEVStages[j]->IndirectTexFormat = IND_TEXFMT_EIGHT_BIT;
			else if (!strcmp(indirect_tex_format_str, "ITF_5"))
				mat[i]->TEVStages[j]->IndirectTexFormat = IND_TEXFMT_FIVE_BIT;
			else if (!strcmp(indirect_tex_format_str, "ITF_4"))
				mat[i]->TEVStages[j]->IndirectTexFormat = IND_TEXFMT_FOUR_BIT;
			else if (!strcmp(indirect_tex_format_str, "ITF_3"))
				mat[i]->TEVStages[j]->IndirectTexFormat = IND_TEXFMT_THREE_BIT;

			const char *indirect_tex_bias_str = json_object_get_string(json_object_object_get(tev_stages_array_item, "IndTexBiasSel"));
			if (!strcmp(indirect_tex_bias_str, "None"))
				mat[i]->TEVStages[j]->IndirectTexBias = IND_TEXBIAS_NONE;
			else if (!strcmp(indirect_tex_bias_str, "ITB_S"))
				mat[i]->TEVStages[j]->IndirectTexBias = IND_TEXBIAS_S;
			else if (!strcmp(indirect_tex_bias_str, "ITB_T"))
				mat[i]->TEVStages[j]->IndirectTexBias = IND_TEXBIAS_T;
			else if (!strcmp(indirect_tex_bias_str, "ITB_ST"))
				mat[i]->TEVStages[j]->IndirectTexBias = IND_TEXBIAS_ST;
			else if (!strcmp(indirect_tex_bias_str, "ITB_U"))
				mat[i]->TEVStages[j]->IndirectTexBias = IND_TEXBIAS_U;
			else if (!strcmp(indirect_tex_bias_str, "ITB_SU"))
				mat[i]->TEVStages[j]->IndirectTexBias = IND_TEXBIAS_SU;
			else if (!strcmp(indirect_tex_bias_str, "ITB_TU"))
				mat[i]->TEVStages[j]->IndirectTexBias = IND_TEXBIAS_TU;
			else if (!strcmp(indirect_tex_bias_str, "ITB_STU"))
				mat[i]->TEVStages[j]->IndirectTexBias = IND_TEXBIAS_STU;

			const char *indirect_tex_mtx_id_str = json_object_get_string(json_object_object_get(tev_stages_array_item, "IndTexMtxId"));
			if (!strcmp(indirect_tex_mtx_id_str, "ITM_OFF"))
				mat[i]->TEVStages[j]->IndirectTexMtxID = INDMTX_OFF;
			else if (!strcmp(indirect_tex_mtx_id_str, "ITM_0"))
				mat[i]->TEVStages[j]->IndirectTexMtxID = INDMTX_ZERO;
			else if (!strcmp(indirect_tex_mtx_id_str, "ITM_1"))
				mat[i]->TEVStages[j]->IndirectTexMtxID = INDMTX_ONE;
			else if (!strcmp(indirect_tex_mtx_id_str, "ITM_2"))
				mat[i]->TEVStages[j]->IndirectTexMtxID = INDMTX_TWO;
			else if (!strcmp(indirect_tex_mtx_id_str, "ITM_S0"))
				mat[i]->TEVStages[j]->IndirectTexMtxID = INDMTX_S_ZERO;
			else if (!strcmp(indirect_tex_mtx_id_str, "ITM_S1"))
				mat[i]->TEVStages[j]->IndirectTexMtxID = INDMTX_S_ONE;
			else if (!strcmp(indirect_tex_mtx_id_str, "ITM_S2"))
				mat[i]->TEVStages[j]->IndirectTexMtxID = INDMTX_S_TWO;
			else if (!strcmp(indirect_tex_mtx_id_str, "ITM_T0"))
				mat[i]->TEVStages[j]->IndirectTexMtxID = INDMTX_T_ZERO;
			else if (!strcmp(indirect_tex_mtx_id_str, "ITM_T1"))
				mat[i]->TEVStages[j]->IndirectTexMtxID = INDMTX_T_ONE;
			else if (!strcmp(indirect_tex_mtx_id_str, "ITM_T2"))
				mat[i]->TEVStages[j]->IndirectTexMtxID = INDMTX_T_TWO;

			const char *indirect_tex_wrap_s_str = json_object_get_string(json_object_object_get(tev_stages_array_item, "IndTexWrapS"));
			if (!strcmp(indirect_tex_wrap_s_str, "ITW_OFF"))
				mat[i]->TEVStages[j]->IndirectTexWrapS = INDWRAP_OFF;
			else if (!strcmp(indirect_tex_wrap_s_str, "ITW_256"))
				mat[i]->TEVStages[j]->IndirectTexWrapS = INDWRAP_TWO_HUNDRED_FIFTY_SIX;
			else if (!strcmp(indirect_tex_wrap_s_str, "ITW_128"))
				mat[i]->TEVStages[j]->IndirectTexWrapS = INDWRAP_HUNDRED_TWENTY_EIGHT;
			else if (!strcmp(indirect_tex_wrap_s_str, "ITW_64"))
				mat[i]->TEVStages[j]->IndirectTexWrapS = INDWRAP_SIXTY_FOUR;
			else if (!strcmp(indirect_tex_wrap_s_str, "ITW_32"))
				mat[i]->TEVStages[j]->IndirectTexWrapS = INDWRAP_THIRTY_TWO;
			else if (!strcmp(indirect_tex_wrap_s_str, "ITW_16"))
				mat[i]->TEVStages[j]->IndirectTexWrapS = INDWRAP_SIXTEEN;
			else if (!strcmp(indirect_tex_wrap_s_str, "ITW_0"))
				mat[i]->TEVStages[j]->IndirectTexWrapS = INDWRAP_ZERO;

			const char *indirect_tex_wrap_t_str = json_object_get_string(json_object_object_get(tev_stages_array_item, "IndTexWrapT"));
			if (!strcmp(indirect_tex_wrap_t_str, "ITW_OFF"))
				mat[i]->TEVStages[j]->IndirectTexWrapT = INDWRAP_OFF;
			else if (!strcmp(indirect_tex_wrap_t_str, "ITW_256"))
				mat[i]->TEVStages[j]->IndirectTexWrapT = INDWRAP_TWO_HUNDRED_FIFTY_SIX;
			else if (!strcmp(indirect_tex_wrap_t_str, "ITW_128"))
				mat[i]->TEVStages[j]->IndirectTexWrapT = INDWRAP_HUNDRED_TWENTY_EIGHT;
			else if (!strcmp(indirect_tex_wrap_t_str, "ITW_64"))
				mat[i]->TEVStages[j]->IndirectTexWrapT = INDWRAP_SIXTY_FOUR;
			else if (!strcmp(indirect_tex_wrap_t_str, "ITW_32"))
				mat[i]->TEVStages[j]->IndirectTexWrapT = INDWRAP_THIRTY_TWO;
			else if (!strcmp(indirect_tex_wrap_t_str, "ITW_16"))
				mat[i]->TEVStages[j]->IndirectTexWrapT = INDWRAP_SIXTEEN;
			else if (!strcmp(indirect_tex_wrap_t_str, "ITW_0"))
				mat[i]->TEVStages[j]->IndirectTexWrapT = INDWRAP_ZERO;

			mat[i]->TEVStages[j]->IndirectAddPrevious = json_object_get_boolean(json_object_object_get(tev_stages_array_item, "AddPrev"));
			mat[i]->TEVStages[j]->IndirectUseOriginalLoD = json_object_get_boolean(json_object_object_get(tev_stages_array_item, "UtcLod"));

			const char *indirect_tex_alpha_str = json_object_get_string(json_object_object_get(tev_stages_array_item, "AlphaSel"));
			if (!strcmp(indirect_tex_alpha_str, "ITBA_OFF"))
				mat[i]->TEVStages[j]->IndirectTexAlpha = IND_ALPHA_OFF;
			else if (!strcmp(indirect_tex_alpha_str, "ITBA_S"))
				mat[i]->TEVStages[j]->IndirectTexAlpha = IND_ALPHA_S;
			else if (!strcmp(indirect_tex_alpha_str, "ITBA_T"))
				mat[i]->TEVStages[j]->IndirectTexAlpha = IND_ALPHA_T;
			else if (!strcmp(indirect_tex_alpha_str, "ITBA_U"))
				mat[i]->TEVStages[j]->IndirectTexAlpha = IND_ALPHA_U;
		}

		// MaterialColors
		struct json_object *mat_colors_object = json_object_object_get(mat_array_obj, "MaterialColors");
	}

	return mat;
}
