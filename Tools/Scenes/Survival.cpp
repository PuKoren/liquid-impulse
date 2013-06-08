#include "Survival.h"
#include "../Tools/DrawString.h"
#include "../Tools/Colors.h"

int HeroLastPosition = 0;
Survival::Survival(){
	this->backgroundSurfaces = new BackgroundManager("survival", 1);
    this->hero = new Hero(&this->heroProjectiles);
	this->Load();
	this->HeroPosition = RES_WIDTH/2;
	this->BackgroundForce = 2.5;
	this->freeMove = false;
	this->hero->SetPositionX(this->HeroPosition);
	this->HeroLastPosition = this->HeroPosition; 
	this->HeroLastPositionZ = this->hero->GetPositionZ();
	this->backgroundSurfaces->MoveY(400);
	this->arrow_right = new Surface("Resources/Textures/UI/arrow_right.png");
	this->hero_icon = new Surface("Resources/Textures/UI/hero_icon.png");
	this->virtualX = 0;
}

Survival::~Survival(){
	delete this->backgroundSurfaces;
	delete this->hero;
	delete this->hero_icon;
	delete this->arrow_right;
	Mix_FreeMusic(this->backgroundMusic);
}

void Survival::Load(){
	this->backgroundMusic = Mix_LoadMUS("Resources/Sounds/Music/Danjyon_Kimura_Nazca.mp3");
	Mix_PlayMusic(this->backgroundMusic,-1);
}

void Survival::Event(SDL_Event *e, GameState *gs){
	if(e->key.keysym.sym == SDLK_ESCAPE){
		*gs = GAME_LOAD_MENU;
	}
	this->hero->Event(e);
}


void Survival::Update(Uint32 gameTime){
	this->hero->Update(gameTime, this->heroProjectiles);

	this->backgroundSurfaces->Move((int)(this->hero->GetPosition().X - this->HeroLastPosition));
	this->backgroundSurfaces->MoveY((int)(this->hero->GetPositionZ() - this->HeroLastPositionZ));

	this->HeroLastPositionZ = this->hero->GetPositionZ();
	this->HeroLastPosition = this->hero->GetPosition().X;
}

void Survival::Draw(SDL_Surface * viewport){
	//background
	this->backgroundSurfaces->Draw(viewport);

	//hero
	this->hero->Draw(viewport);

	//hero life
	this->hero_icon->Draw(viewport, 10, 10);
	int i = 0;
    for(i = 0; i < 100; i+=2){
		DrawString::Draw(viewport, "|", FONT_INGAME_UI, RED, this->hero_icon->GetWidth()+15 + i*2, 10);
	}
	for(i = 0; i < this->hero->GetHealth(); i+=2){
		DrawString::Draw(viewport, "|", FONT_INGAME_UI, YELLOW, this->hero_icon->GetWidth()+15 + i*2, 10);
	}
	
	DrawString::Draw(viewport, "*", FONT_INGAME_UI, WHITE, this->hero_icon->GetWidth()+15, 40);
	DrawString::Draw(viewport, this->hero->GetLife(), FONT_INGAME_UI, WHITE, this->hero_icon->GetWidth()+40, 40);
}
