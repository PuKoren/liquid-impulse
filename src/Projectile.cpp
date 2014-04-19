#include "Projectile.h"

int Projectile::projId = 0;
int Projectile::GetId(){
	return Projectile::projId;
}

Projectile::Projectile(){
	this->velocity = 0.0f;
	this->direction = Vector2(0, 0);
	this->lifeTime = 0;
	this->power = 0;
	this->elapsedLifeTime = 0;
	this->collisions = 0;
	this->alive = false;
	this->waitTime = 0;
	Projectile::projId++;
}

Projectile::Projectile(Rectangle _collisionBox, float _velocity, float _power, Vector2 _direction, int _lifeTime){
	this->collisionBox = _collisionBox;
	this->velocity = _velocity;
	this->power = _power;
	this->direction = _direction;
	this->lifeTime = _lifeTime;
	this->collisions = 0;
	this->elapsedLifeTime = 0;
	this->alive = true;
	this->waitTime = 0;
	Projectile::projId++;
}

Projectile::Projectile(Rectangle _collisionBox, float _velocity, float _power, Vector2 _direction, int _lifeTime, int _waitTime){
	this->collisionBox = _collisionBox;
	this->velocity = _velocity;
	this->power = _power;
	this->direction = _direction;
	this->lifeTime = _lifeTime;
	this->collisions = 0;
	this->elapsedLifeTime = 0;
	this->alive = true;
	this->waitTime = _waitTime;
	Projectile::projId++;
}

Projectile::~Projectile(){

}

void Projectile::Update(Uint32 gameTime){
	if(this->elapsedLifeTime >= this->waitTime){
		this->collisionBox.Position = this->collisionBox.Position + this->direction * (float)gameTime * this->velocity;
	}
	this->elapsedLifeTime += gameTime;

	if(this->elapsedLifeTime > this->lifeTime){
		this->alive = false;
	}
}

void Projectile::Draw(SDL_Surface* viewport){
	if(DEBUG_COLLIDER && this->elapsedLifeTime >= this->waitTime){
		this->GetCollisionBox().Draw(viewport, 255, 255, 0);
	}
}

void Projectile::AddCollision(){
	this->collisions++;
}

int Projectile::GetCollision(){
	return this->collisions;
}

Vector2 Projectile::GetDirection(){
	return this->direction;
}

Rectangle Projectile::GetCollisionBox(){
	if(this->elapsedLifeTime >= this->waitTime){
		return this->collisionBox;
	}else{
		return Rectangle();
	}
}

bool Projectile::IsAlive(){
	return this->alive;
}

void Projectile::SetAlive(bool _alive){
	this->alive = _alive;
}

float Projectile::GetPower(){
	return this->power;
}