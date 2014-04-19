#include "Enemy.h"

//default contructor, loads 'k' enemy with 100 life and 100 IA handicap
Enemy::Enemy(){
	this->Load("neg_terry");
	this->Init(100, 0.2f, 200);
}

//constructor used to specify enemy name.
//IA handicap is 100 and life is 100 with this one
Enemy::Enemy(std::string sprite_name){
	this->Load(sprite_name);
	this->Init(100, 0.2f, 200);
}

//contructor used to specify enemy name, life, speed, and IA handicap
Enemy::Enemy(std::string sprite_name, int _life, float _velocity, int _handicap){
	this->Load(sprite_name);
	this->Init(_life, _velocity, _handicap);
}


Enemy::Enemy(Enemy& e, int _life, float _velocity, int _handicap){
	this->Load(e.GetAnimationList());
	this->Init(_life, _velocity, _handicap);
}

Enemy::Enemy(Enemy& e){
	this->Load(e.GetAnimationList());
	this->Init(e.GetLife(), e.GetVelocity(), e.GetHandicap());
	this->copyCreated = true;
}

void Enemy::Init(int _life, float _velocity, int _handicap){
	this->handicap = _handicap;
	this->life = _life;
	this->velocity = _velocity*RATIO;
	this->orientation = 1;
	this->jump_velocity = 0.5f*RATIO;
	this->jump_velocity_current = this->jump_velocity;
	this->jumping = false;
	this->attacking = false;
	this->onfloor = false;
    this->stunned = false;
    this->dying = false;
    this->blink = false;
    this->dieCurrentTimer = 0;
    this->dieTimer = 1500;
    this->stunTimer = 1000;
    this->stunCurrentTimer = 0;
    this->blinkTimer = 100;
    this->blinkCurrentTimer = 0;
	this->projected = 0;
	this->gravity = 0.001f*RATIO;
	this->animationState = ENEMY_STAND_RIGHT;
    this->lastProjectileId = 0;

	int side = rand() % 2;

	this->position = Vector2(side*RES_WIDTH + (-1 + side)*this->animationList[ENEMY_STAND_RIGHT]->GetWidth(), 200 + rand() % 200);
	this->direction = Vector2(0, 0);
	this->hit = false;
	this->handicapTimer = 0;
	this->alive = true;
	this->copyCreated = false;
}

Enemy::~Enemy(){
	for(int i = 0; i <= ENEMY_FLOOR_RIGHT; i++){
		delete this->animationList[i];
	}
	delete[] this->animationList;
}

//load enemy content
void Enemy::Load(std::string sprite_name){
	//load all of the enemy sprites into a Surface array
	this->animationList = new Surface*[ENEMY_FLOOR_RIGHT+1];
	this->animationList[ENEMY_RUN_LEFT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/run_left.png", 10, 75, Vector2(0,0), true);
	this->animationList[ENEMY_RUN_RIGHT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/run_right.png", 10, 75, Vector2(0,0), false);
	this->animationList[ENEMY_STAND_LEFT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/stand_left.png", 8, 75, Vector2(0,0), true);
	this->animationList[ENEMY_STAND_RIGHT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/stand_right.png", 8, 75, Vector2(0,0), false);
	this->animationList[ENEMY_PUNCH_LEFT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/punch_left.png", 8, 75, Vector2(0,0), true);
	this->animationList[ENEMY_PUNCH_RIGHT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/punch_right.png", 8, 75, Vector2(0,0), false);
	this->animationList[ENEMY_JUMP_LEFT_DOWN] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/jump_down_left.png", 1, 100, Vector2(0,0), true);
	this->animationList[ENEMY_JUMP_LEFT_UP] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/jump_up_left.png", 1, 100, Vector2(0,0), true);
	this->animationList[ENEMY_JUMP_RIGHT_UP] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/jump_up_right.png", 1, 100, Vector2(0,0), false);
	this->animationList[ENEMY_JUMP_RIGHT_DOWN] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/jump_down_right.png", 1, 100, Vector2(0,0), false);
	this->animationList[ENEMY_PUNCH_AIR_LEFT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/punch_air_left.png", 7, 75, Vector2(0,0), true);
	this->animationList[ENEMY_PUNCH_AIR_RIGHT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/punch_air_right.png", 7, 75, Vector2(0,0), false);
	this->animationList[ENEMY_HIT_LEFT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/hit_left.png", 5, 75, Vector2(0,0), true);
	this->animationList[ENEMY_HIT_RIGHT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/hit_right.png", 5, 75, Vector2(0,0), false);
	this->animationList[ENEMY_HARD_HIT_LEFT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/hard_hit_left.png", 5, 75, Vector2(0,0), true);
	this->animationList[ENEMY_HARD_HIT_RIGHT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/hard_hit_right.png", 5, 75, Vector2(0,0), false);	
	this->animationList[ENEMY_FALL_LEFT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/fall_left.png", 1, 100, Vector2(0,0), true);
	this->animationList[ENEMY_FALL_RIGHT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/fall_right.png", 1, 100, Vector2(0,0), false);
	this->animationList[ENEMY_FLOOR_LEFT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/floor_left.png", 1, 100, Vector2(0,0), true);
	this->animationList[ENEMY_FLOOR_RIGHT] = new Surface("Resources/Characters/Enemies/"+sprite_name + "/floor_right.png", 1, 100, Vector2(0,0), false);
}

void Enemy::Load(Surface** anim_list){
	this->animationList = new Surface*[ENEMY_FLOOR_RIGHT+1];
	this->animationList[ENEMY_RUN_LEFT] = new Surface(*anim_list[ENEMY_RUN_LEFT], 10, 75, true);
	this->animationList[ENEMY_RUN_RIGHT] = new Surface(*anim_list[ENEMY_RUN_RIGHT], 10, 75, false);
	this->animationList[ENEMY_STAND_LEFT] = new Surface(*anim_list[ENEMY_STAND_LEFT], 8, 75, true);
	this->animationList[ENEMY_STAND_RIGHT] = new Surface(*anim_list[ENEMY_STAND_RIGHT], 8, 75, false);
	this->animationList[ENEMY_PUNCH_LEFT] = new Surface(*anim_list[ENEMY_PUNCH_LEFT], 8, 75, true);
	this->animationList[ENEMY_PUNCH_RIGHT] = new Surface(*anim_list[ENEMY_PUNCH_RIGHT], 8, 75, false);
	this->animationList[ENEMY_JUMP_LEFT_DOWN] = new Surface(*anim_list[ENEMY_JUMP_LEFT_DOWN], 1, 100, true);
	this->animationList[ENEMY_JUMP_LEFT_UP] = new Surface(*anim_list[ENEMY_JUMP_LEFT_UP], 1, 100, true);
	this->animationList[ENEMY_JUMP_RIGHT_UP] = new Surface(*anim_list[ENEMY_JUMP_RIGHT_UP], 1, 100, false);
	this->animationList[ENEMY_JUMP_RIGHT_DOWN] = new Surface(*anim_list[ENEMY_JUMP_RIGHT_DOWN], 1, 100, false);
	this->animationList[ENEMY_PUNCH_AIR_LEFT] = new Surface(*anim_list[ENEMY_PUNCH_AIR_LEFT], 7, 75, true);
	this->animationList[ENEMY_PUNCH_AIR_RIGHT] = new Surface(*anim_list[ENEMY_PUNCH_AIR_RIGHT], 7, 75, false);
	this->animationList[ENEMY_HIT_LEFT] = new Surface(*anim_list[ENEMY_HIT_LEFT], 5, 85, true);
	this->animationList[ENEMY_HIT_RIGHT] = new Surface(*anim_list[ENEMY_HIT_RIGHT], 5, 85, false);
	this->animationList[ENEMY_HARD_HIT_LEFT] = new Surface(*anim_list[ENEMY_HARD_HIT_LEFT], 5, 75, true);
	this->animationList[ENEMY_HARD_HIT_RIGHT] = new Surface(*anim_list[ENEMY_HARD_HIT_RIGHT], 5, 75, false);
	this->animationList[ENEMY_FALL_LEFT] = new Surface(*anim_list[ENEMY_FALL_LEFT], 1, 100, true);
	this->animationList[ENEMY_FALL_RIGHT] = new Surface(*anim_list[ENEMY_FALL_RIGHT], 1, 100, false);
	this->animationList[ENEMY_FLOOR_LEFT] = new Surface(*anim_list[ENEMY_FLOOR_LEFT], 1, 100, true);
	this->animationList[ENEMY_FLOOR_RIGHT] = new Surface(*anim_list[ENEMY_FLOOR_RIGHT], 1, 100, false);

}

Surface** Enemy::GetAnimationList(){
	return this->animationList;
}

//change the current animation
//if the same animation is asked, does nothing
void Enemy::ChangeAnimation(ANIMATION_STATE state){
	if(this->animationState != state){
		this->animationList[this->animationState]->ResetSprite();
		this->animationState = state;
	}
}

//compute enemy distance from hero and return a boolean
//if hero is far away, return true
bool Enemy::IsHeroFarAway(Hero& hero){
	Vector2 pos = &this->position;
	if(this->jumping) pos = &this->jump_position;

	if(hero.GetPosition().X + HERO_WIDTH/2 < pos.X
		|| hero.GetPositionZ() < pos.Y - 20
		|| hero.GetPosition().X > pos.X + this->animationList[ENEMY_STAND_RIGHT]->GetWidth()/2
		|| hero.GetPositionZ() > pos.Y + 20){
			return true;
	}
	return false;
}

void Enemy::RefreshAnimation(Hero &hero){
	//update enemy animation
    if(this->stunned){
        return;
    }
	//if enemy is running to the right and not jumping
    if(this->direction.X > 0 && !this->jumping && !this->attacking && !this->hit && !this->projected){
		this->ChangeAnimation(ENEMY_RUN_RIGHT);
	//else if he is running to the left and not jumping
    }else if(this->direction.X < 0 && !this->jumping && !this->attacking && !this->hit && !this->projected){
		this->ChangeAnimation(ENEMY_RUN_LEFT);
	}else{
		//we set a modifier to change the direction of the enemy sprite (right or left in the enum)
		ANIMATION_STATE animationModifier = ENEMY_RUN_LEFT;
		if(hero.GetPosition().X > this->position.X){
			animationModifier = ENEMY_RUN_RIGHT;
		}
		
        if(this->projected) {
            if(this->position.Y >= this->jump_position.Y || this->direction.Y == 0.f){
                this->ChangeAnimation((ANIMATION_STATE)(ENEMY_FLOOR_LEFT + animationModifier));
            }else{
                this->ChangeAnimation((ANIMATION_STATE)(ENEMY_FALL_LEFT + animationModifier));
            }
        }
		//if he is jumping
        else if(this->jumping && !this->hit && !this->projected ){
			//if he is attacking
			if(this->attacking){
				this->ChangeAnimation((ANIMATION_STATE)(ENEMY_PUNCH_AIR_LEFT + animationModifier));
			}else{
				//if he is falling
				if(this->jump_velocity > 0){
					this->ChangeAnimation((ANIMATION_STATE)(ENEMY_JUMP_LEFT_UP + animationModifier));
				}else{
					this->ChangeAnimation((ANIMATION_STATE)(ENEMY_JUMP_LEFT_DOWN + animationModifier));
				}
			}
		}else if(!this->hit){
			if(this->attacking){
				this->ChangeAnimation((ANIMATION_STATE)(ENEMY_PUNCH_LEFT + animationModifier));
			}else{
				this->ChangeAnimation((ANIMATION_STATE)(ENEMY_STAND_LEFT + animationModifier));
			}
        }else{
            this->ChangeAnimation((ANIMATION_STATE)(ENEMY_HIT_LEFT + animationModifier));
		}
	}
}

//update the enemy game logic every frame 
void Enemy::Update(Uint32 gameTime, Hero &hero, std::vector<Projectile*>& heroProjectiles, std::vector<Projectile*>& enemiesProjectiles){
	if(!this->alive)
		return;

    if(this->stunned){
        this->stunCurrentTimer += gameTime;
        if(this->stunCurrentTimer >= this->stunTimer){
            this->stunned = false;
            this->stunCurrentTimer = 0;
        }
        return;
    }

    if(this->dying){
        this->dieCurrentTimer += gameTime;
        this->blinkCurrentTimer += gameTime;
        if(this->blinkCurrentTimer >= this->blinkTimer){
            this->blink = !this->blink;
            this->blinkCurrentTimer -= this->blinkTimer;
        }

        if(this->dieCurrentTimer >= this->dieTimer){
            this->dying = false;
            this->alive = false;
            this->stunned = false;
            this->dieCurrentTimer = 0;
        }
        return;
    }

    //if projected physics applies ! fun is here
    if(this->projected) {
        this->position = this->position + (this->direction * (this->velocity/2)) * gameTime;

        bool stuckX = false;
        if(this->direction.X > 0.f){
            this->direction.X = this->direction.X - (this->gravity * gameTime);
            if(this->direction.X <= 0.f){
                stuckX = true;
            }
        }else{
            this->direction.X = this->direction.X + (this->gravity * gameTime);
            if(this->direction.X >= 0.f){
                stuckX = true;
            }
        }

        if(this->GetPosition().X > RES_WIDTH - this->GetAnimationList()[this->animationState]->GetWidth()){
            this->position.X = RES_WIDTH - this->GetAnimationList()[this->animationState]->GetWidth();
            this->direction.X = -1.f;
            this->direction.Y = -1.f;
            this->jump_velocity_current = this->jump_velocity/2;
        }else if(this->position.X < 0){
            this->position.X = 0.f;
            this->direction.X = 1.f;
            this->direction.Y = -1.f;
            this->jump_velocity_current = this->jump_velocity/2;
        }

        if(this->position.Y < this->jump_position.Y){
            this->position.Y -= this->jump_velocity_current * gameTime;
            this->jump_velocity_current -= this->gravity * gameTime;
        }else{
            if(stuckX){
                this->direction.X = 0.f;
                this->direction.Y = 0.f;
                this->projected = false;
                this->stunned = true;
            }else{
                if(this->direction.X > 0.f){
                    this->direction.X = this->direction.X - (this->gravity * 4 * gameTime);
                }else{
                    this->direction.X = this->direction.X + (this->gravity * 4 * gameTime);
                }
            }
        }
	} else {
		this->handicapTimer += gameTime;
		if(this->handicapTimer >= this->handicap){
			this->onfloor = false;
			//now think robot, think !
			//if we are attacking or hit, we dont use our brain
			if(!this->attacking && !this->hit){
				if(this->IsHeroFarAway(hero)){
					//compute new direction for the robot
					if(!this->jumping){
						this->direction = (Vector2(hero.GetPosition().X + rand() % 100, (float)hero.GetPositionZ()) - this->position).Normalize();
					}else{
						this->direction = (Vector2(hero.GetPosition().X + rand() % 100, (float)hero.GetPositionZ()) - this->jump_position).Normalize();
					}
				}else{
					//else we stay on place
					this->direction = Vector2(0,0);
					//if hero is jumping and not falling and we are on floor
					if(hero.IsJumpingUp() && !this->jumping){
						//then we jump to reach him !
						this->jumping = true;
						this->jump_position = this->position;
					}else if(!(this->jumping && !hero.IsJumping())){
						//if we are in air and the hero is not, we dont attack
						//we attack him (but not if he is under us if in air)
						if(this->jumping){
							if(this->position.Y < hero.GetPosition().Y){
								this->attacking = true;
							}
						}else{
							this->attacking = true;
						}

						if(this->attacking){
							enemiesProjectiles.push_back(this->CreateProjectile());
						}
					}
				}
			}
			this->handicapTimer -= this->handicap;
		}
	}

	//if enemy is jumping
    if(this->jumping && !this->attacking){
		//move it by the current jump velocity
		this->position.Y -= this->jump_velocity_current * gameTime;
		//if enemy falled down to his original position
		if(this->jump_position.Y <= this->position.Y){
			//we know he is no more jumping
			this->jumping = false;
			this->jump_velocity_current = this->jump_velocity;	
			if(this->projected == 2) {
				this->onfloor = true;
			}
            this->projected = false;
			this->direction.Y = 0;
		} else {
			//we decrease the jump speed by the current gravity
            this->jump_velocity_current -= this->gravity * gameTime;
		}		
	}else if(this->jumping && this->attacking){
		this->jump_velocity_current = 0.0f;
	}

	//refresh the animation status of the enemy
	this->RefreshAnimation(hero);

	//if the enemy is not attacking we allow move on X and Y axis
    if(!this->attacking && !this->hit && !this->projected && !this->stunned && !this->dying){
		if(!this->jumping){
			this->position.X += this->velocity * gameTime * this->direction.X;
			this->position.Y += this->velocity * gameTime * this->direction.Y;
		}
		else{
			this->position.X += this->velocity * gameTime * this->direction.X/2;
			this->position.Y += this->velocity * gameTime * this->direction.Y/2;
			this->jump_position.X += this->velocity * gameTime * this->direction.X/2;
			this->jump_position.Y += this->velocity * gameTime * this->direction.Y/2;
		}
	}

	//update the current animation sprite 
	this->animationList[this->animationState]->Update(gameTime);

	//if attack animation ended, we are no more attacking
	if(this->attacking || this->hit){
		if(this->animationList[this->animationState]->HasLooped()){
			this->attacking = false;
			this->hit = false;
			this->handicapTimer = 0;
			this->animationList[this->animationState]->ResetLoop();
		}
	}	
}

Projectile* Enemy::CreateProjectile(){
	int direction = -1;
	if(this->animationState >= ENEMY_RUN_RIGHT){
		direction = 1;
	}
	return new Projectile(this->GetCollisionBox(), 0.25f, 3.0f, Vector2(direction, 0), 250);
}

void Enemy::Draw(SDL_Surface* viewport){
	//draw the animation sprite
    if(!this->blink){
        this->animationList[this->animationState]->DrawSprite(viewport, (int)this->position.X, (int)this->position.Y);
    }
	if(DEBUG_COLLIDER){
		this->GetCollisionBox().Draw(viewport, 255, 0, 0);
	}
}

//return the position of the current enemy (top left corner of the rectangle)
Vector2 Enemy::GetPosition(){
	return this->position;
}

Vector2 Enemy::GetFootPosition(){
	return Vector2(this->position.X+this->animationList[ENEMY_RUN_RIGHT]->GetWidth()/2, this->position.Y + this->animationList[ENEMY_RUN_RIGHT]->GetHeight());
}

Rectangle Enemy::GetCollisionBox(){
	return Rectangle(this->position.X + 75*RATIO, this->position.Y + 50*RATIO, 30*RATIO, 30*RATIO);
}

bool Enemy::Hit(Projectile* proj){
	if(this->lastProjectileId != proj->GetId()){
		this->life -= (int)proj->GetPower();
		this->attacking = false;
		this->hit = true;
		this->animationList[this->animationState]->ResetSprite();
		this->jump_velocity_current = this->jump_velocity/5;
		this->lastProjectileId = proj->GetId();
		if(this->life <= 0){
            this->dying = true;
		} else {
            if(proj->GetPower() >= 5) {
                if(proj->GetDirection().Y <= 0.f){
                    if(!this->projected && !this->jumping){
                        this->jump_position = this->GetPosition();
                    }
                    this->jump_velocity_current = this->jump_velocity;
                    this->direction =  proj->GetDirection() * (proj->GetPower()/2.f);
                    this->direction.Y = -1.f;
                }
                this->projected = true;
            }
		}
		return true;
	}
	return false;
}

bool Enemy::IsAlive(){
    return this->alive;
}

bool Enemy::IsDying(){
    return this->dying;
}

int Enemy::GetLife(){
	return this->life;
}

float Enemy::GetVelocity(){
	return this->velocity;
}

int Enemy::GetHandicap(){
	return this->handicap;
}

// Return Z position of enemy
float Enemy::GetPositionZ(){
	if(!this->jumping){
		return this->position.Y;
	}else{
        return this->jump_position.Y;
	}
}
