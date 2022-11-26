#ifndef __BMT_H
#define __BMT_H

#include <stdbool.h>
#include <stdio.h>

#include "mat3.h"
#include "tex1.h"

#define MAGIC_BMT "bmt3"

struct BMT
{
	/// <summary>
	/// An array of materials inside this BMT
	/// </summary>
	struct J3DMaterial** Materials;
	unsigned int MaterialCount;
	/// <summary>
	/// An array of textures inside this BMT
	/// </summary>
	struct JUTTexture** Textures;
	unsigned int TextureCount;
};

/// <summary>
/// Reads a Binary Material File
/// </summary>
/// <param name="fp">- File pointer</param>
/// <param name="output">- Destination <see cref="struct BMT"/> pointer</param>
/// <returns>true if successful</returns>
bool readBMT(FILE* fp, struct BMT* output);

/// <summary>
/// Gets a material based on it's name
/// </summary>
/// <returns>NULL if the material is not found</returns>
struct J3DMaterial* getMaterial(struct BMT* bmt, const char* name);
/// <summary>
/// Gets a texture based on it's name
/// </summary>
/// <returns>NULL if the texture is not found</returns>
struct JUTTexture* getTexture(struct BMT* bmt, const char* name);

#endif /* __BMT_H */
