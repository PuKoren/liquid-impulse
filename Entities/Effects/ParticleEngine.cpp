#include "ParticleEngine.h"

ParticleEngine::ParticleEngine(){

}

ParticleEngine::ParticleEngine(std::string particleName, float _gravity){
	this->surf = new Surface("Resources/Effects/Particles/"+particleName+".png");
	this->name = particleName;
	this->gravity = _gravity;
}

ParticleEngine::ParticleEngine(const ParticleEngine& p):name(p.name), gravity(p.gravity){
	this->surf = new Surface("Resources/Effects/Particles/"+this->name+".png");
}

ParticleEngine::~ParticleEngine(){
	delete this->surf;
}

ParticleEngine& ParticleEngine::operator=(const ParticleEngine& p){
	this->name = p.name;
	this->gravity = p.gravity;
	this->surf = new Surface("Resources/Effects/Particles/"+this->name+".png");
	return *this;
}

void ParticleEngine::AddParticle(Vector2 position, Vector2 direction, float speed, int lifeTime){
	this->particles.push_back(Particle(this->surf, position, direction, speed, lifeTime));
}

void ParticleEngine::Update(Uint32 gameTime){
	unsigned int size = this->particles.size();
	for(unsigned int i = 0; i < size; i++){
		this->particles[i].Update(gameTime, this->gravity);
		if(!this->particles[i].IsAlive()){
			this->particles.erase(this->particles.begin()+i);
			size--;
			i--;
		}
	}
}

void ParticleEngine::Draw(SDL_Surface* viewport){
	unsigned int size = this->particles.size();
	for(unsigned int i = 0; i < size; i++){
		this->particles[i].Draw(viewport);
	}
}

std::string ParticleEngine::GetName(){
	return this->name;
}
