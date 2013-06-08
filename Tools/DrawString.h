#ifndef _DRAWSTRING_H
#define _DRAWSTRING_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "../Config.h"

enum FontName{
	FONT_TITLE,
	FONT_MENU,
	FONT_INGAME,
	FONT_INGAME_UI,
	FONT_DAMAGE,
	FONT_SCORE
};

class DrawString{
	private:
		static TTF_Font** fontList;
		void static LoadFonts();
	public:
		DrawString();
		~DrawString();
        static void  Clear();
		static void  Draw(SDL_Surface *, std::string, FontName, SDL_Color, int, int);
		static void  Draw(SDL_Surface *, int, FontName, SDL_Color, int, int);
		static SDL_Surface* GetSurface(std::string, FontName, SDL_Color);
		static SDL_Surface* GetSurface(int, FontName, SDL_Color);
};

#endif