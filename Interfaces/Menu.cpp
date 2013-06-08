#include "Menu.h"
#include <fstream>

int bg_01_position = 0;
int bg_02_position = RES_WIDTH;
bool newSettings = false;

int titleFinalPosition = RES_WIDTH/2 - (380 * RATIO);
int titlePosition = RES_WIDTH;

int titleAnimationSpeed = 8*RATIO;
int selectedAnimationSpeed = 1;
int backgroundAnimationSpeed = 1;

int selectedItemPosition = 0;
int animationTimer = 20;
int animationStatus = 0;
int animationLength = 20;
int animationDirection = 1;

int creditsPosition = RES_HEIGHT;
int creditsAnimationSpeed = 1;
int creditsAnimationTimer = 20;
int creditsAnimationStatus = 0;

Menu::Menu(){
    this->menuTitle = NULL;
	this->selectedItem = 0;
	this->selectedOptionItem = 0;
	this->menuState = MENU_MAIN;
	this->settings = new Settings();
	newSettings = true;
	this->background = NULL;
	this->Load();
}

Menu::Menu(Settings *config){
    this->menuTitle = NULL;
	this->selectedItem = 0;
	this->selectedOptionItem = 0;
	this->menuState = MENU_MAIN;
	this->settings = config;
	this->background = NULL;
	this->Load();
}

Menu::~Menu(){
    SDL_FreeSurface(this->menuTitle);
    SDL_FreeSurface(this->surfYes);
    SDL_FreeSurface(this->surfNo);
    for(unsigned int i = 0; i < this->menuItems.size(); i++){
        SDL_FreeSurface(this->menuItems.at(i));
        SDL_FreeSurface(this->menuItemsY.at(i));
    }
    for(unsigned int i = 0; i < this->optionItems.size(); i++){
        SDL_FreeSurface(this->optionItems.at(i));
        SDL_FreeSurface(this->optionItemsY.at(i));
    }

    for(unsigned int i = 0; i < this->credits.size(); i++){
        SDL_FreeSurface(this->credits.at(i));
    }

	delete background;
	if(newSettings) delete this->settings;
	Mix_FreeMusic(this->backgroundMusic);
	Mix_FreeChunk(this->effectSound);
	Mix_FreeChunk(this->menuChange);
	Mix_FreeChunk(this->menuSelect);
}

void Menu::Load(){
    this->menuItems.push_back(DrawString::GetSurface("Start Game", FONT_MENU, WHITE));
	this->menuItems.push_back(DrawString::GetSurface("Survival", FONT_MENU, WHITE));
	this->menuItems.push_back(DrawString::GetSurface("Options", FONT_MENU, WHITE));
	this->menuItems.push_back(DrawString::GetSurface("Credits", FONT_MENU, WHITE));
	this->menuItems.push_back(DrawString::GetSurface("Exit", FONT_MENU, WHITE));

	this->optionItems.push_back(DrawString::GetSurface("Music", FONT_MENU, WHITE));
	this->optionItems.push_back(DrawString::GetSurface("SFX", FONT_MENU, WHITE));
	this->optionItems.push_back(DrawString::GetSurface("Windowed", FONT_MENU, WHITE));
	this->optionItems.push_back(DrawString::GetSurface("Limit FPS", FONT_MENU, WHITE));
	this->optionItems.push_back(DrawString::GetSurface("Show FPS", FONT_MENU, WHITE));

    this->menuItemsY.push_back(DrawString::GetSurface("Start Game", FONT_MENU, YELLOW));
	this->menuItemsY.push_back(DrawString::GetSurface("Survival", FONT_MENU, YELLOW));
	this->menuItemsY.push_back(DrawString::GetSurface("Options", FONT_MENU, YELLOW));
	this->menuItemsY.push_back(DrawString::GetSurface("Credits", FONT_MENU, YELLOW));
	this->menuItemsY.push_back(DrawString::GetSurface("Exit", FONT_MENU, YELLOW));

	this->optionItemsY.push_back(DrawString::GetSurface("Music", FONT_MENU, YELLOW));
	this->optionItemsY.push_back(DrawString::GetSurface("SFX", FONT_MENU, YELLOW));
	this->optionItemsY.push_back(DrawString::GetSurface("Windowed", FONT_MENU, YELLOW));
	this->optionItemsY.push_back(DrawString::GetSurface("Limit FPS", FONT_MENU, YELLOW));
	this->optionItemsY.push_back(DrawString::GetSurface("Show FPS", FONT_MENU, YELLOW));

    this->surfYes = DrawString::GetSurface("Yes", FONT_MENU, WHITE);
    this->surfNo = DrawString::GetSurface("No", FONT_MENU, WHITE);

    this->menuTitle = DrawString::GetSurface("Liquid Impulse", FONT_TITLE, WHITE);

	this->background = new Surface("Resources/Textures/Backgrounds/menu.png");
	this->backgroundMusic = Mix_LoadMUS("Resources/Sounds/Music/Danjyon_Kimura_Teleporter.ogg");
	this->effectSound = Mix_LoadWAV("Resources/Sounds/Effects/Hit.wav");
	this->menuChange = Mix_LoadWAV("Resources/Sounds/Effects/MenuChange.wav");
	this->menuSelect = Mix_LoadWAV("Resources/Sounds/Effects/MenuSelect.wav");
	Mix_PlayMusic(this->backgroundMusic, -1);
}

void Menu::Event(SDL_Event *e, GameState *gs){
	if(this->menuState == MENU_MAIN){
		if(e->type == SDL_KEYDOWN){
			if(e->key.keysym.sym == SDLK_DOWN){
				if(this->selectedItem < this->menuItems.size()-1){
					this->selectedItem += 1;
				}else{
					this->selectedItem = 0;
				}
				selectedItemPosition = 0;
				Mix_PlayChannel(-1, this->menuChange, false);
			}else if(e->key.keysym.sym == SDLK_UP){
				if(this->selectedItem > 0){
					this->selectedItem -= 1;
				}else{
					this->selectedItem = this->menuItems.size()-1;
				}
				selectedItemPosition = 0;
				Mix_PlayChannel(-1, menuChange, false);
			}else if(e->key.keysym.sym == SDLK_RETURN){
				Mix_PlayChannel(-1, this->menuSelect, false);
				if(this->selectedItem == 0){
					*gs = GAME_LOAD_GAME;
					Mix_PlayChannel(0, menuSelect, false);
				}else if(this->selectedItem == 1){
					*gs = GAME_LOAD_SURVIVAL;
					Mix_PlayChannel(0, menuSelect, false);
				}else if(this->selectedItem == 2){
					this->menuState = MENU_OPTIONS;
				}else if(this->selectedItem == 3){
					this->menuState = MENU_CREDITS;
				}else if(this->selectedItem == 4){
					e->type = SDL_QUIT;
				}
			}else if(e->key.keysym.sym == SDLK_ESCAPE){
				e->type = SDL_QUIT;
			}
		}
	}else if(this->menuState == MENU_OPTIONS){
		if(e->type == SDL_KEYDOWN){
			if(e->key.keysym.sym == SDLK_DOWN){
				if(this->selectedOptionItem < this->optionItems.size()-1){
					this->selectedOptionItem += 1;
				}else{
					this->selectedOptionItem = 0;
				}
				Mix_PlayChannel(-1, this->menuChange, false);
			}else if(e->key.keysym.sym == SDLK_UP){
				if(this->selectedOptionItem > 0){
					this->selectedOptionItem -= 1;
				}else{
					this->selectedOptionItem = this->optionItems.size()-1;
				}
				Mix_PlayChannel(-1, this->menuChange, false);
            }else if(e->key.keysym.sym == SDLK_ESCAPE || e->key.keysym.sym == SDLK_d){
				this->selectedOptionItem = 0;
				selectedItemPosition = 0;
				this->menuState = MENU_MAIN;
				Mix_PlayChannel(-1, this->effectSound, false);
			}else if(e->key.keysym.sym == SDLK_RIGHT){
				if(this->selectedOptionItem == 2){
					#ifndef PSP
					this->settings->ToggleFullScreen();
                    SDL_Surface *screen = SDL_GetVideoSurface();
                    if(this->settings->IsFullScreen() && screen != NULL){
						screen = SDL_SetVideoMode(RES_WIDTH, RES_HEIGHT, this->settings->GetColors(), SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
                    }else{
						screen = SDL_SetVideoMode(RES_WIDTH, RES_HEIGHT, this->settings->GetColors(), SDL_HWSURFACE | SDL_DOUBLEBUF);
					}
					#endif
				}else if(this->selectedOptionItem == 0){
					this->settings->RaiseMusicVolume();
					Mix_VolumeMusic(this->settings->GetMusicVolume());
				}else if(this->selectedOptionItem == 1){
					this->settings->RaiseEffectVolume();
					Mix_Volume(-1, this->settings->GetEffectVolume());
					Mix_PlayChannel(-1, this->effectSound, false);
				}else if(this->selectedOptionItem == 3){
					this->settings->ToggleLimitFPS();
				}else if(this->selectedOptionItem == 4){
					this->settings->ToggleShowFPS();
				}
			}else if(e->key.keysym.sym == SDLK_LEFT){
				if(this->selectedOptionItem == 2){
					#ifndef PSP
					this->settings->ToggleFullScreen();
					SDL_Surface *screen = SDL_GetVideoSurface();
                    if(this->settings->IsFullScreen() && screen != NULL){
						screen = SDL_SetVideoMode(RES_WIDTH, RES_HEIGHT, this->settings->GetColors(), SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
					}else{
						screen = SDL_SetVideoMode(RES_WIDTH, RES_HEIGHT, this->settings->GetColors(), SDL_HWSURFACE | SDL_DOUBLEBUF);
					}
					#endif
				}else if(this->selectedOptionItem == 0){
					this->settings->LowerMusicVolume();
					Mix_VolumeMusic(this->settings->GetMusicVolume());
				}else if(this->selectedOptionItem == 1){
					this->settings->LowerEffectVolume();
					Mix_Volume(-1, this->settings->GetEffectVolume());
					Mix_PlayChannel(-1, this->effectSound, false);
				}else if(this->selectedOptionItem == 3){
					this->settings->ToggleLimitFPS();
				}else if(this->selectedOptionItem == 4){
					this->settings->ToggleShowFPS();
				}
			}
		}
	}else if(this->menuState == MENU_CREDITS){
		if(e->key.keysym.sym == SDLK_ESCAPE){
			selectedItemPosition = 0;
			creditsPosition = RES_HEIGHT;
			this->menuState = MENU_MAIN;
		}
	}
}

void Menu::Update(Uint32 gameTime){
	animationStatus += gameTime;
	if(animationStatus > animationTimer){
		animationStatus -= animationTimer;
		selectedItemPosition += animationDirection * selectedAnimationSpeed;

		if(titlePosition > titleFinalPosition){
			titlePosition -= titleAnimationSpeed;
		}

		if(selectedItemPosition > animationLength){
			animationDirection = -1;
		}else if(selectedItemPosition <= 0){
			animationDirection = 1;
		}
		
		bg_01_position -= backgroundAnimationSpeed;
		bg_02_position -= backgroundAnimationSpeed;
		if(bg_01_position <= -RES_WIDTH){
			bg_01_position = RES_WIDTH;
		}
		if(bg_02_position <= -RES_WIDTH){
			bg_02_position = RES_WIDTH;
		}
	}

	if(this->menuState == MENU_CREDITS){
		if(this->credits.size() <= 1){
			std::ifstream myfile ("credits.txt");
			if (myfile.is_open()){
				std::string line;
                SDL_Color color = YELLOW;
				while(myfile.good()){
					std::getline(myfile, line);
                    if(line.size() < 1){
                        this->credits.push_back(DrawString::GetSurface(" ", FONT_MENU, color));
                        color = YELLOW;
                    }else{
                        this->credits.push_back(DrawString::GetSurface(line, FONT_MENU, color));
                        color = WHITE;
                    }
				}
				myfile.close();
			}
		}else{
			creditsAnimationStatus += gameTime;
			if(creditsAnimationStatus > creditsAnimationTimer){
				creditsPosition -= creditsAnimationSpeed;
				creditsAnimationStatus -= creditsAnimationTimer;
			}
			
			if(creditsPosition + (this->credits.size()*30) <= 0){
				this->menuState = MENU_MAIN;
				creditsPosition = RES_HEIGHT;
			}
		}
	}
}

void Menu::Draw(SDL_Surface *viewport){
	background->Draw(viewport, bg_01_position, 0);
	background->Draw(viewport, bg_02_position, 0);
	if(this->menuState == MENU_MAIN){
		for(unsigned int i = 0; i < this->menuItems.size(); i ++){
            SDL_Rect DestR; DestR.x = RES_WIDTH/3; DestR.y = RES_HEIGHT/2 + i*30*RATIO;
			if(this->selectedItem == i){
                DestR.x += selectedItemPosition;
                SDL_BlitSurface(this->menuItemsY.at(i), NULL, viewport, &DestR);
			}else{
                SDL_BlitSurface(this->menuItems.at(i), NULL, viewport, &DestR);
            }
		}
	}else if(this->menuState == MENU_OPTIONS){
		for(unsigned int i = 0; i < this->optionItems.size(); i++){
			int spacing = 40*RATIO;
            SDL_Rect DestR; DestR.x = RES_WIDTH/4; DestR.y = RES_HEIGHT/2 + i*spacing;
            if(i == this->selectedOptionItem){
                SDL_BlitSurface(this->optionItemsY.at(i), NULL, viewport, &DestR);
            }else{
                SDL_BlitSurface(this->optionItems.at(i), NULL, viewport, &DestR);
            }
			if(i == 0){
				int j = 0;
                for(j = 0; j < this->settings->GetMusicVolume(); j += 6){
                    DestR.x = RES_WIDTH/2 + j*2*RATIO; DestR.y = RES_HEIGHT/2 + i*spacing; DestR.h = 30 * RATIO; DestR.w = 10 * RATIO;
                    SDL_FillRect(viewport, &DestR, SDL_MapRGB(viewport->format, YELLOW.r, YELLOW.g, YELLOW.b));
				}
                for(j = j; j < 128; j += 6){
                    DestR.x = RES_WIDTH/2 + j*2*RATIO; DestR.y = RES_HEIGHT/2 + i*spacing;
                    SDL_FillRect(viewport, &DestR, SDL_MapRGB(viewport->format, WHITE.r, WHITE.g, WHITE.b));
				}
			}else if(i==1){
				int j = 0;
                for(j = 0; j < this->settings->GetEffectVolume(); j += 6){
                    DestR.x = RES_WIDTH/2 + j*2*RATIO; DestR.y = RES_HEIGHT/2 + i*spacing; DestR.h = 30 * RATIO; DestR.w = 10 * RATIO;
                    SDL_FillRect(viewport, &DestR, SDL_MapRGB(viewport->format, YELLOW.r, YELLOW.g, YELLOW.b));
				}
                for(j = j; j < 128; j += 6){
                    DestR.x = RES_WIDTH/2 + j*2*RATIO; DestR.y = RES_HEIGHT/2 + i*spacing;
					SDL_FillRect(viewport, &DestR, SDL_MapRGB(viewport->format, WHITE.r, WHITE.g, WHITE.b));
				}
			}else if(i==2){
                DestR.x = RES_WIDTH/2 + 100*RATIO; DestR.y = RES_HEIGHT/2 + i*spacing;
				if(this->settings->IsFullScreen()){
                    SDL_BlitSurface(this->surfNo, NULL, viewport, &DestR);
				}else{
                    SDL_BlitSurface(this->surfYes, NULL, viewport, &DestR);
				}
			}else if(i == 3){
                DestR.x = RES_WIDTH/2 + 100*RATIO; DestR.y = RES_HEIGHT/2 + i*spacing;
				if(this->settings->LimitFps()){
					SDL_BlitSurface(this->surfYes, NULL, viewport, &DestR);
				}else{
					SDL_BlitSurface(this->surfNo, NULL, viewport, &DestR);
				}
			}else if(i == 4){
                DestR.x = RES_WIDTH/2 + 100*RATIO; DestR.y = RES_HEIGHT/2 + i*spacing;
				if(this->settings->ShowFps()){
                    SDL_BlitSurface(this->surfYes, NULL, viewport, &DestR);
				}else{
					SDL_BlitSurface(this->surfNo, NULL, viewport, &DestR);
				}
			}
		}
	}else if(this->menuState == MENU_CREDITS){
		for(unsigned int i = 0; i < this->credits.size(); i++){
            SDL_Rect DestR; DestR.x = (RES_WIDTH /2) - (this->credits.at(i)->w/2); DestR.y = creditsPosition + i*30*RATIO;
            SDL_BlitSurface(this->credits.at(i), NULL, viewport, &DestR);
		}
	}
    SDL_Rect DestR; DestR.x = titlePosition - (RES_HEIGHT - creditsPosition)*2; DestR.y = 20;
    SDL_BlitSurface(this->menuTitle, NULL, viewport, &DestR);
}
