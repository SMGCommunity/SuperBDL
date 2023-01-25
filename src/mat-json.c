#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <json.h>

#include "superbdl.h"
#include "mat3.h"
#include "tex1.h"
#include "mat-json.h"

static void parse_channel_control(struct ChannelControl *channel_controls, struct json_object *channel_controls_obj) {
	channel_controls->LightingEnabled = json_object_get_boolean(json_object_object_get(channel_controls_obj, "Enable"));

	const char *material_color_src_str = json_object_get_string(json_object_object_get(channel_controls_obj, "MaterialSrcColor"));
	if (!strcmp(material_color_src_str, "Vertex"))
		channel_controls->MaterialColorSource = SOURCE_VERTEX;
	else if (!strcmp(material_color_src_str, "Register"))
		channel_controls->MaterialColorSource = SOURCE_REGISTER;

	const char *light_mask_str = json_object_get_string(json_object_object_get(channel_controls_obj, "LitMask"));
	if (!strcmp(light_mask_str, "None"))
		channel_controls->LightMask = 0;
	else if (!memcmp(light_mask_str, "Light", 5))
		channel_controls->LightMask = pow(2, atoi(light_mask_str + 5));

	const char *diffuse_function_str = json_object_get_string(json_object_object_get(channel_controls_obj, "DiffuseFunction"));
	if (!strcmp(diffuse_function_str, "None"))
		channel_controls->DiffuseFunc = DIFFUSE_NONE;
	else if (!strcmp(diffuse_function_str, "Signed"))
		channel_controls->DiffuseFunc = DIFFUSE_SIGNED;
	else if (!strcmp(diffuse_function_str, "Clamp"))
		channel_controls->DiffuseFunc = DIFFUSE_CLAMPED;

	const char *attenuation_function_str = json_object_get_string(json_object_object_get(channel_controls_obj, "AttenuationFunction"));
	if (!strcmp(attenuation_function_str, "Spec"))
		channel_controls->AttenuationFunc = ATTENUATION_SPECULAR;
	else if (!strcmp(attenuation_function_str, "Spot"))
		channel_controls->AttenuationFunc = ATTENUATION_SPOTLIGHT;
	else if (!strcmp(attenuation_function_str, "None_0") || !strcmp(attenuation_function_str, "None_2"))
		channel_controls->AttenuationFunc = ATTENUATION_NONE;

	const char *ambient_src_color_str = json_object_get_string(json_object_object_get(channel_controls_obj, "AmbientSrcColor"));
	if (!strcmp(ambient_src_color_str, "Vertex"))
		channel_controls->AmbientColorSource = SOURCE_VERTEX;
	else if (!strcmp(ambient_src_color_str, "Register"))
		channel_controls->AmbientColorSource = SOURCE_REGISTER;
}

static void parse_tex_gen(struct TextureGenerator *tex_gen, struct json_object *tex_gen_obj) {
	const char *tex_gen_type_str = json_object_get_string(json_object_object_get(tex_gen_obj, "Type"));
	if (!strcmp(tex_gen_type_str, "Matrix3x4"))
		tex_gen->Type = TEXGENTYPE_MTX3x4;
	else if (!strcmp(tex_gen_type_str, "Matrix2x4"))
		tex_gen->Type = TEXGENTYPE_MTX2x4;
	else if (!strcmp(tex_gen_type_str, "Bump0"))
		tex_gen->Type = TEXGENTYPE_BUMP0;
	else if (!strcmp(tex_gen_type_str, "Bump1"))
		tex_gen->Type = TEXGENTYPE_BUMP1;
	else if (!strcmp(tex_gen_type_str, "Bump2"))
		tex_gen->Type = TEXGENTYPE_BUMP2;
	else if (!strcmp(tex_gen_type_str, "Bump3"))
		tex_gen->Type = TEXGENTYPE_BUMP3;
	else if (!strcmp(tex_gen_type_str, "Bump4"))
		tex_gen->Type = TEXGENTYPE_BUMP4;
	else if (!strcmp(tex_gen_type_str, "Bump5"))
		tex_gen->Type = TEXGENTYPE_BUMP5;
	else if (!strcmp(tex_gen_type_str, "Bump6"))
		tex_gen->Type = TEXGENTYPE_BUMP6;
	else if (!strcmp(tex_gen_type_str, "Bump7"))
		tex_gen->Type = TEXGENTYPE_BUMP7;
	else if (!strcmp(tex_gen_type_str, "SRTG"))
		tex_gen->Type = TEXGENTYPE_SRTG;

	const char *tex_gen_src_str = json_object_get_string(json_object_object_get(tex_gen_obj, "Source"));
	if (!strcmp(tex_gen_src_str, "Position"))
		tex_gen->Source = TEXGENSRC_POSITION;
	else if (!strcmp(tex_gen_src_str, "Normal"))
		tex_gen->Source = TEXGENSRC_NORMAL;
	else if (!strcmp(tex_gen_src_str, "Binormal"))
		tex_gen->Source = TEXGENSRC_BINORMAL;
	else if (!strcmp(tex_gen_src_str, "Tangent"))
		tex_gen->Source = TEXGENSRC_TANGENT;
	else if (!strcmp(tex_gen_src_str, "Tex0"))
		tex_gen->Source = TEXGENSRC_TEXTURE0;
	else if (!strcmp(tex_gen_src_str, "Tex1"))
		tex_gen->Source = TEXGENSRC_TEXTURE1;
	else if (!strcmp(tex_gen_src_str, "Tex2"))
		tex_gen->Source = TEXGENSRC_TEXTURE2;
	else if (!strcmp(tex_gen_src_str, "Tex3"))
		tex_gen->Source = TEXGENSRC_TEXTURE3;
	else if (!strcmp(tex_gen_src_str, "Tex4"))
		tex_gen->Source = TEXGENSRC_TEXTURE4;
	else if (!strcmp(tex_gen_src_str, "Tex5"))
		tex_gen->Source = TEXGENSRC_TEXTURE5;
	else if (!strcmp(tex_gen_src_str, "Tex6"))
		tex_gen->Source = TEXGENSRC_TEXTURE6;
	else if (!strcmp(tex_gen_src_str, "Tex7"))
		tex_gen->Source = TEXGENSRC_TEXTURE7;
	else if (!strcmp(tex_gen_src_str, "TexCoord0"))
		tex_gen->Source = TEXGENSRC_TEXCOORD0;
	else if (!strcmp(tex_gen_src_str, "TexCoord1"))
		tex_gen->Source = TEXGENSRC_TEXCOORD1;
	else if (!strcmp(tex_gen_src_str, "TexCoord2"))
		tex_gen->Source = TEXGENSRC_TEXCOORD2;
	else if (!strcmp(tex_gen_src_str, "TexCoord3"))
		tex_gen->Source = TEXGENSRC_TEXCOORD3;
	else if (!strcmp(tex_gen_src_str, "TexCoord4"))
		tex_gen->Source = TEXGENSRC_TEXCOORD4;
	else if (!strcmp(tex_gen_src_str, "TexCoord5"))
		tex_gen->Source = TEXGENSRC_TEXCOORD5;
	else if (!strcmp(tex_gen_src_str, "TexCoord6"))
		tex_gen->Source = TEXGENSRC_TEXCOORD6;
	else if (!strcmp(tex_gen_src_str, "Color0"))
		tex_gen->Source = TEXGENSRC_COLOR0;
	else if (!strcmp(tex_gen_src_str, "Color1"))
		tex_gen->Source = TEXGENSRC_COLOR1;

	const char *tex_gen_matrix_src_str = json_object_get_string(json_object_object_get(tex_gen_obj, "TexMatrixSource"));
	if (!strcmp(tex_gen_matrix_src_str, "Identity"))
		tex_gen->Matrix = TEXGENMTX_IDENTITY;
	else if (!strcmp(tex_gen_matrix_src_str, "TexMtx0"))
		tex_gen->Matrix = TEXGENMTX_TEXTUREMATRIX0;
	else if (!strcmp(tex_gen_matrix_src_str, "TexMtx1"))
		tex_gen->Matrix = TEXGENMTX_TEXTUREMATRIX1;
	else if (!strcmp(tex_gen_matrix_src_str, "TexMtx2"))
		tex_gen->Matrix = TEXGENMTX_TEXTUREMATRIX2;
	else if (!strcmp(tex_gen_matrix_src_str, "TexMtx3"))
		tex_gen->Matrix = TEXGENMTX_TEXTUREMATRIX3;
	else if (!strcmp(tex_gen_matrix_src_str, "TexMtx4"))
		tex_gen->Matrix = TEXGENMTX_TEXTUREMATRIX4;
	else if (!strcmp(tex_gen_matrix_src_str, "TexMtx5"))
		tex_gen->Matrix = TEXGENMTX_TEXTUREMATRIX5;
	else if (!strcmp(tex_gen_matrix_src_str, "TexMtx6"))
		tex_gen->Matrix = TEXGENMTX_TEXTUREMATRIX6;
	else if (!strcmp(tex_gen_matrix_src_str, "TexMtx7"))
		tex_gen->Matrix = TEXGENMTX_TEXTUREMATRIX7;
	else if (!strcmp(tex_gen_matrix_src_str, "TexMtx8"))
		tex_gen->Matrix = TEXGENMTX_TEXTUREMATRIX8;
	else if (!strcmp(tex_gen_matrix_src_str, "TexMtx9"))
		tex_gen->Matrix = TEXGENMTX_TEXTUREMATRIX9;
}

static void parse_tex_matrix(struct TextureMatrix *tex_matrix, struct json_object *tex_matrix_obj) {
	const char *projection_str = json_object_get_string(json_object_object_get(tex_matrix_obj, "Projection"));
	if (!strcmp(projection_str, "Matrix3x4"))
		tex_matrix->MatrixType = TEXMTXPROJ_MTX3x4;
	else if (!strcmp(projection_str, "Matrix2x4"))
		tex_matrix->MatrixType = TEXMTXPROJ_MTX2x4;

	tex_matrix->Mode = json_object_get_int(json_object_object_get(tex_matrix_obj, "Type"));

	struct json_object *tex_matrix_effect_translation_array = json_object_object_get(tex_matrix_obj, "EffectTranslation");
	size_t tex_matrix_effect_translation_length = json_object_array_length(tex_matrix_effect_translation_array);
	for (int i = 0; i < min(tex_matrix_effect_translation_length, 3); ++i)
		tex_matrix->Center.values[i] = json_object_get_double(json_object_array_get_idx(tex_matrix_effect_translation_array, i));

	struct json_object *tex_matrix_scale_array = json_object_object_get(tex_matrix_obj, "Scale");
	size_t tex_matrix_scale_length = json_object_array_length(tex_matrix_scale_array);
	for (int i = 0; i < min(tex_matrix_scale_length, 2); ++i)
		tex_matrix->Scale.values[i] = json_object_get_double(json_object_array_get_idx(tex_matrix_scale_array, i));

	tex_matrix->Rotation = json_object_get_double(json_object_object_get(tex_matrix_obj, "Rotation"));

	struct json_object *tex_matrix_translation_array = json_object_object_get(tex_matrix_obj, "Translation");
	size_t tex_matrix_translation_length = json_object_array_length(tex_matrix_translation_array);
	for (int i = 0; i < min(tex_matrix_translation_length, 2); ++i)
		tex_matrix->Translation.values[i] = json_object_get_double(json_object_array_get_idx(tex_matrix_translation_array, i));

	struct json_object *tex_matrix_projection_matrix_array = json_object_object_get(tex_matrix_obj, "ProjectionMatrix");
	size_t tex_matrix_projection_matrix_length = json_object_array_length(tex_matrix_projection_matrix_array);
	for (int i = 0; i < min(tex_matrix_projection_matrix_length, 4); ++i) {
		struct json_object *tex_matrix_projection_matrix_sub_array = json_object_array_get_idx(tex_matrix_projection_matrix_array, i);
		size_t tex_matrix_projection_matrix_sub_length = json_object_array_length(tex_matrix_projection_matrix_sub_array);
		for (int j = 0; j < min(tex_matrix_projection_matrix_sub_length, 4); ++j)
			tex_matrix->ProjectionMatrix.values[i][j] = json_object_get_double(json_object_array_get_idx(tex_matrix_projection_matrix_sub_array, j));
	}
}

static void parse_combine_color(enum GXCombineColor *combine_color, const char *combine_color_str) {
	if (!strcmp(combine_color_str, "ColorPrev"))
		*combine_color = COMBINECOLOR_COLORPREVIOUS;
	else if (!strcmp(combine_color_str, "AlphaPrev"))
		*combine_color = COMBINECOLOR_ALPHAPREVIOUS;
	else if (!strcmp(combine_color_str, "C0"))
		*combine_color = COMBINECOLOR_COLORREGISTER0;
	else if (!strcmp(combine_color_str, "A0"))
		*combine_color = COMBINECOLOR_ALPHAREGISTER0;
	else if (!strcmp(combine_color_str, "C1"))
		*combine_color = COMBINECOLOR_COLORREGISTER1;
	else if (!strcmp(combine_color_str, "A1"))
		*combine_color = COMBINECOLOR_ALPHAREGISTER1;
	else if (!strcmp(combine_color_str, "C2"))
		*combine_color = COMBINECOLOR_COLORREGISTER2;
	else if (!strcmp(combine_color_str, "A2"))
		*combine_color = COMBINECOLOR_ALPHAREGISTER2;
	else if (!strcmp(combine_color_str, "TexColor"))
		*combine_color = COMBINECOLOR_TEXTURECOLOR;
	else if (!strcmp(combine_color_str, "TexAlpha"))
		*combine_color = COMBINECOLOR_TEXTUREALPHA;
	else if (!strcmp(combine_color_str, "RasColor"))
		*combine_color = COMBINECOLOR_RASTERIZERCOLOR;
	else if (!strcmp(combine_color_str, "RasAlpha"))
		*combine_color = COMBINECOLOR_RASTERIZERALPHA;
	else if (!strcmp(combine_color_str, "One"))
		*combine_color = COMBINECOLOR_ONE;
	else if (!strcmp(combine_color_str, "Half"))
		*combine_color = COMBINECOLOR_HALF;
	else if (!strcmp(combine_color_str, "Konst"))
		*combine_color = COMBINECOLOR_CONSTANTSELECTION;
	else if (!strcmp(combine_color_str, "Zero"))
		*combine_color = COMBINECOLOR_ZERO;
}

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

		struct json_object *indtexentry_tevorders_array = json_object_object_get(indtexentry_object, "TevOrders");
		size_t indtexentry_tevorders_array_length = json_object_array_length(indtexentry_tevorders_array);

		for (int j = 0; j < min(indtexentry_tevorders_array_length, 4); ++j) {
			mat[i]->IndirectStages[j] = malloc(sizeof (struct IndirectTextureStage));

			struct json_object *indtexentry_tevorders_array_item = json_object_array_get_idx(indtexentry_tevorders_array, j);

			const char *indtexentry_texcoord_str = json_object_get_string(json_object_object_get(indtexentry_tevorders_array_item, "TexCoord"));
			if (!strcmp(indtexentry_texcoord_str, "Null"))
				mat[i]->IndirectStages[j]->TexCoordID = TEXTURE_NULL;
			else if (!memcmp(indtexentry_texcoord_str, "TexCoord", 8))
				mat[i]->IndirectStages[j]->TexCoordID = indtexentry_texcoord_str[8] - '0';

			const char *indtexentry_texmap_str = json_object_get_string(json_object_object_get(indtexentry_tevorders_array_item, "TexMap"));
			if (!strcmp(indtexentry_texmap_str, "Null"))
				mat[i]->IndirectStages[j]->TexMapID = TEXTURE_NULL;
			else if (!memcmp(indtexentry_texmap_str, "TexMap", 6))
				mat[i]->IndirectStages[j]->TexMapID = indtexentry_texmap_str[6] - '0';
		}

		struct json_object *indtexentry_scale_array = json_object_object_get(indtexentry_object, "Scales");
		size_t indtexentry_scale_array_length = json_object_array_length(indtexentry_scale_array);

		for (int j = 0; j < min(indtexentry_scale_array_length, 4); ++j) {
			if (!mat[i]->IndirectStages[j])
				mat[i]->IndirectStages[j] = malloc(sizeof (struct IndirectTextureStage));

			struct json_object *indtexentry_scale_array_item = json_object_array_get_idx(indtexentry_scale_array, j);

			const char *scales_str = json_object_get_string(json_object_object_get(indtexentry_scale_array_item, "ScaleS"));
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

			const char *scalet_str = json_object_get_string(json_object_object_get(indtexentry_scale_array_item, "ScaleT"));
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

		struct json_object *indtexentry_ind_matrices = json_object_object_get(indtexentry_object, "Matrices");
		size_t indtexentry_ind_matrices_length = json_object_array_length(indtexentry_ind_matrices);
		for (int j = 0; j < min(indtexentry_ind_matrices_length, 3); ++j) {
			struct json_object *indtexentry_ind_matrices_array_item = json_object_array_get_idx(indtexentry_ind_matrices, j);

			mat[i]->IndirectTextureMatricies[j].m03 = json_object_get_int(json_object_object_get(indtexentry_ind_matrices_array_item, "Exponent"));

			struct json_object *indtexentry_ind_matrices_matrix = json_object_object_get(indtexentry_ind_matrices_array_item, "Matrix");

			for (int k = 0; k < 2; ++k) {
				struct json_object *indtexentry_ind_matrices_matrix_item = json_object_array_get_idx(indtexentry_ind_matrices_matrix, k);
				for (int l = 0; l < 3; ++l)
					mat[i]->IndirectTextureMatricies[j].values[k][l] = json_object_get_double(json_object_array_get_idx(indtexentry_ind_matrices_matrix_item, l));
			}
		}

		// TevStages
		struct json_object *indtexentry_tev_stages_array = json_object_object_get(indtexentry_object, "TevStages");
		size_t indtexentry_tev_stages_array_length = json_object_array_length(indtexentry_tev_stages_array);
		for (int j = 0; j < min(indtexentry_tev_stages_array_length, 16); ++j) {
			mat[i]->TEVStages[j] = malloc(sizeof (struct TextureEnvironmentStage));

			struct json_object *indtexentry_tev_stages_array_item = json_object_array_get_idx(indtexentry_tev_stages_array, j);
			struct json_object *indirect_tex_format_str_obj = json_object_object_get(indtexentry_tev_stages_array_item, "IndTexFormat");

			const char *indirect_tex_format_str = json_object_get_string(indirect_tex_format_str_obj);
			if (!strcmp(indirect_tex_format_str, "ITF_8"))
				mat[i]->TEVStages[j]->IndirectTexFormat = IND_TEXFMT_EIGHT_BIT;
			else if (!strcmp(indirect_tex_format_str, "ITF_5"))
				mat[i]->TEVStages[j]->IndirectTexFormat = IND_TEXFMT_FIVE_BIT;
			else if (!strcmp(indirect_tex_format_str, "ITF_4"))
				mat[i]->TEVStages[j]->IndirectTexFormat = IND_TEXFMT_FOUR_BIT;
			else if (!strcmp(indirect_tex_format_str, "ITF_3"))
				mat[i]->TEVStages[j]->IndirectTexFormat = IND_TEXFMT_THREE_BIT;

			const char *indirect_tex_bias_str = json_object_get_string(json_object_object_get(indtexentry_tev_stages_array_item, "IndTexBiasSel"));
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

			const char *indirect_tex_mtx_id_str = json_object_get_string(json_object_object_get(indtexentry_tev_stages_array_item, "IndTexMtxId"));
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

			const char *indirect_tex_wrap_s_str = json_object_get_string(json_object_object_get(indtexentry_tev_stages_array_item, "IndTexWrapS"));
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

			const char *indirect_tex_wrap_t_str = json_object_get_string(json_object_object_get(indtexentry_tev_stages_array_item, "IndTexWrapT"));
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

			mat[i]->TEVStages[j]->IndirectAddPrevious = json_object_get_boolean(json_object_object_get(indtexentry_tev_stages_array_item, "AddPrev"));
			mat[i]->TEVStages[j]->IndirectUseOriginalLoD = json_object_get_boolean(json_object_object_get(indtexentry_tev_stages_array_item, "UtcLod"));

			const char *indirect_tex_alpha_str = json_object_get_string(json_object_object_get(indtexentry_tev_stages_array_item, "AlphaSel"));
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
		size_t mat_colors_length = json_object_array_length(mat_colors_object);

		for (int j = 0; j < min(mat_colors_length, 2); ++j) {
			struct json_object *mat_colors_array_item = json_object_array_get_idx(mat_colors_object, j);

			mat[i]->ColorMatRegs[j].r = json_object_get_double(json_object_object_get(mat_colors_array_item, "R")) * 255;
			mat[i]->ColorMatRegs[j].g = json_object_get_double(json_object_object_get(mat_colors_array_item, "G")) * 255;
			mat[i]->ColorMatRegs[j].b = json_object_get_double(json_object_object_get(mat_colors_array_item, "B")) * 255;
			mat[i]->ColorMatRegs[j].a = json_object_get_double(json_object_object_get(mat_colors_array_item, "A")) * 255;
		}

		// ChannelControls
		struct json_object *channel_controls_array = json_object_object_get(mat_array_obj, "ChannelControls");

		struct json_object *channel_controls_item_one = json_object_array_get_idx(channel_controls_array, 0);
		if (!channel_controls_item_one)
			goto channel_controls_done;
		mat[i]->LightChannels[0] = calloc(1, sizeof (struct LightChannelControl));
		mat[i]->LightChannels[0]->Color = calloc(1, sizeof (struct ChannelControl));
		parse_channel_control(mat[i]->LightChannels[0]->Color, channel_controls_item_one);

		struct json_object *channel_controls_item_two = json_object_array_get_idx(channel_controls_array, 1);
		if (!channel_controls_item_two)
			goto channel_controls_done;
		mat[i]->LightChannels[0]->Alpha = calloc(1, sizeof (struct ChannelControl));
		parse_channel_control(mat[i]->LightChannels[0]->Alpha, channel_controls_item_two);

		struct json_object *channel_controls_item_three = json_object_array_get_idx(channel_controls_array, 2);
		if (!channel_controls_item_three)
			goto channel_controls_done;
		mat[i]->LightChannels[1] = calloc(1, sizeof (struct LightChannelControl));
		mat[i]->LightChannels[1]->Color = calloc(1, sizeof (struct ChannelControl));
		parse_channel_control(mat[i]->LightChannels[1]->Color, channel_controls_item_three);

		struct json_object *channel_controls_item_four = json_object_array_get_idx(channel_controls_array, 3);
		if (!channel_controls_item_four)
			goto channel_controls_done;
		mat[i]->LightChannels[1]->Alpha = calloc(1, sizeof (struct ChannelControl));
		parse_channel_control(mat[i]->LightChannels[1]->Alpha, channel_controls_item_four);
channel_controls_done:

		// AmbientColors
		struct json_object *ambient_colors_array = json_object_object_get(mat_array_obj, "AmbientColors");
		size_t ambient_colors_length = json_object_array_length(ambient_colors_array);

		for (int j = 0; j < min(ambient_colors_length, 2); ++j) {
			struct json_object *ambient_colors_array_item = json_object_array_get_idx(ambient_colors_array, j);

			mat[i]->ColorAmbRegs[j].r = json_object_get_double(json_object_object_get(ambient_colors_array_item, "R")) * 255;
			mat[i]->ColorAmbRegs[j].g = json_object_get_double(json_object_object_get(ambient_colors_array_item, "G")) * 255;
			mat[i]->ColorAmbRegs[j].b = json_object_get_double(json_object_object_get(ambient_colors_array_item, "B")) * 255;
			mat[i]->ColorAmbRegs[j].a = json_object_get_double(json_object_object_get(ambient_colors_array_item, "A")) * 255;
		}

		// TexCoord1Gens
		struct json_object *tex_gen_array = json_object_object_get(mat_array_obj, "TexCoord1Gens");
		size_t tex_gen_length = json_object_array_length(tex_gen_array);

		for (int j = 0; j < min(tex_gen_length, 8); ++j) {
			struct json_object *tex_gen_item = json_object_array_get_idx(tex_gen_array, j);
			if (tex_gen_item) {
				mat[i]->TextureGenerators[j] = calloc(1, sizeof (struct TextureGenerator));
				parse_tex_gen(mat[i]->TextureGenerators[j], tex_gen_item);
			}
		}

		// PostTexCoordGens
		struct json_object *post_tex_gen_array = json_object_object_get(mat_array_obj, "PostTexCoordGens");
		size_t post_tex_gen_length = json_object_array_length(post_tex_gen_array);

		for (int j = 0; j < min(post_tex_gen_length, 8); ++j) {
			struct json_object *post_tex_gen_item = json_object_array_get_idx(post_tex_gen_array, j);
			if (post_tex_gen_item) {
				mat[i]->PostTextureGenerators[j] = calloc(1, sizeof (struct TextureGenerator));
				parse_tex_gen(mat[i]->PostTextureGenerators[j], post_tex_gen_item);
			}
		}

		// TexMatrix1
		struct json_object *tex_matrix_array = json_object_object_get(mat_array_obj, "TexMatrix1");
		size_t tex_matrix_length = json_object_array_length(tex_matrix_array);

		for (int j = 0; j < min(tex_matrix_length, 10); ++j) {
			struct json_object *tex_matrix_item = json_object_array_get_idx(tex_matrix_array, j);
			if (tex_matrix_item) {
				mat[i]->TextureMatricies[j] = calloc(1, sizeof (struct TextureMatrix));
				parse_tex_matrix(mat[i]->TextureMatricies[j], tex_matrix_item);
			}
		}

		// PostTexMatrix
		struct json_object *post_tex_matrix_array = json_object_object_get(mat_array_obj, "PostTexMatrix");
		size_t post_tex_matrix_length = json_object_array_length(post_tex_matrix_array);

		for (int j = 0; j < min(post_tex_matrix_length, 20); ++j) {
			struct json_object *post_tex_matrix_item = json_object_array_get_idx(post_tex_matrix_array, j);
			if (post_tex_matrix_item) {
				mat[i]->TextureMatricies[j] = calloc(1, sizeof (struct TextureMatrix));
				parse_tex_matrix(mat[i]->TextureMatricies[j], post_tex_matrix_item);
			}
		}

		struct json_object *tevorders_array = json_object_object_get(mat_array_obj, "TevOrders");
		size_t tevorders_array_length = json_object_array_length(tevorders_array);

		for (int j = 0; j < min(tevorders_array_length, 16); ++j) {
			struct json_object *tevorders_array_item = json_object_array_get_idx(tevorders_array, j);

			if (!tevorders_array_item)
				continue;

			if (!mat[i]->TEVStages[j])
				mat[i]->TEVStages[j] = malloc(sizeof (struct TextureEnvironmentStage));

			const char *texcoord_str = json_object_get_string(json_object_object_get(tevorders_array_item, "TexCoord"));
			if (!strcmp(texcoord_str, "Null"))
				mat[i]->TEVStages[j]->TextureCoordID = TEXTURE_NULL;
			else if (!memcmp(texcoord_str, "TexCoord", 8))
				mat[i]->TEVStages[j]->TextureCoordID = texcoord_str[8] - '0';

			const char *texmap_str = json_object_get_string(json_object_object_get(tevorders_array_item, "TexMap"));
			if (!strcmp(texmap_str, "Null"))
				mat[i]->TEVStages[j]->TextureMapID = TEXTURE_NULL;
			else if (!memcmp(texmap_str, "TexMap", 6))
				mat[i]->TEVStages[j]->TextureMapID = texmap_str[6] - '0';

			const char *channel_id_str = json_object_get_string(json_object_object_get(tevorders_array_item, "ChannelId"));
			if (!strcmp(channel_id_str, "Color0"))
				mat[i]->TEVStages[j]->ChannelID = RASTERIZER_COLOR0;
			else if (!strcmp(channel_id_str, "Color1"))
				mat[i]->TEVStages[j]->ChannelID = RASTERIZER_COLOR1;
			else if (!strcmp(channel_id_str, "Alpha0"))
				mat[i]->TEVStages[j]->ChannelID = RASTERIZER_ALPHA0;
			else if (!strcmp(channel_id_str, "Alpha1"))
				mat[i]->TEVStages[j]->ChannelID = RASTERIZER_ALPHA1;
			else if (!strcmp(channel_id_str, "Color0A0"))
				mat[i]->TEVStages[j]->ChannelID = RASTERIZER_COLOR0A0;
			else if (!strcmp(channel_id_str, "Color1A1"))
				mat[i]->TEVStages[j]->ChannelID = RASTERIZER_COLOR1A1;
			else if (!strcmp(channel_id_str, "ColorZero"))
				mat[i]->TEVStages[j]->ChannelID = RASTERIZER_COLOR_ZERO;
			else if (!strcmp(channel_id_str, "AlphaBump"))
				mat[i]->TEVStages[j]->ChannelID = RASTERIZER_ALPHA_BUMP;
			else if (!strcmp(channel_id_str, "AlphaBumpN"))
				mat[i]->TEVStages[j]->ChannelID = RASTERIZER_ALPHA_BUMPN;
			else if (!strcmp(channel_id_str, "ColorNull"))
				mat[i]->TEVStages[j]->ChannelID = RASTERIZER_COLOR_NULL;
		}

		// ColorSels
		struct json_object *color_sels_array = json_object_object_get(mat_array_obj, "ColorSels");
		size_t color_sels_length = json_object_array_length(color_sels_array);

		for (int j = 0; j < min(color_sels_length, 16); ++j) {
			struct json_object *color_sels_array_item = json_object_array_get_idx(color_sels_array, j);

			if (!color_sels_array_item)
				continue;

			if (!mat[i]->TEVStages[j])
				mat[i]->TEVStages[j] = malloc(sizeof (struct TextureEnvironmentStage));

			const char *color_sels_array_str = json_object_get_string(color_sels_array_item);
			if (!strcmp(color_sels_array_str, "KCSel_1"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_ONE;
			else if (!strcmp(color_sels_array_str, "KCSel_7_8"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_SEVEN_EIGHTHS;
			else if (!strcmp(color_sels_array_str, "KCSel_3_4"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_SIX_EIGHTHS;
			else if (!strcmp(color_sels_array_str, "KCSel_5_8"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_FIVE_EIGHTHS;
			else if (!strcmp(color_sels_array_str, "KCSel_1_2"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_FOUR_EIGHTHS;
			else if (!strcmp(color_sels_array_str, "KCSel_3_8"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_THREE_EIGHTHS;
			else if (!strcmp(color_sels_array_str, "KCSel_1_4"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_TWO_EIGHTHS;
			else if (!strcmp(color_sels_array_str, "KCSel_1_8"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_ONE_EIGHTH;
			else if (!strcmp(color_sels_array_str, "KCSel_K0_R"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_RRR0;
			else if (!strcmp(color_sels_array_str, "KCSel_K1_R"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_RRR1;
			else if (!strcmp(color_sels_array_str, "KCSel_K2_R"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_RRR2;
			else if (!strcmp(color_sels_array_str, "KCSel_K3_R"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_RRR3;
			else if (!strcmp(color_sels_array_str, "KCSel_K0_G"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_GGG0;
			else if (!strcmp(color_sels_array_str, "KCSel_K1_G"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_GGG1;
			else if (!strcmp(color_sels_array_str, "KCSel_K2_G"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_GGG2;
			else if (!strcmp(color_sels_array_str, "KCSel_K3_G"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_GGG3;
			else if (!strcmp(color_sels_array_str, "KCSel_K0_B"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_BBB0;
			else if (!strcmp(color_sels_array_str, "KCSel_K1_B"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_BBB1;
			else if (!strcmp(color_sels_array_str, "KCSel_K2_B"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_BBB2;
			else if (!strcmp(color_sels_array_str, "KCSel_K3_B"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_BBB3;
			else if (!strcmp(color_sels_array_str, "KCSel_K0_A"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_AAA0;
			else if (!strcmp(color_sels_array_str, "KCSel_K1_A"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_AAA1;
			else if (!strcmp(color_sels_array_str, "KCSel_K2_A"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_AAA2;
			else if (!strcmp(color_sels_array_str, "KCSel_K3_A"))
				mat[i]->TEVStages[j]->ConstantColorSelection = CONST_COLOR_AAA3;
		}

		// AlphaSels
		struct json_object *alpha_sels_array = json_object_object_get(mat_array_obj, "AlphaSels");
		size_t alpha_sels_length = json_object_array_length(alpha_sels_array);

		for (int j = 0; j < min(alpha_sels_length, 16); ++j) {
			struct json_object *alpha_sels_array_item = json_object_array_get_idx(alpha_sels_array, j);

			if (!alpha_sels_array_item)
				continue;

			if (!mat[i]->TEVStages[j])
				mat[i]->TEVStages[j] = malloc(sizeof (struct TextureEnvironmentStage));

			const char *alpha_sels_array_str = json_object_get_string(alpha_sels_array_item);
			if (!strcmp(alpha_sels_array_str, "KASel_1"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_ONE;
			else if (!strcmp(alpha_sels_array_str, "KASel_7_8"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_SEVEN_EIGHTHS;
			else if (!strcmp(alpha_sels_array_str, "KASel_3_4"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_SIX_EIGHTHS;
			else if (!strcmp(alpha_sels_array_str, "KASel_5_8"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_FIVE_EIGHTHS;
			else if (!strcmp(alpha_sels_array_str, "KASel_1_2"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_FOUR_EIGHTHS;
			else if (!strcmp(alpha_sels_array_str, "KASel_3_8"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_THREE_EIGHTHS;
			else if (!strcmp(alpha_sels_array_str, "KASel_1_4"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_TWO_EIGHTHS;
			else if (!strcmp(alpha_sels_array_str, "KASel_1_8"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_ONE_EIGHTH;
			else if (!strcmp(alpha_sels_array_str, "KASel_K0_R"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_R0;
			else if (!strcmp(alpha_sels_array_str, "KASel_K1_R"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_R1;
			else if (!strcmp(alpha_sels_array_str, "KASel_K2_R"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_R2;
			else if (!strcmp(alpha_sels_array_str, "KASel_K3_R"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_R3;
			else if (!strcmp(alpha_sels_array_str, "KASel_K0_G"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_G0;
			else if (!strcmp(alpha_sels_array_str, "KASel_K1_G"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_G1;
			else if (!strcmp(alpha_sels_array_str, "KASel_K2_G"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_G2;
			else if (!strcmp(alpha_sels_array_str, "KASel_K3_G"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_G3;
			else if (!strcmp(alpha_sels_array_str, "KASel_K0_B"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_B0;
			else if (!strcmp(alpha_sels_array_str, "KASel_K1_B"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_B1;
			else if (!strcmp(alpha_sels_array_str, "KASel_K2_B"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_B2;
			else if (!strcmp(alpha_sels_array_str, "KASel_K3_B"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_B3;
			else if (!strcmp(alpha_sels_array_str, "KASel_K0_A"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_A0;
			else if (!strcmp(alpha_sels_array_str, "KASel_K1_A"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_A1;
			else if (!strcmp(alpha_sels_array_str, "KASel_K2_A"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_A2;
			else if (!strcmp(alpha_sels_array_str, "KASel_K3_A"))
				mat[i]->TEVStages[j]->ConstantAlphaSelection = CONST_ALPHA_A3;
		}

		// TevColors
		struct json_object *tev_colors_array = json_object_object_get(mat_array_obj, "TevColors");
		size_t tev_colors_length = json_object_array_length(tev_colors_array);

		for (int j = 0; j < min(tev_colors_length, 4); ++j) {
			struct json_object *tev_colors_array_item = json_object_array_get_idx(tev_colors_array, j);

			mat[i]->ColorRegisters[j].r = json_object_get_double(json_object_object_get(tev_colors_array_item, "R")) * 255;
			mat[i]->ColorRegisters[j].g = json_object_get_double(json_object_object_get(tev_colors_array_item, "G")) * 255;
			mat[i]->ColorRegisters[j].b = json_object_get_double(json_object_object_get(tev_colors_array_item, "B")) * 255;
			mat[i]->ColorRegisters[j].a = json_object_get_double(json_object_object_get(tev_colors_array_item, "A")) * 255;
		}

		// KonstColors
		struct json_object *konst_colors_array = json_object_object_get(mat_array_obj, "KonstColors");
		size_t konst_colors_length = json_object_array_length(konst_colors_array);

		for (int j = 0; j < min(konst_colors_length, 4); ++j) {
			struct json_object *konst_colors_array_item = json_object_array_get_idx(konst_colors_array, j);

			mat[i]->ColorConstants[j].r = json_object_get_double(json_object_object_get(konst_colors_array_item, "R")) * 255;
			mat[i]->ColorConstants[j].g = json_object_get_double(json_object_object_get(konst_colors_array_item, "G")) * 255;
			mat[i]->ColorConstants[j].b = json_object_get_double(json_object_object_get(konst_colors_array_item, "B")) * 255;
			mat[i]->ColorConstants[j].a = json_object_get_double(json_object_object_get(konst_colors_array_item, "A")) * 255;
		}

		// TevStages
		struct json_object *tev_stages_array = json_object_object_get(mat_array_obj, "TevStages");
		size_t tev_stages_array_length = json_object_array_length(tev_stages_array);
		for (int j = 0; j < min(tev_stages_array_length, 16); ++j) {
			struct json_object *tev_stages_array_item = json_object_array_get_idx(tev_stages_array, j);

			if (!tev_stages_array_item)
				continue;

			if (!mat[i]->TEVStages[j])
				mat[i]->TEVStages[j] = malloc(sizeof (struct TextureEnvironmentStage));

			parse_combine_color(&mat[i]->TEVStages[j]->ColorInA, json_object_get_string(json_object_object_get(tev_stages_array_item, "ColorInA")));
			parse_combine_color(&mat[i]->TEVStages[j]->ColorInB, json_object_get_string(json_object_object_get(tev_stages_array_item, "ColorInB")));
			parse_combine_color(&mat[i]->TEVStages[j]->ColorInC, json_object_get_string(json_object_object_get(tev_stages_array_item, "ColorInC")));
			parse_combine_color(&mat[i]->TEVStages[j]->ColorInD, json_object_get_string(json_object_object_get(tev_stages_array_item, "ColorInD")));

			const char *color_op_str = json_object_get_string(json_object_object_get(tev_stages_array_item, "ColorOp"));
			if (!strcmp(color_op_str, "Add"))
				mat[i]->TEVStages[j]->ColorOperation = OPERATION_ADD;
			else if (!strcmp(color_op_str, "Sub"))
				mat[i]->TEVStages[j]->ColorOperation = OPERATION_SUB;
			else if (!strcmp(color_op_str, "Comp_R8_GT"))
				mat[i]->TEVStages[j]->ColorOperation = OPERATION_COMP_R8_GT;
			else if (!strcmp(color_op_str, "Comp_R8_EQ"))
				mat[i]->TEVStages[j]->ColorOperation = OPERATION_COMP_R8_EQ;
			else if (!strcmp(color_op_str, "Comp_GR16_GT"))
				mat[i]->TEVStages[j]->ColorOperation = OPERATION_COMP_GR16_GT;
			else if (!strcmp(color_op_str, "Comp_GR16_EQ"))
				mat[i]->TEVStages[j]->ColorOperation = OPERATION_COMP_GR16_EQ;
			else if (!strcmp(color_op_str, "Comp_BGR24_GT"))
				mat[i]->TEVStages[j]->ColorOperation = OPERATION_COMP_BGR24_GT;
			else if (!strcmp(color_op_str, "Comp_BGR24_EQ"))
				mat[i]->TEVStages[j]->ColorOperation = OPERATION_COMP_BGR24_EQ;
			else if (!strcmp(color_op_str, "Comp_RGB8_GT") || !strcmp(color_op_str, "Comp_A8_GT"))
				mat[i]->TEVStages[j]->ColorOperation = OPERATION_COMP_RGB8_GT;
			else if (!strcmp(color_op_str, "Comp_RGB8_EQ") || !strcmp(color_op_str, "Comp_A8_EQ"))
				mat[i]->TEVStages[j]->ColorOperation = OPERATION_COMP_RGB8_EQ;

			const char *color_bias_str = json_object_get_string(json_object_object_get(tev_stages_array_item, "ColorBias"));
			if (!strcmp(color_bias_str, "Zero"))
				mat[i]->TEVStages[j]->ColorBias = BIAS_ZERO;
			else if (!strcmp(color_bias_str, "AddHalf"))
				mat[i]->TEVStages[j]->ColorBias = BIAS_ADDHALF;
			else if (!strcmp(color_bias_str, "SubHalf"))
				mat[i]->TEVStages[j]->ColorBias = BIAS_SUBHALF;

			const char *color_scale_str = json_object_get_string(json_object_object_get(tev_stages_array_item, "ColorScale"));
			if (!strcmp(color_scale_str, "Scale_1"))
				mat[i]->TEVStages[j]->ColorScale = SCALE_SCALE_1;
			else if (!strcmp(color_scale_str, "Scale_2"))
				mat[i]->TEVStages[j]->ColorScale = SCALE_SCALE_2;
			else if (!strcmp(color_scale_str, "Scale_4"))
				mat[i]->TEVStages[j]->ColorScale = SCALE_SCALE_4;
			else if (!strcmp(color_scale_str, "Divide_2"))
				mat[i]->TEVStages[j]->ColorScale = SCALE_DIVIDE_2;
		}
	}

	return mat;
}
