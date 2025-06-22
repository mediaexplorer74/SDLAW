/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 */

#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#include "intern.h"

#define VER(x) x

#define SE_INT(i,sz,ver)     { Serializer::SET_INT, sz, 1, i, ver, Serializer::CUR_VER }
#define SE_ARRAY(a,n,sz,ver) { Serializer::SET_ARRAY, sz, n, a, ver, Serializer::CUR_VER }
#define SE_PTR(p,ver)        { Serializer::SET_PTR, 0, 0, p, ver, Serializer::CUR_VER }
#define SE_END()             { Serializer::SET_END, 0, 0, 0, 0, 0 }

struct File;

struct Serializer {
	enum {
		CUR_VER = 2
	};

	enum EntryType {
		SET_INT,
		SET_ARRAY,
		SET_PTR,
		SET_END
	};

	enum {
		SES_BOOL  = 1,
		SES_INT8  = 1,
		SES_INT16 = 2,
		SES_INT32 = 4
	};

	enum Mode {
		SM_SAVE,
		SM_LOAD
	};

	struct Entry {
		EntryType type;
		uint8_t size;
		uint16_t n;
		void *data;
		uint16_t minVer;
		uint16_t maxVer;
	};

	File *_stream;
	Mode _mode;
	uint8_t *_ptrBlock;
	uint16_t _saveVer;
	uint32_t _bytesCount;
	
	Serializer(File *stream, Mode mode, uint8_t *ptrBlock, uint16_t saveVer = CUR_VER);

	void saveOrLoadEntries(Entry *entry);

	void saveEntries(Entry *entry);
	void loadEntries(Entry *entry);

	void saveInt(uint8_t es, void *p);
	void loadInt(uint8_t es, void *p);
};

#endif
