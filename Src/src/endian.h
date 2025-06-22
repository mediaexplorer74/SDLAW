/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 */

#ifndef __SYS_H__
#define __SYS_H__

#include <stdint.h>

//FCS added for windows build
#ifdef _WIN32
	#define SYS_LITTLE_ENDIAN
#endif

#if defined SYS_LITTLE_ENDIAN

inline uint16_t READ_BE_UINT16(const void *ptr) {
	const uint8_t *b = (const uint8_t *)ptr;
	return (b[0] << 8) | b[1];
}

inline uint32_t READ_BE_UINT32(const void *ptr) {
	const uint8_t *b = (const uint8_t *)ptr;
	return (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
}

#elif defined SYS_BIG_ENDIAN

inline uint16_t READ_BE_UINT16(const void *ptr) {
	return *(const uint16_t *)ptr;
}

inline uint32_t READ_BE_UINT32(const void *ptr) {
	return *(const uint32_t *)ptr;
}

#else

#error No endianness defined

#endif

#endif
