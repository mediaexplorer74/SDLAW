// SDLAW, a port/conversion of the DOS game Another World.
// MediaExplorer 2023

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef _MSC_VER // unistd.h does not exist in the Windows SDK.
#include <unistd.h>
#else
#ifndef _UNISTD_H
#define _UNISTD_H    1
#define F_OK    0       /* Test for existence.  */
#define access _access
#endif
#endif

// S_ISREG and S_ISDIR may not be defined under MSVC
#ifndef S_ISREG
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif
#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif

#include "config.h"
#include "types.h"
#include "proto.h"
#include "data.h"


#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef ABS
#define ABS(x) ((x)<0?-(x):(x))
#endif

#define snprintf_check(dst, size, ...)	do {			\
		int __len;					\
		__len = snprintf(dst, size, __VA_ARGS__);	\
		if (__len < 0 || __len >= size) {		\
			fprintf(stderr, "%s: buffer truncation detected!\n", __func__);\
			quit(2);				\
		}						\
	} while (0)

#ifdef __cplusplus
}
#endif

#endif
