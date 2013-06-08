#ifndef _SETTINGS_H
#define _SETTINGS_H
#include "../Libraries/MinIni/minIni.h"

class Settings{
	public:
		Settings();
		~Settings();
		void Load();
		void Save();
		void RaiseMusicVolume();
		void LowerMusicVolume();
		void RaiseEffectVolume();
		void LowerEffectVolume();
		void ToggleFullScreen();
		void ToggleShowFPS();
		void ToggleLimitFPS();
		int GetMusicVolume();
		int GetEffectVolume();
		int GetColors();
		bool IsFullScreen();
		bool LimitFps();
		bool ShowFps();
	private:
		int MusicVolume;
		int EffectVolume;
		int Colors;
		bool full_screen;
		bool show_fps;
		bool limit_fps;
};

#endif