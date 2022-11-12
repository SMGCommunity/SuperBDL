#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <assimp/cimport.h>

#include "bmd.h"

void show_help(void) {
	puts("placeholder help text");
}

int main(int argc, char **argv) {
	bool output_bdl = true;
	bool sm3das = false;
	const char *input_path = NULL;
	const char *output_path = NULL;

	bool parsing_args = true;

	// there's a nice POSIX function for this but it's not available on Windows
	for (int i = 1; i < argc; ++i) {
		if (parsing_args) {
			if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
				show_help();
				return 0;
			} else if (!strcmp(argv[i], "--bmd") || !strcmp(argv[i], "-b")) {
				output_bdl = false;
			} else if (!strcmp(argv[i], "--bdl") || !strcmp(argv[i], "-B")) {
				output_bdl = true;
			} else if (!strcmp(argv[i], "--sm3das")) {
				output_bdl = true; // implies --bdl
				sm3das = true;
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

	const struct aiScene *model_data = aiImportFile(input_path, 0);

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
