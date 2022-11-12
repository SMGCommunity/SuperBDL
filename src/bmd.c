#include <stdbool.h>
#include <stdio.h>

#include <assimp/scene.h>

#include "bmd.h"

bool write_bmd(const struct aiScene *model_data, FILE *output_fp, bool write_bdl, bool sm3das) {
	fwrite("J3D2", 4, 1, output_fp);

	if (write_bdl)
		fwrite("bdl4", 5, 1, output_fp);
	else
		fwrite("bmd3", 5, 1, output_fp);

	// true for success, false for error
	return true;
}
