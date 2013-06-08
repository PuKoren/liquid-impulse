#include "Surface.h"

Surface::Surface(){
	this->surface = NULL;
	this->spriteMode = false;
	this->copyCreated = false;
    this->shiftPosition = Vector2(0,0);
    this->animationTimer = 0;
}

Surface::Surface(std::string name){
	this->surface = NULL;
	this->spriteMode = false;
	this->copyCreated = false;
	this->Load(name);
    this->shiftPosition = Vector2(0,0);
    this->animationTimer = 0;
}

Surface::Surface(std::string name, int animationCount, int animationMs, Vector2 shiftPosition, bool reverse = false){
	this->surface = NULL;
	this->Load(name);
	this->animation_count = animationCount;
	this->animation_ms = animationMs;
	this->frame_current = (reverse)?animationCount-1:0;
	this->frame_width = this->surface->w / this->animation_count;
	this->frame_height = this->surface->h;
	this->looped = false;
	this->halfLooped = false;
	this->spriteMode = true;
	this->copyCreated = false;
    this->shiftPosition = shiftPosition;
	this->reverse = reverse;
    this->animationTimer = 0;
}

Surface::Surface(Surface& _Surface){
	this->copyCreated = true;
	this->spriteMode = false;
	this->surface = _Surface.GetSDLSurface();
    this->shiftPosition = Vector2(0,0);
    this->animationTimer = 0;
}

Surface::Surface(Surface& _Surface, int animationCount, int animationMs, bool reverse){
	this->surface = _Surface.GetSDLSurface();
	this->animation_count = animationCount;
	this->animation_ms = animationMs;
	this->frame_current = (reverse)?animationCount-1:0;
	this->frame_width = this->surface->w / this->animation_count;
	this->frame_height = this->surface->h;
	this->looped = false;
	this->halfLooped = false;
	this->spriteMode = true;
	this->copyCreated = true;
	this->reverse = reverse;
    this->shiftPosition = Vector2(0,0);
    this->animationTimer = 0;
}

Surface::~Surface(){
	if(!this->copyCreated){
		SDL_FreeSurface(this->surface);
	}
}

void Surface::Load(std::string name){
	SDL_Surface * tmp;
	if((tmp = IMG_Load(name.c_str())) == NULL){
		this->surface = NULL;
		return;
	}else{
		this->surface = SDL_DisplayFormatAlpha(tmp);
		SDL_FreeSurface(tmp);
	}
}

SDL_Surface* Surface::GetSDLSurface(){
	return this->surface;
}

bool Surface::HasLooped(){
	if(!this->spriteMode){
		return false;
	}
	return this->looped;
}

bool Surface::HasHalfLooped(){
	if(!this->spriteMode){
		return false;
	}
	return this->halfLooped;
}

void Surface::ResetLoop(){
	this->looped = false;
	this->halfLooped = false;
}

void Surface::ResetSprite(){
	if(!this->spriteMode)return;
	this->frame_current = (this->reverse)?this->animation_count-1:0;
}

int Surface::GetWidth(){
	if(!this->spriteMode){
		return this->surface->w;
	}else{
		return this->frame_width;
	}
}

int Surface::GetHeight(){
	return this->surface->h;
}

void Surface::Update(Uint32 gameTime){
	//if sprite mode is off this method should not be called
	if(!this->spriteMode)
		return;	
  this->animationTimer += gameTime;
	if(this->animationTimer >= this->animation_ms){
		//if not in reverse mode
		if(!this->reverse){
			//we move the blitting rect by 1 and make sure we dont go off
			this->frame_current++;
			if(this->frame_current >= this->animation_count-1){
				this->frame_current = 0;
				this->looped = true;
			}
			if(this->frame_current >= this->animation_count / 2){
				this->halfLooped = true;
			}
		}else{
			//else we do the same thing but reverse mode
			this->frame_current--;
			if(this->frame_current < 0){
				this->frame_current = this->animation_count-1;
				this->looped = true;
			}
			if(this->frame_current < this->animation_count / 2){
				this->halfLooped = true;
			}
		}
		this->animationTimer -= this->animation_ms;
	}
}

void Surface::Draw(SDL_Surface * viewport, int x, int y){
	SDL_Rect DestR;
 
    DestR.x = x + (int)this->shiftPosition.X;
    DestR.y = y + (int)this->shiftPosition.Y;
 
	SDL_BlitSurface(this->surface, NULL, viewport, &DestR);
}

bool Surface::Draw(SDL_Surface * viewport, int x, int y, SDL_Rect rect)
{
	SDL_Rect destR;

	destR.x = x + (int)this->shiftPosition.X;
	destR.y = y + (int)this->shiftPosition.Y;

	SDL_Rect srcR;

	srcR.x = rect.x;
	srcR.y = rect.y;
	srcR.w = rect.w;
	srcR.h = rect.h;

	SDL_BlitSurface(this->surface, &srcR, viewport, &destR);

	return true;
}

void Surface::DrawSprite(SDL_Surface * viewport, int x, int y)
{
	if(!this->spriteMode)
		return;
	SDL_Rect destR;
	destR.x = x + (int)this->shiftPosition.X;
	destR.y = y + (int)this->shiftPosition.Y;

	SDL_Rect srcR;
	srcR.x = this->frame_width*this->frame_current;
	srcR.y = 0;
	srcR.w = this->frame_width;
	srcR.h = this->frame_height;

	SDL_BlitSurface(this->surface, &srcR, viewport, &destR);
}