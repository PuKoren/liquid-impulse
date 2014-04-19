#ifndef _PARTICLEENGINE_H
#define _PARTICLEENGINE_H

#include <string>
#include <vector>
#include <SDL.h>
#include "Particle.h"
#include "Vector2.h"

using namespace std;

class ParticleEngine{
	private:
		Surface* surf;
		std::string name;
		std::vector<Particle> particles;
		float gravity;

	public:
		ParticleEngine();
		ParticleEngine(const ParticleEngine&);
		ParticleEngine(std::string, float);
		~ParticleEngine();

		void AddParticle(Vector2 position, Vector2 direction, float speed, int lifeTime);
		void Update(Uint32);
		void Draw(SDL_Surface*);
		std::string GetName();

		ParticleEngine& operator=( const ParticleEngine&);
};

#endif