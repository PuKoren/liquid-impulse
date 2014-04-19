#ifndef _Level_H
#define _Level_H
#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <random>
#include "Hero.h"
#include "Enemy.h"
#include "Projectile.h"
#include "ParticleEngine.h"
#include "BackgroundManager.h"
#include "Surface.h"
#include "Config.h"

using namespace std;

class Level{
	public:
        Level(bool survival);
		~Level();
		void Draw(SDL_Surface *);
		void Event(SDL_Event *, GameState *);
        void Update(Uint32, GameState*);
        int GetScore();

	private:
		Hero *hero;
		std::vector<Enemy*> enemies;
		std::vector<Projectile*> enemiesProjectiles;
		std::vector<Projectile*> heroProjectiles;
		ParticleEngine bloodEngine;
        bool survival;

		float HeroPosition;
        float HeroLastPosition;
        float HeroLastPositionZ;
		int Score;
		int ScoreMultiplier;
		bool freeMove;
		bool firstRun;
        bool paused;
		float BackgroundForce;
		float gravity;

		Mix_Music *backgroundMusic;
		BackgroundManager *backgroundSurfaces;
		Surface* arrow_right;
		Surface* hero_icon;
        SDL_Surface* arrow_text;
        SDL_Surface* score_text;
        SDL_Surface* lifes_text;

		int virtualX;
		int wave;
		int maxWave;
		int difficulty;
        int survivalWaveCurrentTimer;
        int survivalWaveTimer;
		void Load();
		void GenerateWave();
        void UpdateEnemies(Uint32);
		void SpreadBlood(Vector2, Vector2, float);
		void CollisionDetection(Uint32);
};

#endif
