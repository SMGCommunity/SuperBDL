#ifndef __SUPERBDL_H
#define __SUPERBDL_H

#define VERSION "1.0"

#if !defined(_WIN32) && !defined(_WIN64)
#include <sys/param.h>
#define min MIN
#endif

#include <ctype.h>

#include "bmd.h"
#include "bmt.h"

#define MAX_STRING_ALLOCATION 256

#define RETURN_VOID_IF_NULL(var) if((var) == NULL) return;
#define RETURN_NULL_IF_NULL(var) if((var) == NULL) return NULL;
#define RETURN_FALSE_IF_NULL(var) if((var) == NULL) return false;
#define RETURN_FALSE_IF_FALSE(var) if(!(var)) return false;

#define COUNT_ELEMENTS_NTARRAY(destvar, arrayvar) \
	while(*arrayvar != NULL)\
	{\
		destvar++;\
		arrayvar++;\
	}\

struct InputArgsHolder
{
	bool format_forced;
	bool output_bdl;
	enum J3DImportOptimizations optimizationFlags;
	const char* input_path;
	const char* output_path;
	//string[]*
	char** mat_path;
	int mat_num;
	//string[]*
	char** tex_path;
	int tex_num;
};

//These have i* in case we need to edit i.

bool tryCalcOptimizations(int* i, char** argv, struct InputArgsHolder* ArgHolder);
bool tryReadMatInput(int* i, char** argv, struct InputArgsHolder* ArgHolder);
bool tryReadTexInput(int* i, char** argv, struct InputArgsHolder* ArgHolder);
bool tryReadMultiInput(int* i, char** argv, char** resultPtr, int* elementNum, const char* keyword, const char* shortkeyword);

// Return 1 if str is a number, 0 otherwise.
static inline bool isNumeric(const char* str) {
	while (*str)
	{
		if (!isdigit(*str))
			return false;
		++str;
	}
	return true;
}
static inline const char* get_filename_ext(const char* filename)
{
	const char* dot = strrchr(filename, '.');
	if (!dot || dot == filename)
		return "";
	return dot;
}

bool tryLoadMaterial(char* filepath, struct BMT* InternalCollection);
bool tryLoadMaterialJson(char* filepath, struct BMT* InternalCollection);
bool tryLoadMaterialBmt(char* filepath, struct BMT* InternalCollection);

bool tryLoadTexture(char* filepath, struct BMT* InternalCollection);
bool tryLoadTextureJson(char* filepath, struct BMT* InternalCollection);
bool tryLoadTextureBmt(char* filepath, struct BMT* InternalCollection);


#endif
