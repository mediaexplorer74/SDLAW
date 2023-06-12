/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 */

#ifndef __UTIL_H__
#define __UTIL_H__

#include "intern.h"

enum {
	DBG_VM = 1 << 0,
	DBG_BANK  = 1 << 1,
	DBG_VIDEO = 1 << 2,
	DBG_SND   = 1 << 3,
	DBG_SER   = 1 << 4,
	DBG_INFO  = 1 << 5,
	DBG_RES   = 1 << 6
};

extern uint16_t g_debugMask;

extern void debug(uint16_t cm, const char *msg, ...);
extern void error(const char *msg, ...);
extern void warning(const char *msg, ...);

extern void string_lower(char *p);
extern void string_upper(char *p);

#endif
