/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 */

#include <SDL.h>
#include "sys.h"
#include "util.h"


struct SDLStub : System 
{
	//typedef void (SDLStub::*ScaleProc)(uint16_t *dst, uint16_t dstPitch, const uint16_t *src, 
	//	uint16_t srcPitch, uint16_t w, uint16_t h);

	 typedef void (SDLStub::* ScaleProc)(uint16_t* dst, uint16_t dstPitch, const uint16_t* src, 
			uint16_t srcPitch, uint16_t w, uint16_t h);

	enum {
		SCREEN_W = 320,
		SCREEN_H = 200,
		SOUND_SAMPLE_RATE = 22050
	};

	//RnD
	int DEFAULT_SCALE = 3;

	SDL_Surface* _screen = nullptr;
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	uint8_t _scale = DEFAULT_SCALE;

	struct Scaler {
		const char *name;
		ScaleProc proc;
		uint8_t factor;
	};
	
	/*
	static const Scaler _scalers[];

	uint8_t *_offscreen;
	SDL_Surface *_screen;
	SDL_Surface *_sclscreen;
	bool _fullscreen;
	uint8_t _scaler;
	*/

	//uint16_t palette[NUM_COLORS];

	virtual ~SDLStub() {}
	virtual void init(const char *title);
	virtual void destroy();
	//virtual void setPalette(uint8_t s, uint8_t n, const uint8_t *buf);
	virtual void setPalette(const uint8_t* buf);
	
	virtual void updateDisplay(const uint8_t* src);
	//virtual void copyRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *buf, uint32_t pitch);
	
	virtual void processEvents();
	virtual void sleep(uint32_t duration);
	virtual uint32_t getTimeStamp();
	virtual void startAudio(AudioCallback callback, void *param);
	virtual void stopAudio();
	virtual uint32_t getOutputSampleRate();
	
	//virtual void *addTimer(uint32_t delay, TimerCallback callback, void *param);
	virtual int addTimer(uint32_t delay, TimerCallback callback, void* param);

	virtual void removeTimer(int timerId);
	virtual void *createMutex();
	virtual void destroyMutex(void *mutex);
	virtual void lockMutex(void *mutex);
	virtual void unlockMutex(void *mutex);
	
	//uint8_t* getOffScreenFramebuffer();

	void prepareGfxMode();
	void cleanupGfxMode();
	void switchGfxMode();
	//void switchGfxMode(bool fullscreen, uint8_t scaler);

	//void point1_tx(uint16_t *dst, uint16_t dstPitch, const uint16_t *src, uint16_t srcPitch, uint16_t w, uint16_t h);
	//void point2_tx(uint16_t *dst, uint16_t dstPitch, const uint16_t *src, uint16_t srcPitch, uint16_t w, uint16_t h);
	//void point3_tx(uint16_t *dst, uint16_t dstPitch, const uint16_t *src, uint16_t srcPitch, uint16_t w, uint16_t h);
	//void scale2x(uint16_t *dst, uint16_t dstPitch, const uint16_t *src, uint16_t srcPitch, uint16_t w, uint16_t h);
	//void scale3x(uint16_t *dst, uint16_t dstPitch, const uint16_t *src, uint16_t srcPitch, uint16_t w, uint16_t h);
	
};

/*
const SDLStub::Scaler SDLStub::_scalers[] = {
	{ "Point1_tx", &SDLStub::point1_tx, 1 },
	{ "Point2_tx", &SDLStub::point2_tx, 2 },
	{ "Scale2x", &SDLStub::scale2x, 2 },
	{ "Point3_tx", &SDLStub::point3_tx, 3 },
	{ "Scale3x", &SDLStub::scale3x, 3 }
};
*/


/*
void SDLStub::init(const char *title) 
{
	//RnD
	//SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
	{
		//printf("Unable to init SDL");
		printf("Unable to init SDL: %s", SDL_GetError());
		return;
	}

	//RnD
	//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	
	SDL_ShowCursor(SDL_DISABLE);
	
	//RnD
	//SDL_WM_SetCaption(title, NULL);

	int x, y; 

	SDL_GetMouseState( &x,&y ); 
	SDL_ShowCursor( SDL_ENABLE ); 
	
	//RnD
	//SDL_WarpMouse( x, y ); 

	memset(&input, 0, sizeof(input));
	_offscreen = (uint8_t *)malloc(SCREEN_W * SCREEN_H * 2);
	if (!_offscreen) {
		error("Unable to allocate offscreen buffer");
	}
	_fullscreen = false;
	_scaler = 1;
	prepareGfxMode();
}
*/

void SDLStub::init(const char* title) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	//	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	SDL_ShowCursor(SDL_DISABLE);

	SDL_ShowCursor(SDL_ENABLE);
	SDL_CaptureMouse(SDL_TRUE);

	memset(&input, 0, sizeof(input));
	_scale = DEFAULT_SCALE;
	prepareGfxMode();
}


void SDLStub::destroy() {
	cleanupGfxMode();
	SDL_Quit();
}

/*
void SDLStub::setPalette(uint8_t start, uint8_t numEnties, const uint8_t *buf) 
{

	assert(start + numEnties <= 16);

	for (int i = start; i < start + numEnties; ++i) {

		uint8_t c[3];
		for (int j = 0; j < 3; j++) {
			uint8_t col = buf[i * 3 + j];
			c[j] =  (col << 2) | (col & 3);
		}

		palette[i] = SDL_MapRGB(_screen->format, c[0], c[1], c[2]);
	}

}
*/
static SDL_Color palette[NUM_COLORS];
void SDLStub::setPalette(const uint8_t* p) {
	// The incoming palette is in 565 format.
	for (int i = 0; i < NUM_COLORS; ++i)
	{
		uint8_t c1 = *(p + 0);
		uint8_t c2 = *(p + 1);
		// debug(DBG_VM, "-- color %d: %02x%02x", i, c2,c1);
		palette[i].r = (((c1 & 0x0F) << 2) | ((c1 & 0x0F) >> 2)) << 2; // r
		palette[i].g = (((c2 & 0xF0) >> 2) | ((c2 & 0xF0) >> 6)) << 2; // g
		palette[i].b = (((c2 & 0x0F) >> 2) | ((c2 & 0x0F) << 2)) << 2; // b
		palette[i].a = 0xFF;
		p += 2;
	}
	SDL_SetPaletteColors(_screen->format->palette, palette, 0, NUM_COLORS);
}//SDLStub::setPalette

/*
void SDLStub::copyRect(uint16_t x, uint16_t y, uint16_t width,
	uint16_t height, const uint8_t *buf, uint32_t pitch) {

	buf += y * pitch + x;
	uint16_t *p = (uint16_t *)_offscreen;

	//For each line
	while (height--) {

		//One byte gives us two pixels, we only need to iterate w/2 times.
		for (int i = 0; i < width / 2; ++i) {

			//Extract two palette indices from upper byte and lower byte.
			uint8_t p1 = *(buf + i) >> 4;
			uint8_t p2 = *(buf + i) & 0xF;

			//Get the pixel value from the palette and write in in offScreen.
			*(p + i * 2 + 0) = palette[p1];
			*(p + i * 2 + 1) = palette[p2];

		}

		p += SCREEN_W;
		buf += pitch;
	}

	SDL_LockSurface(_sclscreen);
	(this->*_scalers[_scaler].proc)((uint16_t *)_sclscreen->pixels, _sclscreen->pitch, 
		(uint16_t *)_offscreen, SCREEN_W, SCREEN_W, SCREEN_H);

	SDL_UnlockSurface(_sclscreen);
	
	SDL_BlitSurface(_sclscreen, NULL, _screen, NULL);
	
	//RnD
	//SDL_UpdateRect(_screen, 0, 0, 0, 0);
	//SDL_UpdateWindowSurface(_screen, 0, 0, 0, 0);
}
*/


void SDLStub::prepareGfxMode() 
{
	int w = SCREEN_W;
	int h = SCREEN_H;

	_window = SDL_CreateWindow("Another World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		w * _scale, h * _scale, SDL_WINDOW_SHOWN);

	_renderer = SDL_CreateRenderer(_window, -1, 0);
	_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 8, 0, 0, 0, 0);
	if (!_screen) {
		error("SDLStub::prepareGfxMode() unable to allocate _screen buffer");
	}
	// Upon resize during gameplay, the screen surface is re-created and a new palette is allocated.
	// This will result in an all-white surface palette displaying a window full of white until a
	// a palette is set by the VM.
	// To avoid this issue, we save the last palette locally and re-upload it each time. On game start-up this
	// is not requested.
	SDL_SetPaletteColors(_screen->format->palette, palette, 0, NUM_COLORS);
}

void SDLStub::updateDisplay(const uint8_t* src) 
{
	uint16_t height = SCREEN_H;
	uint8_t* p = (uint8_t*)_screen->pixels;

	//For each line
	while (height--) {
		//One byte gives us two pixels, we only need to iterate w/2 times.
		for (int i = 0; i < SCREEN_W / 2; ++i) {
			//Extract two palette indices from upper byte and lower byte.
			p[i * 2 + 0] = *(src + i) >> 4;
			p[i * 2 + 1] = *(src + i) & 0xF;
		}
		p += _screen->pitch;
		src += SCREEN_W / 2;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, _screen);
	SDL_RenderCopy(_renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(_renderer);
	SDL_DestroyTexture(texture);

}

/*
void SDLStub::processEvents() 
{
	SDL_Event ev;
	while(SDL_PollEvent(&ev)) 
	{
		switch (ev.type) {
		case SDL_QUIT:
			input.quit = true;
			break;
		case SDL_KEYUP:
			switch(ev.key.keysym.sym) 
			{
			case SDLK_LEFT:
				input.dirMask &= ~PlayerInput::DIR_LEFT;
				break;
			case SDLK_RIGHT:
				input.dirMask &= ~PlayerInput::DIR_RIGHT;
				break;
			case SDLK_UP:
				input.dirMask &= ~PlayerInput::DIR_UP;
				break;
			case SDLK_DOWN:
				input.dirMask &= ~PlayerInput::DIR_DOWN;
				break;
			case SDLK_SPACE:
			case SDLK_RETURN:
				input.button = false;
				break;
			default:
				break;
			}
			break;
		case SDL_KEYDOWN:

			if (ev.key.keysym.mod & KMOD_ALT) 
			{
				if (ev.key.keysym.sym == SDLK_RETURN) 
				{
					switchGfxMode(!_fullscreen, _scaler);
				} else if (ev.key.keysym.sym == SDLK_KP_PLUS) 
				{
					uint8_t s = _scaler + 1;
					if (s < ARRAYSIZE(_scalers)) {
						switchGfxMode(_fullscreen, s);
					}
				} else if (ev.key.keysym.sym == SDLK_KP_MINUS) 
				{
					int8_t s = _scaler - 1;
					if (_scaler > 0) {
						switchGfxMode(_fullscreen, s);
					}
				} else if (ev.key.keysym.sym == SDLK_x) 
				{
					input.quit = true;
				}
				break;
			} else if (ev.key.keysym.mod & KMOD_CTRL) 
			{
				if (ev.key.keysym.sym == SDLK_s) {
					input.save = true;
				} else if (ev.key.keysym.sym == SDLK_l) {
					input.load = true;
				} else if (ev.key.keysym.sym == SDLK_f) {
					input.fastMode = true;
				} else if (ev.key.keysym.sym == SDLK_KP_PLUS) {
					input.stateSlot = 1;
				} else if (ev.key.keysym.sym == SDLK_KP_MINUS) {
					input.stateSlot = -1;
				}
				break;
			}
			input.lastChar = ev.key.keysym.sym;
			switch(ev.key.keysym.sym) {
			case SDLK_LEFT:
				input.dirMask |= PlayerInput::DIR_LEFT;
				break;
			case SDLK_RIGHT:
				input.dirMask |= PlayerInput::DIR_RIGHT;
				break;
			case SDLK_UP:
				input.dirMask |= PlayerInput::DIR_UP;
				break;
			case SDLK_DOWN:
				input.dirMask |= PlayerInput::DIR_DOWN;
				break;
			case SDLK_SPACE:
			case SDLK_RETURN:
				input.button = true;
				break;
			case SDLK_c:
				input.code = true;
				break;
			case SDLK_p:
				input.pause = true;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}
*/
void SDLStub::processEvents() {
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT:
			input.quit = true;
			break;
		case SDL_KEYUP:
			switch (ev.key.keysym.sym) {
			case SDLK_LEFT:
				input.dirMask &= ~PlayerInput::DIR_LEFT;
				break;
			case SDLK_RIGHT:
				input.dirMask &= ~PlayerInput::DIR_RIGHT;
				break;
			case SDLK_UP:
				input.dirMask &= ~PlayerInput::DIR_UP;
				break;
			case SDLK_DOWN:
				input.dirMask &= ~PlayerInput::DIR_DOWN;
				break;
			case SDLK_SPACE:
			case SDLK_RETURN:
				input.button = false;
				break;
			case SDLK_ESCAPE:
				input.quit = true;
				break;
			}
			break;
		case SDL_KEYDOWN:
			if (ev.key.keysym.mod & KMOD_CTRL) {
				if (ev.key.keysym.sym == SDLK_x) {
					input.quit = true;
				}
				else if (ev.key.keysym.sym == SDLK_s) {
					input.save = true;
				}
				else if (ev.key.keysym.sym == SDLK_l) {
					input.load = true;
				}
				else if (ev.key.keysym.sym == SDLK_KP_PLUS) {
					input.stateSlot = 1;
				}
				else if (ev.key.keysym.sym == SDLK_KP_MINUS) {
					input.stateSlot = -1;
				}
				break;
			}
			input.lastChar = ev.key.keysym.sym;
			switch (ev.key.keysym.sym) {
			case SDLK_LEFT:
				input.dirMask |= PlayerInput::DIR_LEFT;
				break;
			case SDLK_RIGHT:
				input.dirMask |= PlayerInput::DIR_RIGHT;
				break;
			case SDLK_UP:
				input.dirMask |= PlayerInput::DIR_UP;
				break;
			case SDLK_DOWN:
				input.dirMask |= PlayerInput::DIR_DOWN;
				break;
			case SDLK_SPACE:
			case SDLK_RETURN:
				input.button = true;
				break;
			case SDLK_c:
				input.code = true;
				break;
			case SDLK_p:
				input.pause = true;
				break;
			case SDLK_TAB:
				_scale = _scale + 1;
				if (_scale > 4) { _scale = 1; }
				switchGfxMode();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}



void SDLStub::sleep(uint32_t duration) 
{
	SDL_Delay(duration);
}

uint32_t SDLStub::getTimeStamp() {
	return SDL_GetTicks();	
}

void SDLStub::startAudio(AudioCallback callback, void *param) 
{
	SDL_AudioSpec desired;
	memset(&desired, 0, sizeof(desired));

	desired.freq = SOUND_SAMPLE_RATE;
	desired.format = AUDIO_U8;
	desired.channels = 1;
	desired.samples = 2048;
	desired.callback = callback;
	desired.userdata = param;
	if (SDL_OpenAudio(&desired, NULL) == 0) {
		SDL_PauseAudio(0);
	} else {
		error("SDLStub::startAudio() unable to open sound device");
	}
}


void SDLStub::stopAudio() 
{
	SDL_CloseAudio();
}


uint32_t SDLStub::getOutputSampleRate() 
{
	return SOUND_SAMPLE_RATE;
}


//void* SDLStub::addTimer(uint32_t delay, TimerCallback callback, void *param) 
int SDLStub::addTimer(uint32_t delay, TimerCallback callback, void* param)
{
    return SDL_AddTimer(delay,(SDL_TimerCallback)callback, param);
}


void SDLStub::removeTimer(int timerId)
{
	SDL_RemoveTimer(timerId);
}


void *SDLStub::createMutex() 
{
	return SDL_CreateMutex();
}


void SDLStub::destroyMutex(void *mutex) 
{
	SDL_DestroyMutex((SDL_mutex *)mutex);
}


void SDLStub::lockMutex(void *mutex) 
{
	SDL_mutexP((SDL_mutex *)mutex);
}


void SDLStub::unlockMutex(void *mutex) 
{
	SDL_mutexV((SDL_mutex *)mutex);
}


// SDLStub::cleanupGfxMode
void SDLStub::cleanupGfxMode() 
{
	if (_screen) {
		SDL_FreeSurface(_screen);
		_screen = 0;
	}

	if (_window) {
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}

	if (_screen) {
		SDL_FreeSurface(_screen);
		_screen = nullptr;
	}
}


void SDLStub::switchGfxMode() {
	cleanupGfxMode();
	prepareGfxMode();
}

SDLStub sysImplementation;
System* stub = &sysImplementation;

/*
void SDLStub::cleanupGfxMode() 
{
	if (_offscreen) 
	{
		free(_offscreen);
		_offscreen = 0;
	}
	if (_sclscreen) 
	{
		SDL_FreeSurface(_sclscreen);
		_sclscreen = 0;
	}
	if (_screen)
	{
		SDL_FreeSurface(_screen);
		_screen = 0;
	}
}
*/

/*
void SDLStub::switchGfxMode(bool fullscreen, uint8_t scaler) 
{
	SDL_Surface *prev_sclscreen = _sclscreen;
	SDL_FreeSurface(_screen); 	
	_fullscreen = fullscreen;
	_scaler = scaler;
	prepareGfxMode();
	SDL_BlitSurface(prev_sclscreen, NULL, _sclscreen, NULL);
	SDL_FreeSurface(prev_sclscreen);
}
*/

/*
void SDLStub::point1_tx(uint16_t *dst, uint16_t dstPitch, 
	const uint16_t *src, uint16_t srcPitch, uint16_t w, uint16_t h) 
{
	dstPitch >>= 1;
	while (h--) {
		memcpy(dst, src, w * 2);
		dst += dstPitch;
		src += dstPitch;
	}
}
*/

/*
void SDLStub::point2_tx(uint16_t *dst, uint16_t dstPitch, const uint16_t *src, 
	uint16_t srcPitch, uint16_t w, uint16_t h) 
{
	dstPitch >>= 1;
	while (h--) 
	{
		uint16_t *p = dst;
		for (int i = 0; i < w; ++i, p += 2) {
			uint16_t c = *(src + i);
			*(p + 0) = c;
			*(p + 1) = c;
			*(p + 0 + dstPitch) = c;
			*(p + 1 + dstPitch) = c;
		}
		dst += dstPitch * 2;
		src += srcPitch;
	}
}
*/

/*
void SDLStub::point3_tx(uint16_t *dst, uint16_t dstPitch, 
	const uint16_t *src, uint16_t srcPitch, uint16_t w, uint16_t h) 
{
	dstPitch >>= 1;
	while (h--) 
	{
		uint16_t *p = dst;
		for (int i = 0; i < w; ++i, p += 3) 
		{
			uint16_t c = *(src + i);
			*(p + 0) = c;
			*(p + 1) = c;
			*(p + 2) = c;
			*(p + 0 + dstPitch) = c;
			*(p + 1 + dstPitch) = c;
			*(p + 2 + dstPitch) = c;
			*(p + 0 + dstPitch * 2) = c;
			*(p + 1 + dstPitch * 2) = c;
			*(p + 2 + dstPitch * 2) = c;
		}
		dst += dstPitch * 3;
		src += srcPitch;
	}
}
*/

/*
void SDLStub::scale2x(uint16_t *dst, uint16_t dstPitch, const uint16_t *src, 
	uint16_t srcPitch, uint16_t w, uint16_t h) 
{
	dstPitch >>= 1;
	while (h--) {
		uint16_t *p = dst;
		for (int i = 0; i < w; ++i, p += 2) {
			uint16_t B = *(src + i - srcPitch);
			uint16_t D = *(src + i - 1);
			uint16_t E = *(src + i);
			uint16_t F = *(src + i + 1);
			uint16_t H = *(src + i + srcPitch);
			if (B != H && D != F) {
				*(p) = D == B ? D : E;
				*(p + 1) = B == F ? F : E;
				*(p + dstPitch) = D == H ? D : E;
				*(p + dstPitch + 1) = H == F ? F : E;
			} else {
				*(p) = E;
				*(p + 1) = E;
				*(p + dstPitch) = E;
				*(p + dstPitch + 1) = E;
			}
		}
		dst += dstPitch * 2;
		src += srcPitch;
	}
}
*/

/*
void SDLStub::scale3x(uint16_t *dst, uint16_t dstPitch, const uint16_t *src, uint16_t srcPitch, uint16_t w, uint16_t h) {
	dstPitch >>= 1;
	while (h--) {
		uint16_t *p = dst;
		for (int i = 0; i < w; ++i, p += 3) {
			uint16_t A = *(src + i - srcPitch - 1);
			uint16_t B = *(src + i - srcPitch);
			uint16_t C = *(src + i - srcPitch + 1);
			uint16_t D = *(src + i - 1);
			uint16_t E = *(src + i);
			uint16_t F = *(src + i + 1);
			uint16_t G = *(src + i + srcPitch - 1);
			uint16_t H = *(src + i + srcPitch);
			uint16_t I = *(src + i + srcPitch + 1);
			if (B != H && D != F) {
				*(p) = D == B ? D : E;
				*(p + 1) = (D == B && E != C) || (B == F && E != A) ? B : E;
				*(p + 2) = B == F ? F : E;
				*(p + dstPitch) = (D == B && E != G) || (D == B && E != A) ? D : E;
				*(p + dstPitch + 1) = E;
				*(p + dstPitch + 2) = (B == F && E != I) || (H == F && E != C) ? F : E;
				*(p + 2 * dstPitch) = D == H ? D : E;
				*(p + 2 * dstPitch + 1) = (D == H && E != I) || (H == F && E != G) ? H : E;
				*(p + 2 * dstPitch + 2) = H == F ? F : E;
			} else {
				*(p) = E;
				*(p + 1) = E;
				*(p + 2) = E;
				*(p + dstPitch) = E;
				*(p + dstPitch + 1) = E;
				*(p + dstPitch + 2) = E;
				*(p + 2 * dstPitch) = E;
				*(p + 2 * dstPitch + 1) = E;
				*(p + 2 * dstPitch + 2) = E;
			}
		}
		dst += dstPitch * 3;
		src += srcPitch;
	}
}
*/

/*
uint8_t* SDLStub::getOffScreenFramebuffer()
{
	return _offscreen;
}
*/


//SDLStub sysImplementation;
//System *stub = &sysImplementation;

