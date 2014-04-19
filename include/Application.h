#ifndef _APPLICATION_H
#define _APPLICATION_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <sstream>
#if PSP
#include <pspctrl.h>
#endif
#include "Config.h"
#include "Menu.h"
#include "GameOver.h"
#include "Level.h"
#include "DrawString.h"
#include "Settings.h"
#include "Colors.h"
#include "Hero.h"

class Application{
	private:
		bool Running;
		SDL_Surface* Viewport;
        SDL_Surface* loadingSurface;
        SDL_Surface* fpsSurface;
		Menu *menu;
		Level *level;
        GameOver *gameover;
        Level *survival;
		Settings *settings;
        SDL_Joystick* joystick;

		void LoadSettings();
		void SaveSettings();
        void JoyStickWrapper(SDL_Event* e);
        #if PSP
        void PSPControls(SceCtrlData&, SDL_Event&, unsigned int&);
        #endif
	public:
		Application();
		~Application();
		int Run();
		void Stop();
		bool Init();
        GameState state;
        void Event(SDL_Event *, bool);
		void Update(Uint32);
		void Draw(SDL_Surface *);
};

#endif
