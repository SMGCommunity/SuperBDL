#ifndef __ENDIANIO_H
#define __ENDIANIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
// Windows only runs on little-endian hosts
#define	htobe16(x)	_byteswap_ushort((x))
#define	htobe32(x)	_byteswap_ulong((x))
#define	htole16(x)	(x)
#define	htole32(x)	(x)

#define	be16toh(x)	_byteswap_ushort((x))
#define	be32toh(x)	_byteswap_ulong((x))
#define	le16toh(x)	(x)
#define	le32toh(x)	(x)
#else
#include <endian.h>
#endif

bool isMagicMatch(FILE* fp, const char* target);

int readReverseUint32(FILE* fp, unsigned int* output);
int readReverseUint16(FILE* fp, unsigned short* output);
int writeReverseUint32(FILE* fp, unsigned int* output);
int writeReverseUint16(FILE* fp, unsigned short* output);

#endif /* __ENDIANIO_H */
