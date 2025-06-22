// SDLAW, a port/conversion of the DOS game Another World.
// MediaExplorer 2023

#include "common.h"
#include <setjmp.h>
#include <math.h>
#include "engine.h"
#include "sys.h"
#include "util.h"


static const char* USAGE =
"Raw - Another World Interpreter\n"
"Usage: raw [OPTIONS]...\n"
"  --datapath=PATH   Path to where the game is installed (default '.')\n"
"  --savepath=PATH   Path to where the save files are stored (default '.')\n";

static bool parseOption(const char* arg, const char* longCmd, const char** opt)
{
	bool ret = false;
	if (arg[0] == '-' && arg[1] == '-') 
	{
		if (strncmp(arg + 2, longCmd, strlen(longCmd)) == 0) 
		{
			*opt = arg + 2 + strlen(longCmd);
			ret = true;
		}
	}
	return ret;
}

/*
	An Abstract Class pointer pointing to the implementation on the Heap.
*/
extern System* stub;

// data:461E
dat_type * dathandle;

// data:4C08
word need_redraw_because_flipped;

//void fix_sound_priorities();

// seg000:0000
void far pop_main() 
{
	const char* dataPath = ".";
	const char* savePath = ".";

	
	for (int i = 1; i < g_argc; ++i)
	{
		bool opt = false;
		if (strlen(g_argv[i]) >= 2) {
			opt |= parseOption(g_argv[i], "datapath=", &dataPath);
			opt |= parseOption(g_argv[i], "savepath=", &savePath);
		}
		
	}
		
	
	//RnD
	g_debugMask = DBG_RES; //DBG_INFO |  DBG_VM | DBG_BANK | DBG_VIDEO | DBG_SER | DBG_SND

	Engine* e = new Engine(stub, dataPath, savePath);
	e->init();
	e->run();


	delete e;


}

