/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 */

#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "intern.h"
#include "vm.h"
#include "mixer.h"
#include "sfxplayer.h"
#include "resource.h"
#include "video.h"

struct System;

struct Engine {
	enum {
		MAX_SAVE_SLOTS = 100
	};

	System *sys;
	VirtualMachine vm;
	Mixer mixer;
	Resource res;
	SfxPlayer player;
	Video video;
	const char *_dataDir, *_saveDir;
	uint8_t _stateSlot;

	Engine(System *stub, const char *dataDir, const char *saveDir);
	~Engine();

	void run();
	void init();
	void finish();
	void processInput();
	
	void makeGameStateName(uint8_t slot, char *buf);
	void saveGameState(uint8_t slot, const char *desc);
	void loadGameState(uint8_t slot);
	const char* getDataDir();
};

#endif
