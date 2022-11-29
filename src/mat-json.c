#include <stdbool.h>
#include <stdlib.h>

#include <json.h>

#include "mat3.h"
#include "mat-json.h"

bool read_mat_json(FILE *fp, struct J3DMaterial *output) {
	struct json_object *mat_obj = NULL;

	fseek(fp, 0, SEEK_END);
	size_t mat_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *mat = malloc(mat_size);

	fread(mat, mat_size, 1, fp);
	fclose(fp);

	mat_obj = json_tokener_parse(mat);
	free(mat);

	return true;
}
