/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 */

#ifndef __BANK_H__
#define __BANK_H__

#include "intern.h"

struct MemEntry;

struct UnpackContext {
	uint16_t size;
	uint32_t crc;
	uint32_t chk;
	int32_t datasize;
};

struct Bank {
	UnpackContext _unpCtx;
	const char *_dataDir;
	uint8_t *_iBuf, *_oBuf, *_startBuf;

	Bank(const char *dataDir);

	bool read(const MemEntry *me, uint8_t *buf);
	void decUnk1(uint8_t numChunks, uint8_t addCount);
	void decUnk2(uint8_t numChunks);
	bool unpack();
	uint16_t getCode(uint8_t numChunks);
	bool nextChunk();
	bool rcr(bool CF);
};

#endif
