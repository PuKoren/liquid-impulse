#include "Settings.h"

Settings::Settings(){
	this->Load();
}

Settings::~Settings(){
	this->Save();
}

void Settings::Load(){
    printf("Loading settings options.ini\n");
	minIni ini("options.ini");
	#if PSP
	this->full_screen = true;
	#else
	this->full_screen = ini.getbool("graphics", "fullscreen", false);
	#endif
	this->show_fps = ini.getbool("graphics", "fpsshow", false);
	this->limit_fps = ini.getbool("graphics", "fpslimit", false);
	this->Colors = ini.geti("graphics", "colors", 32);
	if(this->Colors != 16 && this->Colors != 24 && this->Colors != 32) this->Colors = 32;
	this->MusicVolume = ini.geti("sounds", "music", 64);
	this->EffectVolume = ini.geti("sounds", "effects", 64);
	if(this->MusicVolume > 128) 
		this->MusicVolume = 128;
	else if(this->MusicVolume < 0)
		this->MusicVolume = 64;

	if(this->EffectVolume > 128) 
		this->EffectVolume = 128;
	else if(this->EffectVolume < 0)
		this->EffectVolume = 64;
}

void Settings::Save(){
	minIni ini("options.ini");
	ini.put("graphics", "fullscreen", this->full_screen);
	ini.put("graphics", "colors", this->Colors);
	ini.put("graphics", "fpslimit", this->limit_fps);
	ini.put("graphics", "fpsshow", this->show_fps);
	ini.put("sounds", "music", this->MusicVolume);
	ini.put("sounds", "effects", this->EffectVolume);
}

void Settings::RaiseMusicVolume(){
	if(this->MusicVolume < 122){
		this->MusicVolume += 6;
	}else{
		this->MusicVolume = 128;
	}
}
void Settings::LowerMusicVolume(){
	if(this->MusicVolume > 6){
		this->MusicVolume -= 6;
	}else{
		this->MusicVolume = 0;
	}
}
void Settings::LowerEffectVolume(){
	if(this->EffectVolume > 6){
		this->EffectVolume -= 6;
	}else{
		this->EffectVolume = 0;
	}
}
void Settings::RaiseEffectVolume(){
	if(this->EffectVolume < 122){
		this->EffectVolume += 6;
	}else{
		this->EffectVolume = 128;
	}
}
void Settings::ToggleFullScreen(){
	this->full_screen = !this->full_screen;
}
void Settings::ToggleLimitFPS(){
	this->limit_fps = !this->limit_fps;
}
void Settings::ToggleShowFPS(){
	this->show_fps = !this->show_fps;
}
int Settings::GetMusicVolume(){
	return this->MusicVolume;
}
int Settings::GetEffectVolume(){
	return this->EffectVolume;
}
int Settings::GetColors(){
	return this->Colors;
}

bool Settings::IsFullScreen(){
	return this->full_screen;
}

bool Settings::ShowFps(){
	return this->show_fps;
}

bool Settings::LimitFps(){
	return this->limit_fps;
}
