#include "BackgroundManager.h"
#include <sstream>

BackgroundManager::BackgroundManager(std::string backgroundName, int count){
	this->backgrounds = new Surface *[count];
	this->count = count;
	this->position = 0;
	this->totalWidth = 0;
	this->positionY = 0;
	for(int i = 0; i < count; i++){
		std::ostringstream s;
		s << "Resources/Textures/Backgrounds/" << backgroundName << "_" << i << ".png";
		std::string bg(s.str());
		this->backgrounds[i] = new Surface(bg);
		this->totalWidth += this->backgrounds[i]->GetWidth();
	}
}

BackgroundManager::~BackgroundManager(){
	for(int i = 0; i < this->count; i ++){
		delete this->backgrounds[i];
	}
	delete[] this->backgrounds;
}

void BackgroundManager::Draw(SDL_Surface *viewport){
    //int lastX = this->totalWidth - this->backgrounds[this->count-1]->GetWidth();
	int currentX = 0;
	for(int i = 0; i < this->count; i ++){
		if(this->position + currentX > - RES_WIDTH && this->position + currentX < RES_WIDTH){
			this->backgrounds[i]->Draw(viewport, this->position + currentX, this->positionY);
		}
		currentX += this->backgrounds[i]->GetWidth();
	}
}

void BackgroundManager::Move(int direction){
	if(this->position > -this->totalWidth + RES_WIDTH + direction && this->position - direction <= 0){
		this->position -= direction;
	}
}

void BackgroundManager::MoveY(int direction){
	this->positionY -= direction;
}
