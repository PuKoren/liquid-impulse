#ifndef _SURFACE_H
#define _SURFACE_H
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "../Maths/Vector2.h"


class Surface{
	private:
		int animation_count;
		int animation_ms;
		int frame_current;
		int frame_width;
		int frame_height;
		int animationTimer;
		bool reverse;
		bool spriteMode;
		bool looped;		
		bool halfLooped;
		bool copyCreated;
		void Load(std::string);
		SDL_Surface* surface;
        Vector2 shiftPosition;

	public:
		Surface();
		Surface(std::string);
		Surface(Surface&);
		Surface(std::string, int, int, Vector2, bool);
		Surface(Surface&, int, int, bool);
		~Surface();
		int GetWidth();
		int GetHeight();
		bool HasLooped();
		bool HasHalfLooped();
		void ResetLoop();
		void Update(Uint32);
		void Draw(SDL_Surface *, int, int);
		bool Draw(SDL_Surface * surfDest, int x, int y, SDL_Rect rect);
		void DrawSprite(SDL_Surface*, int x, int y);
		SDL_Surface* GetSDLSurface();
		void ResetSprite();
};

#endif