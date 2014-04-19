#ifndef _BACKGROUNDM_H
#define _BACKGROUNDM_H
#include "Surface.h"
#include "Config.h"
#include <string>

class BackgroundManager{
public:
	BackgroundManager(std::string, int);
	~BackgroundManager();
	void Draw(SDL_Surface *);
	void Move(int);
	void MoveY(int);
private:
	Surface** backgrounds;
	int count;
	int position;
	int positionY;
	int totalWidth;
};

#endif