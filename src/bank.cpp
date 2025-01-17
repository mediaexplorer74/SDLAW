/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 */

#include "bank.h"
#include "file.h"
#include "resource.h"


Bank::Bank(const char *dataDir)
	: _dataDir(dataDir) {
}

bool Bank::read(const MemEntry *me, uint8_t *buf) {

	bool ret = false;
	char bankName[10];
	sprintf(bankName, "bank%02x", me->bankId);
	File f;

	if (!f.open(bankName, _dataDir))
	{
		error("Bank::read() unable to open '%s'", bankName);
	}

	
	f.seek(me->bankOffset);

	// Depending if the resource is packed or not we
	// can read directly or unpack it.
	if (me->packedSize == me->size) {
		f.read(buf, me->packedSize);
		ret = true;
	} else {
		f.read(buf, me->packedSize);
		_startBuf = buf;
		_iBuf = buf + me->packedSize - 4;
		ret = unpack();
	}
	
	return ret;
}

void Bank::decUnk1(uint8_t numChunks, uint8_t addCount) 
{
	uint16_t count = getCode(numChunks) + addCount + 1;
	debug(DBG_BANK, "Bank::decUnk1(%d, %d) count=%d", numChunks, addCount, count);
	_unpCtx.datasize -= count;
	while (count--) {
		assert(_oBuf >= _iBuf && _oBuf >= _startBuf);
		*_oBuf = (uint8_t)getCode(8);
		--_oBuf;
	}
}

/*
   Note from fab: This look like run-length encoding.
*/
void Bank::decUnk2(uint8_t numChunks) {
	uint16_t i = getCode(numChunks);
	uint16_t count = _unpCtx.size + 1;
	debug(DBG_BANK, "Bank::decUnk2(%d) i=%d count=%d", numChunks, i, count);
	_unpCtx.datasize -= count;
	while (count--) {
		assert(_oBuf >= _iBuf && _oBuf >= _startBuf);
		*_oBuf = *(_oBuf + i);
		--_oBuf;
	}
}

/*
	Most resource in the banks are compacted.
*/
bool Bank::unpack() {
	_unpCtx.size = 0;
	_unpCtx.datasize = READ_BE_UINT32(_iBuf); _iBuf -= 4;
	_oBuf = _startBuf + _unpCtx.datasize - 1;
	_unpCtx.crc = READ_BE_UINT32(_iBuf); _iBuf -= 4;
	_unpCtx.chk = READ_BE_UINT32(_iBuf); _iBuf -= 4;
	_unpCtx.crc ^= _unpCtx.chk;
	do {
		if (!nextChunk()) {
			_unpCtx.size = 1;
			if (!nextChunk()) {
				decUnk1(3, 0);
			} else {
				decUnk2(8);
			}
		} else {
			uint16_t c = getCode(2);
			if (c == 3) {
				decUnk1(8, 8);
			} else {
				if (c < 2) {
					_unpCtx.size = c + 2;
					decUnk2(c + 9);
				} else {
					_unpCtx.size = getCode(8);
					decUnk2(12);
				}
			}
		}
	} while (_unpCtx.datasize > 0);
	return (_unpCtx.crc == 0);
}

uint16_t Bank::getCode(uint8_t numChunks) {
	uint16_t c = 0;
	while (numChunks--) {
		c <<= 1;
		if (nextChunk()) {
			c |= 1;
		}			
	}
	return c;
}

bool Bank::nextChunk() {
	bool CF = rcr(false);
	if (_unpCtx.chk == 0) {
		assert(_iBuf >= _startBuf);
		_unpCtx.chk = READ_BE_UINT32(_iBuf); _iBuf -= 4;
		_unpCtx.crc ^= _unpCtx.chk;
		CF = rcr(true);
	}
	return CF;
}

bool Bank::rcr(bool CF) {
	bool rCF = (_unpCtx.chk & 1);
	_unpCtx.chk >>= 1;
	if (CF) _unpCtx.chk |= 0x80000000;
	return rCF;
}
