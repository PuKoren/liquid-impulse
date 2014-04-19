#include "Rectangle.h"

Rectangle::Rectangle(){
	this->Position = Vector2(0,0);
	this->Width = 0;
	this->Height = 0;
}
Rectangle::Rectangle(Rectangle *rect){
	this->Position = rect->Position;
	this->Width = rect->Width;
	this->Height = rect->Height;
}
Rectangle::~Rectangle(){

}

Rectangle::Rectangle(float x, float y, int width, int height){
	this->Position = Vector2(x, y);
	this->Width = width;
	this->Height = height;
}

bool Rectangle::CollideWith(Rectangle *rect){
	//compute collision on X axis
	bool collide = (this->Position.X >= rect->Position.X && this->Position.X <= rect->Position.X + rect->Width) 
		|| (this->Position.X + this->Width > rect->Position.X && this->Position.X + this->Width < rect->Position.X + rect->Width);
	//compute collision on Y axis
	collide = (collide && ((this->Position.Y >= rect->Position.Y && this->Position.Y <= rect->Position.Y + rect->Height) 
		|| (this->Position.Y + this->Height >= rect->Position.Y && this->Position.Y + this->Height <= rect->Position.Y + rect->Height)));

	//compute if rectangle is contained by this (no intersections but still collide)
	collide = collide || (rect->Position.Y >= this->Position.Y && rect->Position.Y <= this->Position.Y + this->Height && rect->Position.X > this->Position.X && rect->Position.X < this->Position.X + this->Width);
	return collide;
}

void Rectangle::Draw(SDL_Surface *viewport, int R, int G, int B){
	SDL_Rect ligneHaut;
	ligneHaut.x = (Sint16)this->Position.X - 1;
	ligneHaut.y = (Sint16)this->Position.Y - 1;
	ligneHaut.w = this->Width;
	ligneHaut.h = 1;
 
	SDL_FillRect(viewport, &ligneHaut, SDL_MapRGB(viewport->format, R, G, B));
 
	SDL_Rect ligneDroite;
	ligneDroite.x = (Sint16)this->Position.X + this->Width - 1;
	ligneDroite.y = (Sint16)this->Position.Y - 1;
	ligneDroite.w = 1;
	ligneDroite.h = this->Height;
 
	SDL_FillRect(viewport, &ligneDroite, SDL_MapRGB(viewport->format, R, G, B));
 
	SDL_Rect ligneGauche;
	ligneGauche.x = (Sint16)this->Position.X - 1;
	ligneGauche.y = (Sint16)this->Position.Y - 1;
	ligneGauche.w = 1;
	ligneGauche.h = this->Height;
 
	SDL_FillRect(viewport, &ligneGauche, SDL_MapRGB(viewport->format, R, G, B));
 
	SDL_Rect ligneBas;
	
	ligneBas.x = (Sint16)this->Position.X-1;
	ligneBas.y = (Sint16)this->Position.Y + this->Height - 1;
	ligneBas.w = this->Width;
	ligneBas.h = 1;
 
	SDL_FillRect(viewport, &ligneBas, SDL_MapRGB(viewport->format, R, G, B));
}
