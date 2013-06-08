#ifndef _PARTICLE_H
#define _PARTICLE_H
#include <SDL.h>
#include "../../Tools/Surface.h"
#include "../../Maths/Vector2.h"

class Particle{
	private:
		Surface* surf;
		Vector2 position;
		Vector2 direction;
		float speed;
		int lifeTime;
		
	public:
		Particle();
		Particle(Surface* _Surface, Vector2 _position, Vector2 _direction, float _speed, int _lifeTime);
		~Particle();

		void Update(Uint32, float);
		void Draw(SDL_Surface*);
		bool IsAlive();
};

#endif