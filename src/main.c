/*
 * SDLPoP, a port/conversion of the DOS game Prince of Persia.
 * Copyright (C) 2013-2020  Dávid Nagy
 */


#include "common.h"

int main(int argc, char *argv[])
{
	SDL_SetHint(SDL_HINT_WINRT_HANDLE_BACK_BUTTON, "1");

	g_argc = argc;
	g_argv = argv;

	pop_main(); // -> seg000.c
	
	return 0;
}

