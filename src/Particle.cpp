#include "Particle.h"

Particle::Particle(){
	this->surf = NULL;
	this->position = Vector2(0, 0);
	this->speed = 0.0f;
	this->direction = Vector2(0,0);
	this->lifeTime = 0;
}

Particle::Particle(Surface* _Surface, Vector2 _position, Vector2 _direction, float _speed, int _lifeTime){
	this->surf = _Surface;
	this->position = _position;
	this->direction = _direction;
	this->speed = _speed;
	this->lifeTime = _lifeTime;
}

Particle::~Particle(){

}

void Particle::Update(Uint32 gameTime, float gravity){
	this->position = this->position + this->direction * (this->speed * gameTime);
	this->direction.Y += gravity * gameTime;
	this->lifeTime -= gameTime;
}

void Particle::Draw(SDL_Surface* viewport){
	if(this->surf != NULL){
		this->surf->Draw(viewport, (int)this->position.X, (int)this->position.Y);
	}
}

bool Particle::IsAlive(){
	if(this->lifeTime > 0){
		return true;
	}else{
		return false;
	}
}