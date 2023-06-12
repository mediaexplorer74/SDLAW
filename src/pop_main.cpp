/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 */

/*
#include "engine.h"
#include "sys.h"
#include "util.h"


static const char *USAGE = 
	"Raw - Another World Interpreter\n"
	"Usage: raw [OPTIONS]...\n"
	"  --datapath=PATH   Path to where the game is installed (default '.')\n"
	"  --savepath=PATH   Path to where the save files are stored (default '.')\n";

static bool parseOption(const char *arg, const char *longCmd, const char **opt) 
{
	bool ret = false;
	if (arg[0] == '-' && arg[1] == '-') {
		if (strncmp(arg + 2, longCmd, strlen(longCmd)) == 0) {
			*opt = arg + 2 + strlen(longCmd);
			ret = true;
		}
	}
	return ret;
}
*/

/*
	An Abstract Class pointer pointing to the implementation on the Heap.
*/
//extern System *System_SDL_create();
//extern System *stub;//= System_SDL_create();

//#undef pop_main

/*
//int 
void far pop_main()//(int argc, char *argv[]) 
{
	const char *dataPath = ".";
	const char *savePath = ".";
	for (int i = 1; i < argc; ++i) 
	{
		bool opt = false;
		if (strlen(argv[i]) >= 2) {
			opt |= parseOption(argv[i], "datapath=", &dataPath);
			opt |= parseOption(argv[i], "savepath=", &savePath);
		}
		if (!opt) {
			//printf(USAGE);
			//return 0;
		}
	}
	//FCS
	//g_debugMask = DBG_INFO; // DBG_VM | DBG_BANK | DBG_VIDEO | DBG_SER | DBG_SND
	g_debugMask = DBG_RES ;
	//g_debugMask = 0 ;//DBG_INFO |  DBG_VM | DBG_BANK | DBG_VIDEO | DBG_SER | DBG_SND ;
	
	Engine* e = new Engine(stub, dataPath, savePath);
	e->init();
	e->run();


	delete e;

	//delete stub;

	//return 0;
}
*/



