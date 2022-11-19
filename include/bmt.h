#ifndef __BMT_H
#define __BMT_H

#include <stdbool.h>
#include <stdio.h>

#include <mat3.h>
#include <tex1.h>

bool readBMT(FILE* fp, struct BMT* output);

struct J3DMaterial* getMaterial(struct BMT* bmt, const char* name);
struct JUTTexture* getTexture(struct BMT* bmt, const char* name);


struct BMT
{
	/// <summary>
	/// An array of materials inside this BMT
	/// </summary>
	struct J3DMaterial* Materials;
	/// <summary>
	/// An array of textures inside this BMT
	/// </summary>
	struct JUTTexture* Textures;
};

#endif /* __BMT_H */
