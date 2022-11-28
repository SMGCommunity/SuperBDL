#ifndef __SUPERBDL_H
#define __SUPERBDL_H

#include <assert.h>

#if defined(_WIN32) || defined(_WIN64)
#define assert _ASSERT
#endif

#define MAX_STRING_ALLOCATION 256
#define RETURN_VOID_IF_NULL(var) if((var) == NULL) return;
#define RETURN_NULL_IF_NULL(var) if((var) == NULL) return NULL;
#define RETURN_FALSE_IF_NULL(var) if((var) == NULL) return false;
#define RETURN_FALSE_IF_FALSE(var) if(!(var)) return false;

#endif
