#ifndef _MENU_H
#define _MENU_H
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include "Config.h"
#include "Settings.h"
#include "Surface.h"
#include "DrawString.h"
#include "Colors.h"

enum MenuState{
	MENU_MAIN,
	MENU_OPTIONS,
	MENU_CREDITS,
	MENU_SCORE
};

class Menu{
	public:
		Menu();
		Menu(Settings *);
		~Menu();
		void Event(SDL_Event *, GameState *);
		void Update(Uint32);
		void Draw(SDL_Surface *);
	private:
		MenuState menuState;
		Settings *settings;
		Surface *background;
		Mix_Music *backgroundMusic;
		Mix_Chunk *effectSound;
		Mix_Chunk *menuChange;
		Mix_Chunk *menuSelect;
		void Load();
		void ToggleFullScreen();
        SDL_Surface* menuTitle;
        SDL_Surface* surfYes;
        SDL_Surface* surfNo;
		unsigned int selectedItem;
		std::vector<SDL_Surface*> menuItems;
        std::vector<SDL_Surface*> menuItemsY;
		unsigned int selectedOptionItem;
		std::vector<SDL_Surface*> optionItems;
        std::vector<SDL_Surface*> optionItemsY;
		std::vector<SDL_Surface*> credits;
};
#endif