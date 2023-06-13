/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "intern.h"


#define NUM_COLORS 16
#define BYTE_PER_PIXEL 3

struct PlayerInput {
	enum {
		DIR_LEFT  = 1 << 0,
		DIR_RIGHT = 1 << 1,
		DIR_UP    = 1 << 2,
		DIR_DOWN  = 1 << 3
	};

	uint8_t dirMask;
	bool button;
	bool code;
	bool pause;
	bool quit;
	char lastChar;
	bool save, load;
	bool fastMode;
	int8_t stateSlot;
};

/*
	System is an abstract class so any find of system can be plugged underneath.
*/
struct System {
	typedef void (*AudioCallback)(void *param, uint8_t *stream, int len);
	typedef uint32_t (*TimerCallback)(uint32_t delay, void *param);
	
	PlayerInput input;

	virtual ~System() {}

	virtual void init(const char *title) = 0;
	virtual void destroy() = 0;

	virtual void setPalette(const uint8_t* buf) = 0;
	//virtual void setPalette(uint8_t s, uint8_t n, const uint8_t *buf) = 0;
	
	//RnD
	virtual void updateDisplay(const uint8_t* buf) = 0;
	//virtual void copyRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *buf, uint32_t pitch) = 0;

	virtual void processEvents() = 0;
	virtual void sleep(uint32_t duration) = 0;
	virtual uint32_t getTimeStamp() = 0;

	virtual void startAudio(AudioCallback callback, void *param) = 0;
	virtual void stopAudio() = 0;
	virtual uint32_t getOutputSampleRate() = 0;
	
	//virtual void *addTimer(uint32_t delay, TimerCallback callback, void *param) = 0;
	virtual int addTimer(uint32_t delay, TimerCallback callback, void* param) = 0;

	virtual void removeTimer(int timerId) = 0;

	virtual void *createMutex() = 0;
	virtual void destroyMutex(void *mutex) = 0;
	virtual void lockMutex(void *mutex) = 0;
	virtual void unlockMutex(void *mutex) = 0;

	//virtual uint8_t* getOffScreenFramebuffer() = 0;
};


struct MutexStack {
	System *sys;
	void *_mutex;

	MutexStack(System *stub, void *mutex) 
		: sys(stub), _mutex(mutex) {
		sys->lockMutex(_mutex);
	}
	~MutexStack() {
		sys->unlockMutex(_mutex);
	}
};



#endif
