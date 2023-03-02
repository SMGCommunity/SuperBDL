#ifndef __ENDIANIO_H
#define __ENDIANIO_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#define bswap_16(x) _byteswap_ushort((x))
#define bswap_32(x) _byteswap_ulong((x))

// Windows only runs on little-endian hosts
#define LITTLE_ENDIAN	1234
#define BIG_ENDIAN	4321
#define BYTE_ORDER	LITTLE_ENDIAN

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

extern bool byte_swap;

bool isMagicMatch(FILE* fp, const char* target);
char** readStringTable(FILE* fp);

extern size_t fread_e(void* _Buffer, size_t ElementSize, size_t ElementCount, FILE* _Stream);
extern size_t fwrite_e(const void* _Buffer, size_t ElementSize, size_t ElementCount, FILE* _Stream);

static inline void tryByteSwap16(void *v) {
	uint16_t *vp = v;

	if (byte_swap)
		*vp = bswap_16(*vp);
}
static inline void tryByteSwap32(void *v) {
	uint32_t *vp = v;

	if (byte_swap)
		*vp = bswap_32(*vp);
}
#endif /* __ENDIANIO_H */
