#include <stdbool.h>
#include <stdio.h>

#include <assimp/scene.h>

#include "bmd.h"

bool write_bmd(const struct aiScene *model_data, FILE *output_fp, bool write_bdl, bool sm3das) {
	fwrite(MAGIC_J3D, 4, 1, output_fp);

	if (write_bdl)
		fwrite(MAGIC_BDL, 5, 1, output_fp);
	else
		fwrite(MAGIC_BMD, 5, 1, output_fp);

	// true for success, false for error
	return true;
}
