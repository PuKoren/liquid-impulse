#include "Application.h"

int fps = 0;
int running = true;

Application::Application(){
	this->Viewport = NULL;
	this->menu = NULL;
	this->Running = false;
	this->level = NULL;
	this->survival = NULL;
	this->settings = new Settings();
	this->state = GAME_INIT;
    this->loadingSurface = NULL;
    this->fpsSurface = NULL;
    this->joystick = NULL;
    this->gameover = NULL;
}


Application::~Application(){
    SDL_FreeSurface(this->loadingSurface);
    SDL_FreeSurface(this->Viewport);
    SDL_FreeSurface(this->fpsSurface);
	delete this->menu;
	if(this->level != NULL){
		delete this->level;
	}
	if(this->survival != NULL){
		delete this->survival;
	}
	delete this->settings;
    DrawString::Clear();
	Mix_CloseAudio();
    SDL_JoystickClose(this->joystick);
    TTF_Quit();
    SDL_Quit();
}

int Application::Run(){
	if(this->Init() == false) {
        return -1;
    }

	this->Running = true;
	running = true;
	this->state = GAME_MENU;
	this->menu = new Menu(this->settings);
	
    SDL_Event Event;
    #if PSP
    SceCtrlData pad;
    sceCtrlReadBufferPositive(&pad, 1);
    unsigned int previousPadStatus = pad.Buttons;
    #endif

	float delaytime;
	float waittime = 1000.0f/FPS;
	int fpsTime = 0;
	int fpsCounter = 0;

	Uint32 previousTicks = SDL_GetTicks();
	while(running) {
	
		#if PSP
            this->PSPControls(pad, Event, previousPadStatus);
		#else

		//event logic
		while(SDL_PollEvent(&Event)) {
            this->Event(&Event, true);
		}
		#endif
		Uint32 ticks = SDL_GetTicks();

		//draw logic
		Draw(this->Viewport);
		//update logic
		Update(ticks - previousTicks);

		//fps counter
		if(this->settings->ShowFps()){
			fpsCounter++;
			fpsTime += ticks - previousTicks;
			if(fpsTime > 1000){
                if(fps != fpsCounter){
                    SDL_FreeSurface(this->fpsSurface);
                    if(fpsCounter < 30){
                        this->fpsSurface = DrawString::GetSurface(fpsCounter, FONT_INGAME, RED);
                    }else if(fpsCounter < 45){
                        this->fpsSurface = DrawString::GetSurface(fpsCounter, FONT_INGAME, YELLOW);
                    }else{
                        this->fpsSurface = DrawString::GetSurface(fpsCounter, FONT_INGAME, WHITE);
                    }
                }
				fps = fpsCounter;
				fpsCounter = 0;
				fpsTime -= 1000;
			}
		}
		previousTicks = ticks;

		//slow down if too fast
		if(this->settings->LimitFps()){
			delaytime = waittime - (SDL_GetTicks() - previousTicks);
			if(delaytime > 0){
				SDL_Delay((Uint32)delaytime);
			}
		}
    }
    return 0;
}

void Application::Stop(){
	this->Running = false;
	running = false;
}

bool  Application::Init(){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        return false;
    }

    if(TTF_Init() < 0){
        return false;
    }

	if(this->settings->IsFullScreen()){
		this->Viewport = SDL_SetVideoMode(RES_WIDTH, RES_HEIGHT, this->settings->GetColors(), SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	}else{
		this->Viewport = SDL_SetVideoMode(RES_WIDTH, RES_HEIGHT, this->settings->GetColors(), SDL_HWSURFACE | SDL_DOUBLEBUF);
	}
    if(this->Viewport == NULL) {
        return false;
    }

    this->loadingSurface = DrawString::GetSurface("Loading ...", FONT_INGAME, WHITE);
    this->fpsSurface = DrawString::GetSurface(0, FONT_INGAME, WHITE);

	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
	Mix_Volume(-1, this->settings->GetEffectVolume());
	Mix_VolumeMusic(this->settings->GetMusicVolume());
	SDL_WM_SetCaption("Liquid Impulse", "SDL");
    //SDL_WM_SetIcon(SDL_LoadBMP("icon.png"), NULL);

    #if !PSP
    if(SDL_NumJoysticks() > 0){
        printf("Using %s as joystick.\n", SDL_JoystickName(0));
        SDL_JoystickEventState(SDL_ENABLE);
        this->joystick = SDL_JoystickOpen(0);
    }
    #endif
	return true;
}

int prevJoystickXValue = 0;
int prevJoystickYValue = 0;
void Application::JoyStickWrapper(SDL_Event* e){
    if(this->joystick != NULL){
        SDL_Event newEvent;
        newEvent.type = SDL_KEYDOWN;

        if(e->type == SDL_JOYBUTTONDOWN){
            switch(e->jbutton.button){
            case 0:
                if(this->state == GAME_MENU){
                    newEvent.key.keysym.sym = SDLK_RETURN;
                    this->Event(&newEvent, false);
                }
                newEvent.key.keysym.sym = SDLK_s;
                break;
            case 2:
                newEvent.key.keysym.sym = SDLK_q;
                break;
            case 3:
                newEvent.key.keysym.sym = SDLK_z;
                break;
            case 1:
                newEvent.key.keysym.sym = SDLK_d;
                break;
            case 4:
                newEvent.key.keysym.sym = SDLK_e;
                break;
            case 5:
                newEvent.key.keysym.sym = SDLK_e;
                break;
            case 6:
                newEvent.key.keysym.sym = SDLK_ESCAPE;
                break;
            case 7:
                newEvent.key.keysym.sym = SDLK_RETURN;
                break;
            default:
                return;
            }
            this->Event(&newEvent, false);

        }else if(e->type == SDL_JOYHATMOTION){
            newEvent.key.keysym.sym = SDLK_0;
            this->Event(&newEvent, false);

            if(e->jhat.value & SDL_HAT_UP){
                newEvent.key.keysym.sym = SDLK_UP;
                this->Event(&newEvent, false);
            }
            if(e->jhat.value & SDL_HAT_LEFT){
                newEvent.key.keysym.sym = SDLK_LEFT;
                this->Event(&newEvent, false);
            }
            if(e->jhat.value & SDL_HAT_RIGHT){
                newEvent.key.keysym.sym = SDLK_RIGHT;
                this->Event(&newEvent, false);
            }
            if(e->jhat.value & SDL_HAT_DOWN){
                newEvent.key.keysym.sym = SDLK_DOWN;
                this->Event(&newEvent, false);
            }
        }else if(e->type == SDL_JOYAXISMOTION){
            //TODO: make it works ! experimental, stick is a mess
            if(e->jaxis.which == 0){
                if ((e->jaxis.value <= -15000 ) || (e->jaxis.value >= 15000)){
                    if(e->jaxis.axis == 0){
                        /* Left-right movement code goes here */
                        if(e->jaxis.value < 0){
                            if(prevJoystickYValue >= 0){
                                newEvent.key.keysym.sym = SDLK_LEFT;
                            }
                        }else{
                            if(prevJoystickYValue <= 0){
                                newEvent.key.keysym.sym = SDLK_RIGHT;
                            }
                        }
                        prevJoystickYValue = e->jaxis.value;
                    }else if( e->jaxis.axis == 1){
                        /* Up-Down movement code goes here */
                        if(e->jaxis.value < 0){
                            if(prevJoystickXValue >= 0){
                                newEvent.key.keysym.sym = SDLK_UP;
                            }
                        }else{
                            if(prevJoystickXValue <= 0){
                                newEvent.key.keysym.sym = SDLK_DOWN;
                            }
                        }
                        prevJoystickXValue = e->jaxis.value;
                    }
                    this->Event(&newEvent, false);
                }else if(e->jaxis.value < -8000 || e->jaxis.value > 8000){
                    newEvent.key.keysym.sym = SDLK_0;
                    prevJoystickXValue = 0;
                    prevJoystickYValue = 0;
                    this->Event(&newEvent, false);
                }
            }
        }
    }
}

void Application::Event(SDL_Event *e, bool joystick){
    if(joystick){
        this->JoyStickWrapper(e);
    }
	switch(this->state){
		case GAME_MENU:
			this->menu->Event(e, &this->state);
			break;
		case GAME_INGAME:
			this->level->Event(e, &this->state);
			break;
		case GAME_SURVIVAL:
			this->survival->Event(e, &this->state);
			break;
        case GAME_GAMEOVER:
            this->gameover->Event(e, &this->state);
            break;
		default:
			if(e->key.keysym.sym == SDLK_ESCAPE){
				this->Running = false;
				running = false;
			}
			break;
	}

	if(e->type == SDL_QUIT){
		this->Running = false;
		running = false;
	}
}

void Application::Update(Uint32 gameTime){
	switch(this->state){
		case GAME_MENU:
			this->menu->Update(gameTime);
			break;
		case GAME_INGAME:
            this->level->Update(gameTime, &this->state);
			break;
		case GAME_SURVIVAL:
            this->survival->Update(gameTime, &this->state);
			break;
		case GAME_LOAD_SURVIVAL:
            SDL_FillRect(this->Viewport, NULL, 0x0);
			delete this->menu;
			this->menu = NULL;
            this->survival = new Level(true);
			this->state = GAME_SURVIVAL;
			break;
		case GAME_LOAD_MENU:
            SDL_FillRect(this->Viewport, NULL, 0x0);
			if(this->level != NULL){
				delete this->level;
				this->level = NULL;
			}else if(this->survival != NULL){
				delete this->survival;
				this->survival = NULL;
			}
			this->menu = new Menu(this->settings);
			this->state = GAME_MENU;
			break;
		case GAME_LOAD_GAME:
            SDL_FillRect(this->Viewport, NULL, 0x0);
			delete this->menu;
			this->menu = NULL;
            this->level = new Level(false);
			this->state = GAME_INGAME;
			break;
        case GAME_GAMEOVER:
            if(this->level != NULL || this->survival != NULL){
                if(this->level != NULL){
                    this->gameover = new GameOver(this->level->GetScore());
                    delete this->level;
                    this->level = NULL;
                }else{
                    this->gameover = new GameOver(this->survival->GetScore());
                    delete this->survival;
                    this->survival = NULL;
                }
            }
            this->gameover->Update(gameTime);
            break;
		default:

			break;
	}
}

void Application::Draw(SDL_Surface *viewport){
	switch(this->state){
		case GAME_MENU:
			this->menu->Draw(viewport);
			break;
		case GAME_INGAME:
			this->level->Draw(viewport);
			break;
		case GAME_SURVIVAL:
			this->survival->Draw(viewport);
			break;
        case GAME_GAMEOVER:
            if(this->gameover != NULL){
                this->gameover->Draw(viewport);
            }
            break;
		default:
            SDL_FillRect(this->Viewport, NULL, 0x0);
            SDL_Rect loadingRect; loadingRect.x = RES_WIDTH - 170; loadingRect.y = RES_HEIGHT - 50;
            SDL_BlitSurface(this->loadingSurface, NULL, viewport, &loadingRect);
			break;
	}

	if(this->settings->ShowFps()){
        SDL_Rect fpsRect; fpsRect.x = RES_WIDTH-(50*RATIO); fpsRect.y = 5;
        SDL_BlitSurface(this->fpsSurface, NULL, viewport, &fpsRect);
	}

	SDL_Flip(viewport);
}

#if PSP
void Application::PSPControls(SceCtrlData& pad, SDL_Event& Event, unsigned int& previousPadStatus){
    sceCtrlReadBufferPositive(&pad, 1);
    if (pad.Buttons & PSP_CTRL_UP && !(previousPadStatus & PSP_CTRL_UP)){
        Event.type = SDL_KEYDOWN;
        Event.key.keysym.sym = SDLK_UP;
        this->Event(&Event, false);
    }else if(!(pad.Buttons & PSP_CTRL_UP) && previousPadStatus & PSP_CTRL_UP){
        Event.type = SDL_KEYUP;
        Event.key.keysym.sym = SDLK_UP;
        this->Event(&Event, false);
    }
    if (pad.Buttons & PSP_CTRL_DOWN && !(previousPadStatus & PSP_CTRL_DOWN)){
        Event.type = SDL_KEYDOWN;
        Event.key.keysym.sym = SDLK_DOWN;
        this->Event(&Event, false);
    }else if(!(pad.Buttons & PSP_CTRL_DOWN) && previousPadStatus & PSP_CTRL_DOWN){
        Event.type = SDL_KEYUP;
        Event.key.keysym.sym = SDLK_DOWN;
        this->Event(&Event, false);
    }
    if (pad.Buttons & PSP_CTRL_LEFT && !(previousPadStatus & PSP_CTRL_LEFT)){
        Event.type = SDL_KEYDOWN;
        Event.key.keysym.sym = SDLK_LEFT;
        this->Event(&Event, false);
    }else if(!(pad.Buttons & PSP_CTRL_LEFT) && previousPadStatus & PSP_CTRL_LEFT){
        Event.type = SDL_KEYUP;
        Event.key.keysym.sym = SDLK_LEFT;
        this->Event(&Event, false);
    }
    if (pad.Buttons & PSP_CTRL_RIGHT &&  !(previousPadStatus & PSP_CTRL_RIGHT)){
        Event.type = SDL_KEYDOWN;
        Event.key.keysym.sym = SDLK_RIGHT;
        this->Event(&Event, false);
    }else if(!(pad.Buttons & PSP_CTRL_RIGHT) && previousPadStatus & PSP_CTRL_RIGHT){
        Event.type = SDL_KEYUP;
        Event.key.keysym.sym = SDLK_RIGHT;
        this->Event(&Event, false);
    }

    if(this->state == GAME_MENU || this->state == GAME_CREDITS || this->state == GAME_OPTIONS){
        if (pad.Buttons & PSP_CTRL_CROSS &&  !(previousPadStatus & PSP_CTRL_CROSS)){
            Event.type = SDL_KEYDOWN;
            Event.key.keysym.sym = SDLK_RETURN;
            this->Event(&Event, false);
        }else if(!(pad.Buttons & PSP_CTRL_CROSS) && previousPadStatus & PSP_CTRL_CROSS){
            Event.type = SDL_KEYUP;
            Event.key.keysym.sym = SDLK_RETURN;
            this->Event(&Event, false);
        }
        if (pad.Buttons & PSP_CTRL_CIRCLE &&  !(previousPadStatus & PSP_CTRL_CIRCLE)){
            Event.type = SDL_KEYDOWN;
            Event.key.keysym.sym = SDLK_ESCAPE;
            this->Event(&Event, false);
        }else if(!(pad.Buttons & PSP_CTRL_CIRCLE) && previousPadStatus & PSP_CTRL_CIRCLE){
            Event.type = SDL_KEYUP;
            Event.key.keysym.sym = SDLK_ESCAPE;
            this->Event(&Event, false);
        }
    }else if(this->state == GAME_INGAME){
        if (pad.Buttons & PSP_CTRL_CROSS &&  !(previousPadStatus & PSP_CTRL_CROSS)){
            Event.type = SDL_KEYDOWN;
            Event.key.keysym.sym = SDLK_s;
            this->Event(&Event, false);
        }else if(!(pad.Buttons & PSP_CTRL_CROSS) && previousPadStatus & PSP_CTRL_CROSS){
            Event.type = SDL_KEYUP;
            Event.key.keysym.sym = SDLK_s;
            this->Event(&Event, false);
        }
        if (pad.Buttons & PSP_CTRL_CIRCLE &&  !(previousPadStatus & PSP_CTRL_CIRCLE)){
            Event.type = SDL_KEYDOWN;
            Event.key.keysym.sym = SDLK_d;
            this->Event(&Event, false);
        }else if(!(pad.Buttons & PSP_CTRL_CIRCLE) && previousPadStatus & PSP_CTRL_CIRCLE){
            Event.type = SDL_KEYUP;
            Event.key.keysym.sym = SDLK_d;
            this->Event(&Event, false);
        }
        if (pad.Buttons & PSP_CTRL_TRIANGLE &&  !(previousPadStatus & PSP_CTRL_TRIANGLE)){
            Event.type = SDL_KEYDOWN;
            Event.key.keysym.sym = SDLK_z;
            this->Event(&Event, false);
        }else if(!(pad.Buttons & PSP_CTRL_TRIANGLE) && previousPadStatus & PSP_CTRL_TRIANGLE){
            Event.type = SDL_KEYUP;
            Event.key.keysym.sym = SDLK_z;
            this->Event(&Event, false);
        }
        if (pad.Buttons & PSP_CTRL_SQUARE &&  !(previousPadStatus & PSP_CTRL_SQUARE)){
            Event.type = SDL_KEYDOWN;
            Event.key.keysym.sym = SDLK_q;
            this->Event(&Event, false);
        }else if(!(pad.Buttons & PSP_CTRL_SQUARE) && previousPadStatus & PSP_CTRL_SQUARE){
            Event.type = SDL_KEYUP;
            Event.key.keysym.sym = SDLK_q;
            this->Event(&Event, false);
        }
        if (pad.Buttons & PSP_CTRL_LTRIGGER &&  !(previousPadStatus & PSP_CTRL_LTRIGGER)){
            Event.type = SDL_KEYDOWN;
            Event.key.keysym.sym = SDLK_e;
            this->Event(&Event, false);
        }else if(!(pad.Buttons & PSP_CTRL_LTRIGGER) && previousPadStatus & PSP_CTRL_LTRIGGER){
            Event.type = SDL_KEYUP;
            Event.key.keysym.sym = SDLK_e;
            this->Event(&Event, false);
        }
        if (pad.Buttons & PSP_CTRL_RTRIGGER &&  !(previousPadStatus & PSP_CTRL_RTRIGGER)){
            Event.type = SDL_KEYDOWN;
            Event.key.keysym.sym = SDLK_e;
            this->Event(&Event, false);
        }else if(!(pad.Buttons & PSP_CTRL_RTRIGGER) && previousPadStatus & PSP_CTRL_RTRIGGER){
            Event.type = SDL_KEYUP;
            Event.key.keysym.sym = SDLK_e;
            this->Event(&Event, false);
        }
        if (pad.Buttons & PSP_CTRL_START &&  !(previousPadStatus & PSP_CTRL_START)){
            Event.type = SDL_KEYDOWN;
            Event.key.keysym.sym = SDLK_RETURN;
            this->Event(&Event, false);
        }else if(!(pad.Buttons & PSP_CTRL_START) && previousPadStatus & PSP_CTRL_START){
            Event.type = SDL_KEYUP;
            Event.key.keysym.sym = SDLK_RETURN;
            this->Event(&Event, false);
        }
    }
    previousPadStatus = pad.Buttons;
    Event.type = NULL;
    Event.key.keysym.sym = SDLK_TAB;

}
#endif
