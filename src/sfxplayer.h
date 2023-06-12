/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 */

#ifndef __SFXPLAYER_H__
#define __SFXPLAYER_H__

#include "intern.h"

struct SfxInstrument {
	uint8_t *data;
	uint16_t volume;
};

struct SfxModule {
	const uint8_t *data;
	uint16_t curPos;
	uint8_t curOrder;
	uint8_t numOrder;
	uint8_t orderTable[0x80];
	SfxInstrument samples[15];
};

struct SfxPattern {
	uint16_t note_1;
	uint16_t note_2;
	uint16_t sampleStart;
	uint8_t *sampleBuffer;
	uint16_t sampleLen;
	uint16_t loopPos;
	uint8_t *loopData;
	uint16_t loopLen;
	uint16_t sampleVolume;
};

struct Mixer;
struct Resource;
struct Serializer;
struct System;

struct SfxPlayer {
	Mixer *mixer;
	Resource *res;
	System *sys;

	void *_mutex;
	void *_timerId;
	uint16_t _delay;
	uint16_t _resNum;
	SfxModule _sfxMod;
	int16_t *_markVar;

	SfxPlayer(Mixer *mix, Resource *res, System *stub);
	void init();
	void free();

	void setEventsDelay(uint16_t delay);
	void loadSfxModule(uint16_t resNum, uint16_t delay, uint8_t pos);
	void prepareInstruments(const uint8_t *p);
	void start();
	void stop();
	void handleEvents();
	void handlePattern(uint8_t channel, const uint8_t *patternData);

	static uint32_t eventsCallback(uint32_t interval, void *param);

	void saveOrLoad(Serializer &ser);
};

#endif
