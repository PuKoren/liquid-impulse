#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include <SDL.h>
#include "Config.h"
#include "Rectangle.h"
#include "Vector2.h"

class Projectile{
	private:
		Rectangle collisionBox;
		float velocity;
		float power;
		Vector2 direction;
		int lifeTime;
		int elapsedLifeTime;
		bool alive;
		int collisions;
		int waitTime;
		static int projId;

	public:
		Projectile();
		Projectile(Rectangle collisionBox, float, float, Vector2, int);
		Projectile(Rectangle collisionBox, float, float, Vector2, int, int);
		~Projectile();
		
		int GetId();
		void Update(Uint32);
		void Draw(SDL_Surface*);
		Rectangle GetCollisionBox();
		Vector2 GetDirection();
		bool IsAlive();
		float GetPower();
		void SetAlive(bool);
		void AddCollision();
		int GetCollision();
};

#endif