#ifndef __BMT_H
#define __BMT_H

#include <stdbool.h>
#include <stdio.h>

#include "mat3.h"
#include "tex1.h"

#define MAGIC_BMT "bmt3"

/// <summary>
/// Reads a Binary Material File
/// </summary>
/// <param name="fp">- File pointer</param>
/// <param name="output">- Destination <see cref="struct BMT"/> pointer</param>
/// <returns>true if successful</returns>
bool readBMT(FILE* fp, struct BMT* output);

/// <summary>
/// 
/// </summary>
struct J3DMaterial* getMaterial(struct BMT* bmt, const char* name);
/// <summary>
/// 
/// </summary>
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
