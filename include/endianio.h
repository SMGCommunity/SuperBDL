#ifndef __ENDIANIO_H
#define __ENDIANIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#define bswap_16(x) _byteswap_ushort((x))
#define bswap_32(x) _byteswap_ulong((x))

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
#include <byteswap.h>
#include <endian.h>
#endif

bool isMagicMatch(FILE* fp, const char* target);
char** readStringTable(FILE* fp);

size_t fread_e(void* _Buffer, size_t ElementSize, size_t ElementCount, FILE* _Stream);
size_t fwrite_e(void* _Buffer, size_t ElementSize, size_t ElementCount, FILE* _Stream);

#endif /* __ENDIANIO_H */
