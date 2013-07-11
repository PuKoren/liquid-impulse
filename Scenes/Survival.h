#ifndef _SURVIVAL_H
#define _SURVIVAL_H
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include "../Entities/Characters/Hero.h"
#include "../Tools/BackgroundManager.h"
#include "../Tools/Surface.h"
#include "../Config.h"

using namespace std;

class Survival{
    public:
        Survival();
        ~Survival();
        void Draw(SDL_Surface *);
        void Event(SDL_Event *, GameState *);
        void Update(Uint32);

    private:
        Hero *hero;
        float HeroPosition;
        float HeroLastPosition;
        float HeroLastPositionZ;
        bool freeMove;
        float BackgroundForce;
        Mix_Music *backgroundMusic;
        BackgroundManager *backgroundSurfaces;
        Surface* arrow_right;
        Surface* hero_icon;
        int virtualX;
        void Load();
        std::vector<Projectile*> heroProjectiles;
};

#endif
