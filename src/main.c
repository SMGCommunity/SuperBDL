#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#ifdef _MSC_VER
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

#include <assimp/scene.h>
#include <assimp/cimport.h>

#include "bmd.h"

#include "tex1.h"
#include "tex-json.h"

#include "mat3.h"
#include "mat-json.h"

#define VERSION "1.0"

static void show_help(void) {
	puts("placeholder help text");
}

int main(int argc, char **argv) {
	bool format_forced = false;
	bool output_bdl = true;
	bool sm3das = false;
	bool optimize = false; //Optimizing the model is very much a thing that we could look into doing...
	const char *input_path = NULL;
	const char *output_path = NULL;
	const char *mat_path = NULL;
	const char *tex_path = NULL;

	bool parsing_args = true;

	// there's a nice POSIX function for this but it's not available on Windows
	for (int i = 1; i < argc; ++i) {
		if (parsing_args) {
			if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v")) {
				puts("SuperBDL v" VERSION);
				return 0;
			} else if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
				show_help();
				return 0;
			} else if (!strcmp(argv[i], "--bmd") || !strcmp(argv[i], "-b")) {
				format_forced = true;
				output_bdl = false;
			} else if (!strcmp(argv[i], "--bdl") || !strcmp(argv[i], "-B")) {
				format_forced = true;
				output_bdl = true;
			} else if (!strcmp(argv[i], "--sm3das")) {
				output_bdl = true; // implies --bdl
				sm3das = true;
			} else if (!strcmp(argv[i], "--optimize") || !strcmp(argv[i], "-O") || !strcmp(argv[i], "-O2")) {
				optimize = true;
			} else if (!strcmp(argv[i], "--mat") || !strcmp(argv[i], "-m")) {
				mat_path = argv[++i];
			} else if (!strcmp(argv[i], "--tex") || !strcmp(argv[i], "-t")) {
				tex_path = argv[++i];
			} else if (!strcmp(argv[i], "--")) {
				parsing_args = false;
			} else if (argv[i][0] == '-') {
				fputs("Invalid argument\n", stderr);
				show_help();
				return 1;
			} else {
				--i;
				parsing_args = false;
			}
		} else {
			if (!input_path)
				input_path = argv[i];
			else
				output_path = argv[i];
		}
	}

	if (!input_path) {
		fputs("Missing input and output path\n", stderr);
		show_help();
		return 1;
	}

	if (!output_path) {
		fputs("Missing output path\n", stderr);
		show_help();
		return 1;
	}

	if (!format_forced) {
		size_t output_len = strlen(output_path);

		if (output_len >= 4 && !strcasecmp(output_path + output_len - 4, ".bmd"))
			output_bdl = false;
	}

	struct JUTTexture **tex = NULL;
	if (tex_path) {
		FILE *tex_fp = fopen(tex_path, "r");

		if (!tex_fp) {
			fprintf(stderr, "Failed to open %s: %s\n", tex_path, strerror(errno));
			return 1;
		}

		tex = read_tex_json(tex_fp);
		if (!tex) {
			fprintf(stderr, "Failed to read texture json\n");
			return 1;
		}
	}

	struct J3DMaterial **mat = NULL;
	if (tex_path && mat_path) {
		FILE *mat_fp = fopen(mat_path, "r");

		if (!mat_fp) {
			fprintf(stderr, "Failed to open %s: %s\n", mat_path, strerror(errno));
			return 1;
		}

		mat = read_mat_json(mat_fp, tex);
		if (!mat) {
			fprintf(stderr, "Failed to read material json\n");
			return 1;
		}
	}

	const struct aiScene *model_data = aiImportFile(input_path, 0);

	if (!model_data) {
		printf("Failed to parse input model: %s\n", aiGetErrorString());
		return 1;
	}

	FILE *output_fp = fopen(output_path, "w");
	if (!output_fp) {
		fprintf(stderr, "Failed to open %s: %s\n", output_path, strerror(errno));
		return 1;
	}

	if (!write_bmd(model_data, output_fp, output_bdl, sm3das)) {
		// failed
		return 1;
	}

	fclose(output_fp);

	return 0;
}
