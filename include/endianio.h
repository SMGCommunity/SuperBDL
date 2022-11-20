#ifndef __ENDIANIO_H
#define __ENDIANIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>

bool isMagicMatch(FILE* fp, const char* target);

int readReverseUint32(FILE* fp, unsigned int* output);
int readReverseUint16(FILE* fp, unsigned short* output);
int writeReverseUint32(FILE* fp, unsigned int* output);
int writeReverseUint16(FILE* fp, unsigned short* output);

#endif /* __ENDIANIO_H */