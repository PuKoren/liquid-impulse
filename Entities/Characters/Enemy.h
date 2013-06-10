#ifndef _ENEMY_H
#define _ENEMY_H
#include <SDL.h>
#include <vector>
#include <string>
#include <time.h>
#include <random>
#include "Hero.h"
#include "../Effects/Projectile.h"
#include "../../Maths/Rectangle.h"
#include "../../Maths/Vector2.h"
#include "../../Tools/Surface.h"
using namespace std;

enum ANIMATION_STATE{
	ENEMY_RUN_LEFT,
	ENEMY_STAND_LEFT,
	ENEMY_PUNCH_LEFT,
	ENEMY_JUMP_LEFT_UP,
	ENEMY_JUMP_LEFT_DOWN,
	ENEMY_PUNCH_AIR_LEFT,
	ENEMY_HIT_LEFT,
	ENEMY_HARD_HIT_LEFT,

	ENEMY_RUN_RIGHT,
	ENEMY_STAND_RIGHT,
	ENEMY_PUNCH_RIGHT,
	ENEMY_JUMP_RIGHT_UP,
	ENEMY_JUMP_RIGHT_DOWN,
	ENEMY_PUNCH_AIR_RIGHT,
	ENEMY_HIT_RIGHT,
	ENEMY_HARD_HIT_RIGHT,

	ENEMY_DIE_LEFT,
	ENEMY_DIE_RIGHT
};

class Enemy{
	private:
		int life;
		float velocity;
		int orientation;
		int handicap;
		int handicapTimer;

		float jump_velocity_current;
		float jump_velocity;
		float gravity;
		float projectedY;

		bool jumping;
		bool attacking;
		int projected;
		bool hit;
		bool alive;
		bool copyCreated;

		Surface** animationList;
		ANIMATION_STATE animationState;

		Vector2 position;
		Vector2 jump_position;
		Vector2 direction;

		int lastProjectileId;
		void Init(int, float, int);
		void RefreshAnimation(Hero&);
		void ChangeAnimation(ANIMATION_STATE);
		bool IsHeroFarAway(Hero&);		
		Projectile* CreateProjectile();

	public:
		Enemy();
		Enemy(std::string, int, float, int);
		Enemy(std::string);
		Enemy(Enemy&, int, float, int);
		Enemy(Enemy&);
		~Enemy();

		Vector2 GetPosition();
		Vector2 GetFootPosition();
		Rectangle GetCollisionBox();
		int GetLife();
		float GetVelocity();
		int GetHandicap();
		float GetPositionZ();

		bool Hit(Projectile*);
		bool IsAlive();

		void Load(std::string);
		void Load(Surface**);
		void Update(Uint32, Hero&, std::vector<Projectile*>&, std::vector<Projectile*>&);
		void Draw(SDL_Surface*);
		int Projected(Projectile*);

		Surface** GetAnimationList();
};

#endif
