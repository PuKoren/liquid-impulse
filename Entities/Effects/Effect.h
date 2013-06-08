#ifndef _EFFECT_H
#define _EFFECT_H
#include <SDL.h>
#include "../../Config.h"
#include "../../Tools/Surface.h"
#include "../../Maths/Rectangle.h"
#include <vector>
using namespace std;

class Effect
{
    private:
        Vector2 position;
        Vector2 direction;

        float velocity;
        int currentDelay;        
        int delay;
        bool effectEnded;

        Surface * surface;

    public:
        Effect(std::string, std::string, int, int, Vector2, float, int, bool);
		Effect(std::string, std::string, Vector2);
        ~Effect();

        void Init(float, int);
        void Load(std::string, std::string, int, int, Vector2, bool);
        void Load(std::string, std::string, Vector2);
		void Update(Uint32);
		void Draw(SDL_Surface *);

        void StartEffect(Vector2, Vector2);
        bool EffectEnded();
        
        Rectangle GetCollisionBox();
};

#endif

//#ifndef _EFFECT_H
//#define _EFFECT_H
//#include <SDL.h>
//#include "../../Config.h"
//#include "../../Tools/Surface.h"
//
///*
//*	EFFECT TYPE
//*	1 : Impulse Attack Effect
//*	2 : Repulse Attack Effect
//*	3 : Jump dust 
//*	4 : Air Bomb Attack Effect
//*/
//
///* IMPULSE PARAM */
//#define NB_SPRITES_IMPULSE_EFFECT 14
//#define EFFECT_WIDTH_IMPULSE_ATTACK 250
//#define EFFECT_HEIGHT_IMPULSE_ATTACK 287
//#define EFFECT_SPEED_IMPULSE_ATTACK 25
//#define ANIMATION_TIMER_IMPULSE_ATTACK 20
//
///* REPULSE PARAM */
//#define NB_SPRITES_REPULSE_EFFECT 12
//#define EFFECT_WIDTH_REPULSE_ATTACK 250
//#define EFFECT_HEIGHT_REPULSE_ATTACK 210
//#define ANIMATION_TIMER_REPULSE_EFFECT 40
//
///* DUST PARAM */
//#define NB_SPRITES_DUST_EFFECT 17
//#define EFFECT_WIDTH_DUST 175
//#define EFFECT_HEIGHT_DUST 175
//#define ANIMATION_TIMER_DUST 30
//
///* AIR BOMB PARAM */
//#define NB_SPRITES_AIR_BOMB_EFFECT 17
//#define EFFECT_WIDTH_AIR_BOMB 250
//#define EFFECT_HEIGHT_AIR_BOMB 287
//#define ANIMATION_TIMER_AIR_BOMB 40
//
//class Effect
//{
//	public:
//		Effect(int type);
//		~Effect();
//
//		void Event(SDL_Event *);
//		void Update(Uint32);
//		void Draw(SDL_Surface *);
//
//		void SetMoveClips();    
//		void Start(int x, int y, int direction);
//		void Finish();
//		bool Animating();
//
//	private:
//		int type;
//		int posX;
//		int posY;
//		int velocityX;
//		int frame;
//		int direction;
//		int animationStatus;
//		int animationTimer;
//		bool started;
//
//		Surface * surfMove;
//		Surface * surfMoveLeft;
//
//		SDL_Rect clipMove[NB_SPRITES_IMPULSE_EFFECT];
//		SDL_Rect clipMoveLeft[NB_SPRITES_IMPULSE_EFFECT];
//};
//
//#endif
//
