/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 */

#define ZLIB_INTERNAL
//#include "zconf.h"
//#include "zlib.h"
#include "file.h"
#include "../SDL2_image/external/zlib-1.2.8/zconf.h"
#include "../SDL2_image/external/zlib-1.2.8/zlib.h"

struct File_impl 
{
	bool _ioErr;
	File_impl() : _ioErr(false) {}
	virtual bool open(const char *path, const char *mode) = 0;
	virtual void close() = 0;
	virtual void seek(int32_t off) = 0;
	virtual void read(void *ptr, uint32_t size) = 0;
	virtual void write(void *ptr, uint32_t size) = 0;
};

struct stdFile : File_impl {
	FILE *_fp;
	stdFile() : _fp(0) {}
	bool open(const char *path, const char *mode) 
	{
		_ioErr = false;
		_fp = fopen(path, mode);
		return (_fp != NULL);
	}
	void close() 
	{
		if (_fp) 
		{
			fclose(_fp);
			_fp = 0;
		}
	}
	void seek(int32_t off) 
	{
		if (_fp) {
			fseek(_fp, off, SEEK_SET);
		}
	}
	void read(void *ptr, uint32_t size) 
	{
		if (_fp) {
			uint32_t r = fread(ptr, 1, size, _fp);
			if (r != size) {
				_ioErr = true;
			}
		}
	}
	void write(void *ptr, uint32_t size) 
	{
		if (_fp) {
			uint32_t r = fwrite(ptr, 1, size, _fp);
			if (r != size) {
				_ioErr = true;
			}
		}
	}
};

struct zlibFile : File_impl 
{
	gzFile _fp;
	zlibFile() : _fp(0) {}
	bool open(const char *path, const char *mode) 
	{
		_ioErr = false;
		_fp = gzopen(path, mode);
		return (_fp != NULL);
	}
	void close() {
		if (_fp) {
			gzclose(_fp);
			_fp = 0;
		}
	}
	void seek(int32_t off) 
	{
		if (_fp) {
			gzseek(_fp, off, SEEK_SET);
		}
	}
	void read(void *ptr, uint32_t size) 
	{
		if (_fp) {
			uint32_t r = gzread(_fp, ptr, size);
			if (r != size) {
				_ioErr = true;
			}
		}
	}
	void write(void *ptr, uint32_t size) 
	{
		if (_fp) {
			uint32_t r = gzwrite(_fp, ptr, size);
			if (r != size) {
				_ioErr = true;
			}
		}
	}
};

File::File(bool gzipped) 
{
	if (gzipped) {
		_impl = new zlibFile;
	} else {
		_impl = new stdFile;
	}
}

File::~File() 
{
	_impl->close();
	delete _impl;
}

bool File::open(const char *filename, const char *directory, const char *mode) 
{	
	_impl->close();
	char buf[512];
	sprintf(buf, "%s/%s", directory, filename);
	char *p = buf + strlen(directory) + 1;
	string_lower(p);
	bool opened = _impl->open(buf, mode);
	if (!opened) { // let's try uppercase
		string_upper(p);
		opened = _impl->open(buf, mode);
	}
	return opened;
}

void File::close() 
{
	_impl->close();
}

bool File::ioErr() const 
{
	return _impl->_ioErr;
}

void File::seek(int32_t off) 
{
	_impl->seek(off);
}

void File::read(void *ptr, uint32_t size) 
{
	_impl->read(ptr, size);
}

uint8_t File::readByte() 
{
	uint8_t b;
	read(&b, 1);
	return b;
}

uint16_t File::readUint16BE() 
{
	uint8_t hi = readByte();
	uint8_t lo = readByte();
	return (hi << 8) | lo;
}

uint32_t File::readUint32BE() 
{
	uint16_t hi = readUint16BE();
	uint16_t lo = readUint16BE();
	return (hi << 16) | lo;
}

void File::write(void *ptr, uint32_t size) 
{
	_impl->write(ptr, size);
}

void File::writeByte(uint8_t b) 
{
	write(&b, 1);
}

void File::writeUint16BE(uint16_t n) 
{
	writeByte(n >> 8);
	writeByte(n & 0xFF);
}

void File::writeUint32BE(uint32_t n) 
{
	writeUint16BE(n >> 16);
	writeUint16BE(n & 0xFFFF);
}
