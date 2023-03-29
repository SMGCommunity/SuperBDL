#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

#include <assimp/scene.h>
#include <assimp/cimport.h>

#include "superbdl.h"

#include "bmd.h"

#include "endianio.h"

#include "tex1.h"
#include "tex-json.h"

#include "mat3.h"
#include "mat-json.h"

static void show_help(void) {
	puts("placeholder help text");
}

bool tryCalcOptimizations(int* i, char** argv, struct InputArgsHolder* ArgHolder)
{
	//TODO!
	return true;
}

bool tryReadMatInput(int* i, char** argv, struct InputArgsHolder* ArgHolder)
{
	return tryReadMultiInput(i, argv, ArgHolder->tex_path, &(ArgHolder->tex_num), "--mat", "-m");
}
bool tryReadTexInput(int* i, char** argv, struct InputArgsHolder* ArgHolder)
{
	return tryReadMultiInput(i, argv, ArgHolder->mat_path, &(ArgHolder->mat_num), "--mat", "-m");
}
bool tryReadMultiInput(int* i, char** argv, char** resultPtr, int* elementNum, const char* keyword, const char* shortkeyword)
{
	if (!strcmp(argv[*i], keyword) && !strcmp(argv[*i], shortkeyword))
		return false;

	++*i; //Skip to the number(?) arg

	long EntryCount = 1;
	if (isNumeric(argv[*i]))
	{
		long value = strtol(argv[*i], NULL, 10);
		EntryCount = value;
		++*i; //Advance to the inputs
	}

	//Reallocate the array
	int CurrentNum = *elementNum;
	*elementNum += EntryCount;
	resultPtr = realloc(resultPtr, sizeof(char*) * *elementNum);

	for (size_t j = CurrentNum; j < *elementNum; j++)
	{
		resultPtr[j] = argv[*i];
		++*i;
	}
	return true;
}








int main(int argc, char **argv) {
	struct InputArgsHolder ArgHolder =
	{
		.format_forced = false,
		.output_bdl = true,
		.optimizationFlags = OPTIMIZE_NONE,
		.input_path = NULL,
		.output_path = NULL,
		.mat_path = malloc(sizeof(char*)),
		.mat_num = 0,
		.tex_path = malloc(sizeof(char*)),
		.tex_num = 0
	};

	// there's a nice POSIX function for this but it's not available on Windows
	for (int i = 1; i < argc; ++i)
	{
		if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v"))
		{
			puts("SuperBDL v" VERSION);
			return 0;
		}

		if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h"))
		{
			show_help();
			return 0;
		}
			
		if (!strcmp(argv[i], "--bmd") || !strcmp(argv[i], "-b"))
		{
			ArgHolder.format_forced = true;
			ArgHolder.output_bdl = false;
			continue;
		}

		if (!strcmp(argv[i], "--bdl") || !strcmp(argv[i], "-B"))
		{
			ArgHolder.format_forced = true;
			ArgHolder.output_bdl = true;
			continue;
		}

		if (!strcmp(argv[i], "--sm3das"))
		{
			byte_swap = BYTE_ORDER != LITTLE_ENDIAN;
			continue;
		}
			
		if (tryReadMatInput(&i, argv, &ArgHolder))
			continue;

		if (tryReadTexInput(&i, argv, &ArgHolder))
			continue;

		if (!tryCalcOptimizations(&argc, argv, &ArgHolder))
			continue;

		if (argv[i][0] == '-')
		{
			fputs("Invalid argument\n", stderr);
			show_help();
			return 1;
		}
			
		if (!ArgHolder.input_path)
			ArgHolder.input_path = argv[i];
		else
			ArgHolder.output_path = argv[i];
	}

	if (!ArgHolder.input_path)
	{
		fputs("Missing input path\n", stderr);
		show_help();
		return 1;
	}

	if (!ArgHolder.output_path)
	{
		fputs("Missing output path\n", stderr);
		show_help();
		return 2;
	}

	if (!ArgHolder.format_forced)
	{
		size_t output_len = strlen(ArgHolder.output_path);

		if (output_len >= 4 && !strcasecmp(ArgHolder.output_path + output_len - 4, ".bmd"))
			ArgHolder.output_bdl = false;
	}

	//struct JUTTexture **tex = NULL;
	//if (tex_path) {
	//	FILE *tex_fp = fopen(tex_path, "r");

	//	if (!tex_fp) {
	//		fprintf(stderr, "Failed to open %s: %s\n", tex_path, strerror(errno));
	//		return 1;
	//	}

	//	tex = read_tex_json(tex_fp);
	//	if (!tex) {
	//		fprintf(stderr, "Failed to read texture json\n");
	//		return 1;
	//	}
	//}

	//struct J3DMaterial **mat = NULL;
	//if (tex_path && mat_path) {
	//	FILE *mat_fp = fopen(mat_path, "r");

	//	if (!mat_fp) {
	//		fprintf(stderr, "Failed to open %s: %s\n", mat_path, strerror(errno));
	//		return 1;
	//	}

	//	mat = read_mat_json(mat_fp, tex);
	//	if (!mat) {
	//		fprintf(stderr, "Failed to read material json\n");
	//		return 1;
	//	}
	//}

	const struct aiScene *model_data = aiImportFile(ArgHolder.input_path, 0);

	if (!model_data) {
		printf("Failed to parse input model: %s\n", aiGetErrorString());
		return 1;
	}

	FILE *output_fp = fopen(ArgHolder.output_path, "w");
	if (!output_fp) {
		fprintf(stderr, "Failed to open %s: %s\n", ArgHolder.output_path, strerror(errno));
		return 1;
	}

	if (!write_bmd(model_data, output_fp, ArgHolder.output_bdl)) {
		// failed
		return 1;
	}

	fclose(output_fp);

	return 0;
}
