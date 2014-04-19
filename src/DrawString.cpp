#include "DrawString.h"
#include <sstream>

TTF_Font** DrawString::fontList = NULL;

DrawString::DrawString(){
	DrawString::LoadFonts();
}

int listSize = 6;
void DrawString::LoadFonts(){
	if(fontList == NULL){
		fontList = new TTF_Font*[listSize];
		fontList[FONT_TITLE] = TTF_OpenFont("Resources/Fonts/visitor2.ttf", (int)(130*RATIO));
		fontList[FONT_MENU] = TTF_OpenFont("Resources/Fonts/visitor2.ttf", (int)(60*RATIO));
		fontList[FONT_SCORE] = TTF_OpenFont("Resources/Fonts/visitor2.ttf", (int)(70*RATIO));
		fontList[FONT_DAMAGE] = TTF_OpenFont("Resources/Fonts/visitor2.ttf", (int)(35*RATIO));
		fontList[FONT_INGAME] = TTF_OpenFont("Resources/Fonts/visitor2.ttf", (int)(35*RATIO));
		fontList[FONT_INGAME_UI] = TTF_OpenFont("Resources/Fonts/visitor2.ttf", (int)(55*RATIO));
	}
}

DrawString::~DrawString(){
	if(this->fontList != NULL){
		for(int i = 0; i < listSize; i++){
			TTF_CloseFont(this->fontList[i]);
		}
		delete[] this->fontList;
	}
}

void DrawString::Draw(SDL_Surface *viewport, std::string text, FontName font, SDL_Color fg, int x, int y){
	if(fontList == NULL){
		DrawString::LoadFonts();
	}
	SDL_Surface *textSurface = TTF_RenderUTF8_Solid(fontList[font], text.c_str(), fg);
	SDL_Rect DestR;
    DestR.x = x;
    DestR.y = y;
    SDL_BlitSurface(textSurface, NULL, viewport, &DestR);
	SDL_FreeSurface(textSurface);
}

void DrawString::Draw(SDL_Surface *viewport, int text, FontName font, SDL_Color fg, int x, int y){
    if(DrawString::fontList == NULL){
		DrawString::LoadFonts();
	}
	std::ostringstream s;
	s << text;
	DrawString::Draw(viewport, s.str(), font, fg, x, y);
}

void DrawString::Clear(){
    delete[] fontList;
}

SDL_Surface* DrawString::GetSurface(std::string text, FontName font, SDL_Color fg){
	if(DrawString::fontList == NULL){
		DrawString::LoadFonts();
	}
	if(DrawString::fontList[font])
		return TTF_RenderUTF8_Solid(DrawString::fontList[font], text.c_str(), fg);
	return NULL;
}

SDL_Surface* DrawString::GetSurface(int text, FontName font, SDL_Color fg){
    if(DrawString::fontList == NULL){
		DrawString::LoadFonts();
	}
	std::ostringstream s;
	s << text;
	return DrawString::GetSurface(s.str(), font, fg);
}
