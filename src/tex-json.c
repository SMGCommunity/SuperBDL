#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <json.h>

#include "tex1.h"
#include "tex-json.h"

bool read_tex_json(FILE *fp, struct JUTTexture *output) {
	struct json_object *tex_obj = NULL;

	fseek(fp, 0, SEEK_END);
	size_t tex_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *tex = calloc(1, tex_size + 1);

	fread(tex, tex_size, 1, fp);
	fclose(fp);

	tex_obj = json_tokener_parse(tex);
	free(tex);

	if (!tex_obj)
		return false;

	struct json_object* name_obj = json_object_object_get(tex_obj, "Name");

	/*if (!name_obj)
		return false;*/

	const char *name = json_object_get_string(name_obj);
	printf("Name: %s\n", name);

	return true;
}
