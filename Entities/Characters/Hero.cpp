#include "Hero.h"

// Default constructor : hero with 100 health and 2 lifes
Hero::Hero(std::vector<Projectile*>* _heroProjectiles){
    this->Load("terry");
    float speed = 0.55f * RATIO;
    this->Init(100, 2, speed, Vector2(0.0f, (float)RES_HEIGHT/3), Vector2((float)(RES_WIDTH - HERO_WIDTH), (float)(RES_HEIGHT - RES_HEIGHT/3)));
    this->heroProjectiles = _heroProjectiles;
}

// Constructor used to specify hero name, health, number of lifes and speed
Hero::Hero(std::string sprite_name, int _health, int _life, float _velocity, Vector2 _limitTopLeftPoint, Vector2 _limitBottomRightPoint, std::vector<Projectile*>* _heroProjectiles){
    this->Load(sprite_name);
    this->Init(_health, _life, _velocity, _limitTopLeftPoint, _limitBottomRightPoint);
    this->heroProjectiles = _heroProjectiles;
}

// Destructor
Hero::~Hero(){
	for(int i = 0; i <= HERO_BOMB_AIR_RIGHT; i++){
		delete this->heroAnimationList[i];
	}
	delete[] this->heroAnimationList;

    Mix_FreeChunk(this->hitSound);
	Mix_FreeChunk(this->repulseSound);
	Mix_FreeChunk(this->impulseSound);
	Mix_FreeChunk(this->jumpSound);

    delete this->impulseEffectRight;
    delete this->impulseEffectLeft;
    delete this->repulseEffect;
	delete this->airBombEffect;
	delete this->jumpDustEffect;
	delete this->heroShadow;
}

// Hero initialisation
void Hero::Init(int _health, int _life, float _velocity, Vector2 _limitTopLeftPoint, Vector2 _limitBottomRightPoint){
    this->health = _health;
    this->life = _life;
    this->limitTopLeftPoint = _limitTopLeftPoint;
    this->limitBottomRightPoint = _limitBottomRightPoint;

    this->velocity = _velocity;
    this->jump_velocity = 0.5f * RATIO;
    this->jump_velocity_current = this->jump_velocity;
    this->gravity = 0.001f * RATIO;

    this->heroAnimationState = HERO_STAND_RIGHT;
    this->position = Vector2(100*RATIO, (float)RES_HEIGHT/2);
    this->direction = Vector2(0, 0);
    this->directionSpecial = Vector2(0, 0);

    this->jumping = false;
    this->attacking = false;
    this->specialAttack = false;
    this->hit = false;
    this->alive = true;	
	this->blockingAnimationModifier = false;
    this->lastMoveDirection = 0;

    // Sounds initialisation
    this->hitSound = Mix_LoadWAV("Resources/Sounds/Effects/Hit.wav");
	this->impulseSound = Mix_LoadWAV("Resources/Sounds/Effects/Impulse.wav");
	this->jumpSound = Mix_LoadWAV("Resources/Sounds/Effects/Jump.wav");
	this->repulseSound = Mix_LoadWAV("Resources/Sounds/Effects/Repulse.wav");

    // Effect initialisation
    this->impulseEffectRight = new Effect("Attacks", "impulse_attack_effect_right", 14, 20, Vector2(-60*RATIO,-80*RATIO), 1.5f*RATIO, 420, false);
    this->impulseEffectLeft = new Effect("Attacks", "impulse_attack_effect_left", 14, 20, Vector2(0.f,-80*RATIO), 1.5f*RATIO, 420, true);
    this->repulseEffect = new Effect("Attacks", "repulse_attack_effect", 12, 40, Vector2(-50*RATIO,-20*RATIO), 0.0f, 320, false);
    this->airBombEffect = new Effect("Attacks", "air_bomb_attack_effect", 17, 40, Vector2(-50*RATIO,-80*RATIO), 0.0f, 330, false);
    this->jumpDustEffect = new Effect("Environment", "jump_dust", 17, 30, Vector2(18*RATIO,0.f), 0.0f, 0, false);
    this->heroShadow = new Surface("Resources/Effects/Environment/hero_shadow.png");
}

// Load hero content
void Hero::Load(std::string sprite_name){    
	// Load all of the hero sprites into a Surface array
	this->heroAnimationList = new Surface*[HERO_BOMB_AIR_RIGHT+1];
	this->heroAnimationList[HERO_RUN_LEFT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_run_left.png", 10, 50, Vector2(0,0), true);
	this->heroAnimationList[HERO_RUN_RIGHT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_run_right.png", 10, 50, Vector2(0,0), false);
	this->heroAnimationList[HERO_STAND_LEFT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_stand_left.png", 8, 75, Vector2(0,0), true);
	this->heroAnimationList[HERO_STAND_RIGHT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_stand_right.png", 8, 75, Vector2(0,0), false);
	
	this->heroAnimationList[HERO_PUNCH_LEFT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_punch_left.png", 8, 50, Vector2(0,0), true);
	this->heroAnimationList[HERO_PUNCH_RIGHT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_punch_right.png", 8, 50, Vector2(0,0), false);
	this->heroAnimationList[HERO_JUMP_LEFT_DOWN] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_jump_down_left.png", 1, 100, Vector2(0,0), true);
	this->heroAnimationList[HERO_JUMP_LEFT_UP] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_jump_up_left.png", 1, 100, Vector2(0,0), true);
	this->heroAnimationList[HERO_JUMP_RIGHT_UP] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_jump_up_right.png", 1, 100, Vector2(0,0), false);
	this->heroAnimationList[HERO_JUMP_RIGHT_DOWN] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_jump_down_right.png", 1, 100, Vector2(0,0), false);
	this->heroAnimationList[HERO_PUNCH_AIR_LEFT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_punch_air_left.png", 7, 50, Vector2(0,0), true);
	this->heroAnimationList[HERO_PUNCH_AIR_RIGHT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_punch_air_right.png", 7, 50, Vector2(0,0), false);
	this->heroAnimationList[HERO_HIT_LEFT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_hit_left.png", 5, 75, Vector2(0,0), true);
	this->heroAnimationList[HERO_HIT_RIGHT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_hit_right.png", 5, 75, Vector2(0,0), false);
	this->heroAnimationList[HERO_HARD_HIT_LEFT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_hard_hit_left.png", 5, 75, Vector2(0,0), true);
	this->heroAnimationList[HERO_HARD_HIT_RIGHT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_hard_hit_right.png", 5, 75, Vector2(0,0), false);
	this->heroAnimationList[HERO_IMPULSE_LEFT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_impulse_left.png", 24, 20, Vector2(0.f,-10*RATIO), true);
	this->heroAnimationList[HERO_IMPULSE_RIGHT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_impulse_right.png", 24, 20, Vector2(-60*RATIO,-10*RATIO), false);
	this->heroAnimationList[HERO_REPULSE_LEFT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_repulse_left.png", 7, 130, Vector2(0,0), true);
	this->heroAnimationList[HERO_REPULSE_RIGHT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_repulse_right.png", 7, 130, Vector2(0,0), false);
	this->heroAnimationList[HERO_UPPERCUT_LEFT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_uppercut_left.png", 8, 50, Vector2(0,0), true);
	this->heroAnimationList[HERO_UPPERCUT_RIGHT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_uppercut_right.png", 8, 50, Vector2(0,0), false);
	this->heroAnimationList[HERO_TAKEDOWN_AIR_LEFT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_takedown_air_left.png", 24, 25, Vector2(15*RATIO,-35*RATIO), true);
	this->heroAnimationList[HERO_TAKEDOWN_AIR_RIGHT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_takedown_air_right.png", 24, 25, Vector2(-45*RATIO,-35*RATIO), false);
	this->heroAnimationList[HERO_BOMB_AIR_LEFT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_bomb_air_left.png", 15, 30, Vector2(-50*RATIO,-50*RATIO), true);
	this->heroAnimationList[HERO_BOMB_AIR_RIGHT] = new Surface("Resources/Characters/Hero/"+sprite_name + "/hero_bomb_air_right.png", 15, 30, Vector2(0.f,-50*RATIO), false);
}

Surface ** Hero::GetAnimationList(){
	return this->heroAnimationList;
}

// Change the current animation
// If the same animation is asked, does nothing
void Hero::ChangeAnimation(HERO_ANIMATION_STATE state){
    if(this->heroAnimationState != state){
		this->heroAnimationList[this->heroAnimationState]->ResetSprite();
		this->heroAnimationState = state;
	}
}

// Update hero animation
void Hero::RefreshAnimation(){
	// If hero is running to the right and not jumping
	if(this->direction.X > 0 && !this->jumping && !this->attacking && !this->hit){
		this->ChangeAnimation(HERO_RUN_RIGHT);
	// Else if he is running to the left and not jumping
	}else if(this->direction.X < 0 && !this->jumping && !this->attacking && !this->hit){
		this->ChangeAnimation(HERO_RUN_LEFT);
	}else{
		// We set a modifier to change the direction of the hero sprite (right or left in the enum)
		if(!this->blockingAnimationModifier)
			this->animationModifier = (this->lastMoveDirection == -1)? HERO_RUN_LEFT : HERO_RUN_RIGHT;

		// If he is jumping
		if(this->jumping && !this->hit){
			// If he is attacking
            if(this->specialAttack){
                this->ChangeAnimation(this->heroAnimationState);
            }else if(this->attacking){
				this->ChangeAnimation((HERO_ANIMATION_STATE)(HERO_PUNCH_AIR_LEFT + animationModifier));
			}else{
				// If he is falling
				if(this->jump_velocity > 0){
					this->ChangeAnimation((HERO_ANIMATION_STATE)(HERO_JUMP_LEFT_UP + animationModifier));
				}else{
					this->ChangeAnimation((HERO_ANIMATION_STATE)(HERO_JUMP_LEFT_DOWN + animationModifier));
				}
			}
		}else if(!this->hit){
            if(this->specialAttack){
                this->ChangeAnimation(this->heroAnimationState);
            }else if(this->attacking){
				this->ChangeAnimation((HERO_ANIMATION_STATE)(HERO_PUNCH_LEFT + animationModifier));
			}else{
                if(this->direction.Y != 0 && !this->jumping){
                    this->ChangeAnimation((HERO_ANIMATION_STATE)(HERO_RUN_LEFT + animationModifier));
                }else {
				    this->ChangeAnimation((HERO_ANIMATION_STATE)(HERO_STAND_LEFT + animationModifier));
                }
			}
		}else{
            this->ChangeAnimation(this->heroAnimationState);
		}
	}
}

/*********************/
/** Update Function **/
/*********************/

// Update the hero game logic every frame 
void Hero::Update(Uint32 gameTime, std::vector<Projectile*>& heroProjectiles){
	if(!this->alive)
		return;

	// If hero is jumping
	if(this->jumping && !this->attacking){
		// Move it by the current jump velocity
		this->position.Y -= this->jump_velocity_current * gameTime;
		// If hero falled down to his original position
		if(this->jump_position.Y < this->position.Y){
			// We know he is no more jumping
			this->jumping = false;
			this->jump_velocity_current = this->jump_velocity;
			Mix_PlayChannel(-1, this->jumpSound, false);
		}
		// We decrease the jump speed by the current gravity
		this->jump_velocity_current -= this->gravity * gameTime;
    }else if(this->jumping && this->specialAttack && this->directionSpecial.Y > 0 && this->position.Y < this->jump_position.Y){
        this->position.Y += this->directionSpecial.Y;
		this->jump_velocity_current = 0.0f;
	}else if(this->jumping && this->attacking){
		this->jump_velocity_current = 0.0f;
	}

	// if the hero is not attacking we allow move on X and Y axis
    if(!this->attacking && !this->hit && !this->specialAttack){
		if(!this->jumping){
			this->position.X += this->velocity * gameTime * this->direction.X;
            if(this->ControlHeroLimitsY())
			    this->position.Y += this->velocity * gameTime * this->direction.Y;
		}else{
			this->position.X += this->velocity * gameTime * this->direction.X/2;
			this->jump_position.X += this->velocity * gameTime * this->direction.X/2;
            if(this->ControlHeroLimitsY()){
			    this->position.Y += this->velocity * gameTime * this->direction.Y/2;
			    this->jump_position.Y += this->velocity * gameTime * this->direction.Y/2;
            }
		}
        this->ControlHeroLimitsX();
	}

	// Update the current animation sprite
	this->heroAnimationList[this->heroAnimationState]->Update(gameTime);

	// If attack animation ended, we are no more attacking
    if(this->attacking || this->hit || this->specialAttack){
		if(this->heroAnimationList[this->heroAnimationState]->HasLooped()){
			this->attacking = false;            
			this->specialAttack = false;
			this->blockingAnimationModifier = false;
			this->hit = false;
			this->heroAnimationList[this->heroAnimationState]->ResetLoop();
            this->directionSpecial = Vector2(0, 0);
		}
	}

    // Refresh the animation status of the hero
	this->RefreshAnimation();
    this->UpdateEffect(gameTime);
}

void Hero::UpdateEffect(Uint32 gameTime){    
    this->impulseEffectRight->Update(gameTime);
    this->impulseEffectLeft->Update(gameTime);
    this->airBombEffect->Update(gameTime);
    this->repulseEffect->Update(gameTime);
	this->jumpDustEffect->Update(gameTime);
}

void Hero::DrawEffect(SDL_Surface * viewport){
    this->impulseEffectRight->Draw(viewport);
    this->impulseEffectLeft->Draw(viewport);
    this->airBombEffect->Draw(viewport);
    this->repulseEffect->Draw(viewport);
    this->jumpDustEffect->Draw(viewport);
}

void Hero::Draw(SDL_Surface * viewport){
	// Draw the animation sprite
	this->heroAnimationList[this->heroAnimationState]->DrawSprite(viewport, (int)this->position.X, (int)this->position.Y);
    
    this->DrawEffect(viewport);

	if(this->jumping){
		this->heroShadow->Draw(viewport, (int)(this->position.X + 75), (int)(this->jump_position.Y + HERO_WIDTH));
	}

    // Draw box collider
	if(DEBUG_COLLIDER){
		this->GetCollisionBox().Draw(viewport, 255, 0, 0);
	}
}

// Events listener
void Hero::Event(SDL_Event * e){
	if(e->type == SDL_KEYDOWN){
		switch(e->key.keysym.sym){
			case SDLK_RIGHT: {
                this->HandleMove(1); 
                this->lastMoveDirection = 1;
                break;
            }
			case SDLK_LEFT:{
                this->HandleMove(-1);                     
                this->lastMoveDirection = -1;
                break;
            }
			case SDLK_q:{
                this->HandleAttack();
                break;
            }
			case SDLK_d:{
                if(this->jumping) this->HandleAirBombAttack(); 
                else this->HandleImpulseAttack();
                break;
            }
            case SDLK_z:{
                if(this->jumping) this->HandleAirTakedownAttack();
                else this->HandleUppercutAttack();
                break;
            }
			case SDLK_s: this->HandleJump(); break;
            case SDLK_UP: this->HandleAxis(-1);break;
			case SDLK_DOWN: this->HandleAxis(1);break;
            case SDLK_e:
                if(!this->jumping) this->HandleRepulseAttack();
                        else this->HandleAirBombAttack();
                break;
            case SDLK_0: this->direction.X = 0;this->direction.Y = 0;break;
			default:
				break;
		}
	}
	else if(e->type == SDL_KEYUP){
		switch(e->key.keysym.sym){
			case SDLK_RIGHT: this->HandleMove(-1); break;
			case SDLK_LEFT: this->HandleMove(1); break;
			case SDLK_UP: this->HandleAxis(1);break;
			case SDLK_DOWN: this->HandleAxis(-1);break;
			default:
				break;
		}
	}
}

/*******************/
/** Handle events **/
/*******************/

// Handle for moving X action
void Hero::HandleMove(int direction){
    if(this->direction.X == 0 || (this->direction.X < 0 && direction > 0) || (this->direction.X > 0 && direction < 0)){
        this->direction.X += this->velocity * direction;
    }
}

// Handle for moving Y action
void Hero::HandleAxis(int direction){
    if(this->direction.Y == 0 || (this->direction.Y < 0 && direction > 0) || (this->direction.Y > 0 && direction < 0)){
        this->direction.Y += this->velocity * direction;
    }
}

// Handle event for jump action
void Hero::HandleJump(){
    if(!this->jumping && (!this->attacking || (this->attacking && this->heroAnimationList[this->heroAnimationState]->HasHalfLooped()) || (this->specialAttack && this->heroAnimationList[this->heroAnimationState]->HasLooped()))){
		this->attacking = false;
        this->jump_position = this->position;
        this->jumping = true;
		this->blockingAnimationModifier = false;
		this->jumpDustEffect->StartEffect(this->position, Vector2(0,0));
		Mix_PlayChannel(-1, this->jumpSound, false);
	}
}

// Handle event for punch attack action
void Hero::HandleAttack(){
    if(!this->hit && !this->specialAttack){        
        // If hero is already attacking and if the current animation is half looped, hero can attack again => so, we reset current attack animation
        if((this->attacking && this->heroAnimationList[this->heroAnimationState]->HasHalfLooped()) || !this->attacking){
	        this->animationModifier = (this->lastMoveDirection == -1)? HERO_RUN_LEFT : HERO_RUN_RIGHT;
			this->heroAnimationList[this->heroAnimationState]->ResetSprite();
			this->heroAnimationList[this->heroAnimationState]->ResetLoop();
			this->attacking = true;
			this->blockingAnimationModifier = true;
			this->heroProjectiles->push_back(this->CreateProjectile());
			Mix_PlayChannel(-1, this->hitSound, false);
		}
    }
}

// Handle event for impulse attack action
void Hero::HandleImpulseAttack(){
    if(this->HandleSpecialAttack(HERO_IMPULSE_LEFT)){
        if(this->lastMoveDirection == -1){
            this->impulseEffectLeft->StartEffect(this->position, Vector2(-1,0));			
        }else{
            this->impulseEffectRight->StartEffect(this->position, Vector2(1,0));
        }
        this->heroProjectiles->push_back(this->CreateProjectile(Vector2(1, 0), Vector2(0,0), 10, 0.6f, 1.0f, 4.0f, 300));
		Mix_PlayChannel(-1, this->impulseSound, false);
    }
}

// Handle event for air bomb attack action
void Hero::HandleAirBombAttack(){
    if(this->HandleSpecialAttack(HERO_BOMB_AIR_LEFT)){
        this->directionSpecial.Y = this->velocity * 10;
        this->airBombEffect->StartEffect(this->jump_position, Vector2(0,0));
		this->heroProjectiles->push_back(this->CreateProjectile(Vector2(0, 1), Vector2(-90, 0), 10, 0.8f, 6.0f, 1.0f, 100));
		Mix_PlayChannel(-1, this->repulseSound, false);
    }
}

// Handle event for uppercut attack action
void Hero::HandleUppercutAttack(){
	if(this->HandleSpecialAttack(HERO_UPPERCUT_LEFT)){
		this->HandleSpecialAttack(HERO_UPPERCUT_LEFT);
		this->heroProjectiles->push_back(this->CreateProjectile(Vector2(0, -1), Vector2(20, 0), 5, 0.25f, 1.0f, 1.0f, 100));
		Mix_PlayChannel(-1, this->hitSound, false);
	}
}

// Handle event for air takedown attack action
void Hero::HandleAirTakedownAttack(){
	if(this->HandleSpecialAttack(HERO_TAKEDOWN_AIR_LEFT)){
		this->HandleSpecialAttack(HERO_TAKEDOWN_AIR_LEFT);
		this->heroProjectiles->push_back(this->CreateProjectile(Vector2(0, 1), Vector2(20, 0), 10, 0.20f, 2.0f, 2.0f, 300));
		Mix_PlayChannel(-1, this->impulseSound, false);
	}
}

// Handle event for repulse attack action
void Hero::HandleRepulseAttack(){
	if(this->HandleSpecialAttack(HERO_REPULSE_LEFT)){
		if(this->lastMoveDirection == -1){
			this->repulseEffect->StartEffect(Vector2(this->position.X + 20,this->position.Y), Vector2(0,0));
        }else{
            this->repulseEffect->StartEffect(this->position, Vector2(0,0));
        }
		Mix_PlayChannel(-1, this->repulseSound, false);
        this->heroProjectiles->push_back(this->CreateProjectile(Vector2(1, 0), Vector2(0,0), 10, 0.5f, 1.0f, 4.0f, 400));
        this->heroProjectiles->push_back(this->CreateProjectile(Vector2(-1, 0), Vector2(0,0), 10, 0.5f, 1.0f, 4.0f, 400));
    }
}

// Handle event for special attacks
bool Hero::HandleSpecialAttack(HERO_ANIMATION_STATE state){
    if(!this->hit && !this->specialAttack){
        this->heroAnimationList[this->heroAnimationState]->ResetSprite();
		this->heroAnimationList[this->heroAnimationState]->ResetLoop();
        this->specialAttack = true;
        this->attacking = true;
	    this->blockingAnimationModifier = true;
        this->animationModifier = (this->lastMoveDirection == -1)? HERO_RUN_LEFT : HERO_RUN_RIGHT;
        this->heroAnimationState = (HERO_ANIMATION_STATE)(state + this->animationModifier);
        return true;
    }
    return false;
}

Projectile * Hero::CreateProjectile(){
	int direction = -1;
	if(this->heroAnimationState >= HERO_RUN_RIGHT){
		direction = 1;
	}
	return new Projectile(this->GetCollisionBox(), 0.25f*RATIO, 3.0f*RATIO, Vector2(direction, 0), 300);
}

Projectile * Hero::CreateProjectile(Vector2 _direction, Vector2 _decal, int power, float speed, float width, float height, int waitTime){
	int direction = -1;
	if(this->heroAnimationState >= HERO_RUN_RIGHT){
		direction = 1;
	}
	Rectangle col = this->GetCollisionBox();
	col.Width *= (int)width;
	col.Height *= (int)height;
	if(direction < 0 && _decal.X < 0){
		col.Position.X += _decal.X;
	}else{
		col.Position.X += _decal.X * direction;
	}
	col.Position.Y += _decal.Y;
	return new Projectile(col, speed*RATIO, power, Vector2(_direction.X * direction, _direction.Y), 300 + waitTime, waitTime);
}

// Control if hero is out of limits X
void Hero::ControlHeroLimitsX(){
    if(this->direction.X < 0 && this->position.X < this->limitTopLeftPoint.X) 
        this->position.X = this->limitTopLeftPoint.X;
    else if(this->direction.X > 0 && this->position.X > this->limitBottomRightPoint.X) 
        this->position.X = this->limitBottomRightPoint.X;
}

// Return false if hero is out of limits Y (false = can't move)
bool Hero::ControlHeroLimitsY(){
    if(!this->jumping){
        if(this->direction.Y < 0 && this->position.Y < this->limitTopLeftPoint.Y) 
            return false;
        else if(this->direction.Y > 0 && this->position.Y > this->limitBottomRightPoint.Y) 
            return false;
    }else{
        if(this->direction.Y < 0 && this->jump_position.Y < this->limitTopLeftPoint.Y) 
            return false;
        else if(this->direction.Y > 0 && this->jump_position.Y > this->limitBottomRightPoint.Y) 
            return false;
    }
	return true;
}

// Return the position of hero (top left corner of the rectangle)
Vector2 Hero::GetPosition(){
	return this->position;
}

// Return the position of hero's feet
Vector2 Hero::GetFootPosition(){
	return Vector2(this->position.X+this->heroAnimationList[HERO_RUN_RIGHT]->GetWidth()/2, this->position.Y + this->heroAnimationList[HERO_RUN_RIGHT]->GetHeight());
}

// Return the collision box
Rectangle Hero::GetCollisionBox(){
	return Rectangle(this->position.X + 75*RATIO, this->position.Y + 50*RATIO, 30*RATIO, 30*RATIO);
}

// If hero has been hit by an attack
void Hero::Hit(Projectile* proj){
	this->heroAnimationList[this->heroAnimationState]->ResetSprite();

	if(proj->GetDirection().X > 0){
        this->animationModifier = HERO_RUN_LEFT;
	}else if(proj->GetDirection().X < 0){
        this->animationModifier = HERO_RUN_RIGHT;
	}

    if((int)proj->GetPower() < 10){
        this->heroAnimationState = (HERO_ANIMATION_STATE)(HERO_HIT_LEFT + this->animationModifier);
	}else{
        this->heroAnimationState = (HERO_ANIMATION_STATE)(HERO_HARD_HIT_LEFT + this->animationModifier);
	}

	this->health -= (int)proj->GetPower();
	if(this->health <= 0){
		this->life--;
		this->health = 100;
	}
    this->hit = true;
	Mix_PlayChannel(-1, this->hitSound, false);
}

// Return true if hero is alive
bool Hero::IsAlive(){
	return this->alive;
}

// Return true if hero is jumping
bool Hero::IsJumping(){
	return this->jumping;
}

// Return true if the hero is jumping up
bool Hero::IsJumpingUp(){
	return this->jumping && this->jump_velocity_current <= 0;
}

// Return true if hero is attacking
bool Hero::IsAttacking(){
	return this->attacking;
}

// Return remaining health point of hero
int Hero::GetHealth(){
	return this->health;
}

// Return number of life remaining
int Hero::GetLife(){
	return this->life;
}

// Return Z position of hero
float Hero::GetPositionZ(){
	if(!this->jumping){
		return this->position.Y;
	}else{
        return this->jump_position.Y;
	}
}

// Return hero's velocity
float Hero::GetVelocity(){
	return this->velocity;
}

// Return hero's moving direction
int Hero::GetMovingDirection(){
    if(this->direction.X > 0 && !this->attacking){
		return 1;	
	}else if(this->direction.X < 0 && !this->attacking){ 
		return -1;
	}else{
		return 0;
	}
}

// Set the X position of hero
void Hero::SetPositionX(float newX){
	this->position.X = newX;
}

//#include "Hero.h"
//
//Effect * impulseAttack;
//Effect * repulseAttack;
//Effect * dust;
//Effect * airBombAttack;
//
//Hero::Hero()
//{
//	this->posX = 10;
//	this->posY = (int)(RES_HEIGHT - HERO_HEIGHT * 1.5);
//
//	this->frame = 0;
//	this->status = STAND;
//	this->directionStatus = RIGHT;
//	this->animationStatus = 0;
//	this->attackStatus = FINISHED;
//	this->animationTimer = 80;
//
//	this->maxZ = RES_HEIGHT - HERO_HEIGHT - 60;
//	this->minZ = (int)(RES_HEIGHT - HERO_HEIGHT * 2.6);
//	this->velocityX = 0;
//	this->velocityY = 0;
//	this->velocityZ = 0;
//	this->jumpStartAtPosY = 0;
//	this->canJump = true;
//	this->jumping = false;
//	this->life = 2;
//	this->health = 100;
//
//	this->hitSound = Mix_LoadWAV("Resources/Sounds/Effects/Hit.wav");
//	this->impulseSound = Mix_LoadWAV("Resources/Sounds/Effects/Impulse.wav");
//	this->jumpSound = Mix_LoadWAV("Resources/Sounds/Effects/Jump.wav");
//	this->repulseSound = Mix_LoadWAV("Resources/Sounds/Effects/Repulse.wav");
//
//	impulseAttack = new Effect(1);
//	repulseAttack = new Effect(2);
//	dust = new Effect(3);
//	airBombAttack = new Effect(4);
//
//	this->surfStand = new Surface("Resources/Characters/Hero/hero_stand.png");
//	this->surfStandLeft = new Surface("Resources/Characters/Hero/hero_stand_left.png");
//	this->surfMove = new Surface("Resources/Characters/Hero/hero_move.png");
//	this->surfMoveLeft = new Surface("Resources/Characters/Hero/hero_move_left.png");
//	this->surfJumpUp = new Surface("Resources/Characters/Hero/hero_jump_up.png");
//	this->surfJumpDown = new Surface("Resources/Characters/Hero/hero_jump_down.png");
//	this->surfJumpUpLeft = new Surface("Resources/Characters/Hero/hero_jump_up_left.png");
//	this->surfJumpDownLeft = new Surface("Resources/Characters/Hero/hero_jump_down_left.png");
//
//	this->surfAttack = new Surface("Resources/Characters/Hero/hero_attack.png");	
//	this->surfAttackLeft = new Surface("Resources/Characters/Hero/hero_attack_left.png");
//	this->surfImpulseAttack = new Surface("Resources/Characters/Hero/hero_impulse_attack.png");	
//	this->surfImpulseAttackLeft = new Surface("Resources/Characters/Hero/hero_impulse_attack_left.png");
//	this->surfRepulseAttack = new Surface("Resources/Characters/Hero/hero_repulse_attack.png");	
//	this->surfRepulseAttackLeft = new Surface("Resources/Characters/Hero/hero_repulse_attack_left.png");	
//	this->surfUppercutAttack = new Surface("Resources/Characters/Hero/hero_uppercut_attack.png");
//	this->surfUppercutAttackLeft = new Surface("Resources/Characters/Hero/hero_uppercut_attack_left.png");
//	this->surfAirAttack = new Surface("Resources/Characters/Hero/hero_air_attack.png");	
//	this->surfAirAttackLeft = new Surface("Resources/Characters/Hero/hero_air_attack_left.png");
//	this->surfAirTakeDownAttack = new Surface("Resources/Characters/Hero/hero_air_take_down_attack.png");
//	this->surfAirTakeDownAttackLeft = new Surface("Resources/Characters/Hero/hero_air_take_down_attack_left.png");
//	this->surfAirBombAttack = new Surface("Resources/Characters/Hero/hero_air_bomb_attack.png");
//	this->surfAirBombAttackLeft = new Surface("Resources/Characters/Hero/hero_air_bomb_attack_left.png");
//	this->surfGetHurted = new Surface("Resources/Characters/Hero/hero_get_hurted.png");
//	this->surfGetHurtedLeft = new Surface("Resources/Characters/Hero/hero_get_hurted_left.png");
//	this->surfGetHurtedHard = new Surface("Resources/Characters/Hero/hero_get_hurted_hard.png");
//	this->surfGetHurtedHardLeft = new Surface("Resources/Characters/Hero/hero_get_hurted_hard_left.png");
//
//	this->surfShadow = new Surface("Resources/Effects/Environment/shadow_pixel.png");
//
//	this->SetMoveClips();
//}
//
//Hero::~Hero()
//{
//	delete this->surfStand;
//	delete this->surfStandLeft;
//	delete this->surfMove;
//	delete this->surfMoveLeft;
//	delete this->surfJumpUp;
//	delete this->surfJumpDown;
//	delete this->surfJumpUpLeft;
//	delete this->surfJumpDownLeft;
//
//	delete this->surfAttack;
//	delete this->surfAttackLeft;
//	delete this->surfImpulseAttack;	
//	delete this->surfImpulseAttackLeft;
//	delete this->surfRepulseAttack;
//	delete this->surfRepulseAttackLeft;
//	delete this->surfUppercutAttack;
//	delete this->surfUppercutAttackLeft;
//	delete this->surfAirAttack;
//	delete this->surfAirAttackLeft;
//	delete this->surfAirTakeDownAttack;
//	delete this->surfAirTakeDownAttackLeft;    
//	delete this->surfAirBombAttack;
//	delete this->surfAirBombAttackLeft;
//	delete this->surfGetHurted;
//	delete this->surfGetHurtedLeft;
//	delete this->surfGetHurtedHard;
//	delete this->surfGetHurtedHardLeft;
//
//	delete impulseAttack;
//	delete repulseAttack;
//	delete dust;
//	delete airBombAttack;
//
//	Mix_FreeChunk(this->hitSound);
//	Mix_FreeChunk(this->repulseSound);
//	Mix_FreeChunk(this->impulseSound);
//	Mix_FreeChunk(this->jumpSound);
//}
//
//void Hero::SetMoveClips()
//{
//	/* Stand clips right & left */
//    this->SetClipsLoop(true, this->clipStand, NB_SPRITES_STAND, HERO_WIDTH, HERO_HEIGHT);
//    this->SetClipsLoop(false, this->clipStandLeft, NB_SPRITES_STAND, HERO_WIDTH, HERO_HEIGHT);
//
//	/* Move clips right & left */
//    this->SetClipsLoop(true, this->clipMove, NB_SPRITES_MOVE, HERO_WIDTH, HERO_HEIGHT);
//    this->SetClipsLoop(false, this->clipMoveLeft, NB_SPRITES_MOVE, HERO_WIDTH, HERO_HEIGHT);
//
//	/* Attack clips right & left */
//    this->SetClipsLoop(true, this->clipAttack, NB_SPRITES_ATTACK, HERO_WIDTH, HERO_HEIGHT);
//    this->SetClipsLoop(false, this->clipAttackLeft, NB_SPRITES_ATTACK, HERO_WIDTH, HERO_HEIGHT);
//
//	/* Impulse clips right & left */
//    this->SetClipsLoop(true, this->clipImpulseAttack, NB_SPRITES_IMPULSE_ATTACK, HERO_IMPULSE_WIDTH, HERO_IMPULSE_HEIGHT);
//    this->SetClipsLoop(false, this->clipImpulseAttackLeft, NB_SPRITES_IMPULSE_ATTACK, HERO_IMPULSE_WIDTH, HERO_IMPULSE_HEIGHT);
//
//	/* Repulse clips right & left */
//    this->SetClipsLoop(true, this->clipRepulseAttack, NB_SPRITES_REPULSE_ATTACK, HERO_WIDTH, HERO_HEIGHT);
//    this->SetClipsLoop(false, this->clipRepulseAttackLeft, NB_SPRITES_REPULSE_ATTACK, HERO_WIDTH, HERO_HEIGHT);
//
//	/* Uppercut clips right & left */    
//    this->SetClipsLoop(true, this->clipUppercutAttack, NB_SPRITES_UPPERCUT_ATTACK, HERO_WIDTH, HERO_HEIGHT);
//    this->SetClipsLoop(false, this->clipUppercutAttackLeft, NB_SPRITES_UPPERCUT_ATTACK, HERO_WIDTH, HERO_HEIGHT);
//
//    /* Air attack clips right & left */
//    this->SetClipsLoop(true, this->clipAirAttack, NB_SPRITES_AIR_ATTACK, HERO_WIDTH, HERO_HEIGHT);
//    this->SetClipsLoop(false, this->clipAirAttackLeft, NB_SPRITES_AIR_ATTACK, HERO_WIDTH, HERO_HEIGHT);
//
//    /* Air Take down attack clips right & left */
//    this->SetClipsLoop(true, this->clipAirTakeDownAttack, NB_SPRITES_AIR_TAKE_DOWN_ATTACK, HERO_IMPULSE_WIDTH, HERO_IMPULSE_HEIGHT);
//    this->SetClipsLoop(false, this->clipAirTakeDownAttackLeft, NB_SPRITES_AIR_TAKE_DOWN_ATTACK, HERO_IMPULSE_WIDTH, HERO_IMPULSE_HEIGHT);
//
//    /* Air bomb attack clips right & left */
//    this->SetClipsLoop(true, this->clipAirBombAttack, NB_SPRITES_AIR_BOMB_ATTACK, HERO_IMPULSE_WIDTH, HERO_IMPULSE_HEIGHT);
//    this->SetClipsLoop(false, this->clipAirBombAttackLeft, NB_SPRITES_AIR_BOMB_ATTACK, HERO_IMPULSE_WIDTH, HERO_IMPULSE_HEIGHT);
//	
//	/* Get hurted clips right & left */
//    this->SetClipsLoop(true, this->clipGetHurted, NB_SPRITES_GET_HURTED, HERO_WIDTH, HERO_HEIGHT);
//    this->SetClipsLoop(false, this->clipGetHurtedLeft, NB_SPRITES_GET_HURTED, HERO_WIDTH, HERO_HEIGHT);
//
//	/* Get hurted hard clips right & left */
//    this->SetClipsLoop(true, this->clipGetHurtedHard, NB_SPRITES_GET_HURTED, HERO_WIDTH, HERO_HEIGHT);
//    this->SetClipsLoop(false, this->clipGetHurtedHardLeft, NB_SPRITES_GET_HURTED, HERO_WIDTH, HERO_HEIGHT);
//
//}
//
//void Hero::SetClipsLoop(bool rightDirection, SDL_Rect * clip, int nbSprites, int width, int height)
//{
//    if(rightDirection)
//    {
//        for(int i=0; i<nbSprites; i++)
//	    {
//		    clip[i].x = width * i;
//		    clip[i].y = 0;
//		    clip[i].w = width;
//		    clip[i].h = height;
//	    }
//    }
//    else
//    {
//	    for(int i=nbSprites-1; i>-1; i--)
//	    {
//		    clip[i].x = width * (nbSprites - i - 1);
//		    clip[i].y = 0;
//		    clip[i].w = width;
//		    clip[i].h = height;
//	    }
//    }
//}
//
//void Hero::Event(SDL_Event * e)
//{
//	if(e->type == SDL_KEYDOWN)
//	{
//		switch(e->key.keysym.sym)
//		{
//			case SDLK_d: this->HandleMove(1); break;
//			case SDLK_a: this->HandleMove(-1); break;
//			case SDLK_LEFT: 
//            {
//                if(this->jumping) this->HandleAirAttack(); 
//                else this->HandleAttack();
//                break;
//            }
//			case SDLK_RIGHT: 
//            {
//                if(this->jumping) this->HandleAirBombAttack(); 
//                else this->HandleImpulseAttack();;
//                break;
//            }
//			case SDLK_e: this->HandleRepulseAttack(); break;
//			case SDLK_UP: 
//            {
//                if(this->jumping) this->HandleAirTakeDownAttack(); 
//                else this->HandleUppercutAttack();
//                break;
//            }
//			case SDLK_DOWN: this->HandleJump(); break;
//			case SDLK_w: this->HandleAxis(-1);break;
//			case SDLK_s: this->HandleAxis(1);break;
//			case SDLK_f: this->HandleGetHurted(1); break;
//			case SDLK_x: this->HandleGetHurted(2); break;
//		}
//	}
//	else if(e->type == SDL_KEYUP)
//	{
//		switch(e->key.keysym.sym)
//		{
//			case SDLK_d: this->HandleMove(-1); break;
//			case SDLK_a: this->HandleMove(1); break;
//			case SDLK_w: this->HandleAxis(1);break;
//			case SDLK_s: this->HandleAxis(-1);break;
//		}
//	}
//}
//
//void Hero::Update(Uint32 gameTime, std::vector<Projectile*>& heroProjectiles)
//{
//	impulseAttack->Update(gameTime);
//	repulseAttack->Update(gameTime);
//	dust->Update(gameTime);
//	airBombAttack->Update(gameTime);
//
//	this->animationStatus += gameTime;
//	if(animationStatus > animationTimer){
//		animationStatus -= animationTimer;
//		if(this->status == GET_HURTED)
//		{
//			this->GetHurted(1);
//			this->animationTimer = ANIMATION_TIMER_GET_HURTED;
//		}
//		else if(this->status == GET_HURTED_HARD)
//		{
//			this->GetHurted(2);
//			this->animationTimer = ANIMATION_TIMER_GET_HURTED;
//		}
//		else if(this->status == ATTACK)
//		{
//			this->Attack(heroProjectiles);
//			this->animationTimer = ANIMATION_TIMER_ATTACK;
//		}
//		else if(this->attackStatus == AIR_ATTACK_STARTED)
//		{
//			this->AirAttack(heroProjectiles);
//			this->animationTimer = ANIMATION_TIMER_AIR_ATTACK;
//		}
//		else if(this->status == IMPULSE_ATTACK)
//		{
//			this->ImpulseAttack();
//			this->animationTimer = ANIMATION_TIMER_IMPULSE_ATTACK;
//		}
//		else if(this->status == REPULSE_ATTACK)
//		{
//			this->RepulseAttack();
//			this->animationTimer = ANIMATION_TIMER_REPULSE_ATTACK;
//		}
//		else if(this->status == UPPERCUT_ATTACK)
//		{
//			this->UppercutAttack();
//			this->animationTimer = ANIMATION_TIMER_UPPERCUT_ATTACK;
//		}
//        else if(this->attackStatus == AIR_TAKE_DOWN_ATTACK_STARTED)
//		{
//			this->AirTakeDownAttack();
//            this->animationTimer = ANIMATION_TIMER_AIR_TAKE_DOWN_ATTACK;
//		}
//        else if(this->attackStatus == AIR_BOMB_ATTACK_STARTED)
//		{
//			this->AirBombAttack();
//            this->animationTimer = ANIMATION_TIMER_AIR_BOMB_ATTACK;
//		}
//		else
//		{
//			this->Move();
//		}    
//
//		if(this->jumping){
//            if(this->attackStatus == AIR_ATTACK_STARTED || this->attackStatus == AIR_TAKE_DOWN_ATTACK_STARTED ||  this->attackStatus == AIR_BOMB_ATTACK_STARTED)
//			{
//				this->velocityY = 0;
//			}
//			else
//			{
//				this->Jump();
//				this->animationTimer = ANIMATION_TIMER_JUMP;
//			}
//		}
//	}
//}
//
//void Hero::Draw(SDL_Surface *viewport){  	
//	if(this->jumping){
//		this->surfShadow->Draw(viewport, this->posX + HERO_WIDTH/2 - 20, this->jumpStartAtPosY + HERO_HEIGHT - 15 );
//	}
//
//	if(this->status == GET_HURTED && this->directionStatus == RIGHT)
//	{
//		this->surfGetHurted->Draw(viewport, this->posX, this->posY, this->clipGetHurted[this->frame]);
//	}
//	else if(this->status == GET_HURTED && this->directionStatus == LEFT)
//	{
//		this->surfGetHurtedLeft->Draw(viewport, this->posX, this->posY, this->clipGetHurtedLeft[this->frame]);
//	}
//	else if(this->status == GET_HURTED_HARD && this->directionStatus == RIGHT)
//	{
//		this->surfGetHurtedHard->Draw(viewport, this->posX, this->posY, this->clipGetHurtedHard[this->frame]);
//	}
//	else if(this->status == GET_HURTED_HARD && this->directionStatus == LEFT)
//	{
//		this->surfGetHurtedHardLeft->Draw(viewport, this->posX, this->posY, this->clipGetHurtedHardLeft[this->frame]);
//	}
//	else if(this->attackStatus == AIR_ATTACK_STARTED && this->directionStatus == RIGHT)
//	{
//		this->surfAirAttack->Draw(viewport, this->posX, this->posY, this->clipAirAttack[this->frame]);
//	}
//	else if(this->attackStatus == AIR_ATTACK_STARTED && this->directionStatus == LEFT)
//	{
//		this->surfAirAttackLeft->Draw(viewport, this->posX, this->posY, this->clipAirAttackLeft[this->frame]);
//	}
//    else if(this->attackStatus == AIR_TAKE_DOWN_ATTACK_STARTED && this->directionStatus == RIGHT)
//	{
//		this->surfAirTakeDownAttack->Draw(viewport, this->posX - 55, this->posY - 55, this->clipAirTakeDownAttack[this->frame]);
//	}
//	else if(this->attackStatus == AIR_TAKE_DOWN_ATTACK_STARTED && this->directionStatus == LEFT)
//	{
//		this->surfAirTakeDownAttackLeft->Draw(viewport, this->posX + 10, this->posY - 55, this->clipAirTakeDownAttackLeft[this->frame]);
//	}
//    else if(this->attackStatus == AIR_BOMB_ATTACK_STARTED && this->directionStatus == RIGHT)
//	{
//		this->surfAirBombAttack->Draw(viewport, this->posX, this->posY - 50, this->clipAirBombAttack[this->frame]);
//	}
//	else if(this->attackStatus == AIR_BOMB_ATTACK_STARTED && this->directionStatus == LEFT)
//	{
//		this->surfAirBombAttackLeft->Draw(viewport, this->posX - 50, this->posY - 50, this->clipAirBombAttackLeft[this->frame]);
//	}
//    else if(this->jumping && this->velocityY < 0)
//	{
//		if(this->directionStatus == RIGHT)
//		{
//			this->surfJumpUp->Draw(viewport, this->posX, this->posY);
//		}
//		else
//		{
//			this->surfJumpUpLeft->Draw(viewport, this->posX, this->posY);
//		}
//	}
//	else if(this->jumping && this->velocityY > 0 )
//	{
//		if(this->directionStatus == RIGHT)
//		{
//			this->surfJumpDown->Draw(viewport, this->posX, this->posY);
//		}
//		else
//		{
//			this->surfJumpDownLeft->Draw(viewport, this->posX, this->posY);
//		}
//	}
//	else if(this->status == STAND && this->directionStatus == RIGHT)
//	{
//		this->surfStand->Draw(viewport, this->posX, this->posY, this->clipStand[this->frame]);
//	}
//	else if(this->status == STAND && this->directionStatus == LEFT)
//	{
//		this->surfStandLeft->Draw(viewport, this->posX, this->posY, this->clipStandLeft[this->frame]);
//	}
//	else if(this->status == MOVE_H && this->directionStatus == RIGHT)
//	{
//		this->surfMove->Draw(viewport, this->posX, this->posY, this->clipMove[this->frame]);
//	}
//	else if(this->status == MOVE_H && this->directionStatus == LEFT)
//	{
//		this->surfMoveLeft->Draw(viewport, this->posX, this->posY, this->clipMoveLeft[this->frame]);
//	}
//	else if(this->status == ATTACK && this->directionStatus == RIGHT)
//	{
//		this->surfAttack->Draw(viewport, this->posX, this->posY, this->clipAttack[this->frame]);
//	}
//	else if(this->status == ATTACK && this->directionStatus == LEFT)
//	{
//		this->surfAttackLeft->Draw(viewport, this->posX, this->posY, this->clipAttackLeft[this->frame]);
//	}
//	else if(this->status == IMPULSE_ATTACK && this->directionStatus == RIGHT)
//	{
//		this->surfImpulseAttack->Draw(viewport, this->posX - 55, this->posY - 30, this->clipImpulseAttack[this->frame]);
//	}
//	else if(this->status == IMPULSE_ATTACK && this->directionStatus == LEFT)
//	{
//		this->surfImpulseAttackLeft->Draw(viewport, this->posX, this->posY  - 30, this->clipImpulseAttackLeft[this->frame]);
//	}
//	else if(this->status == REPULSE_ATTACK && this->directionStatus == RIGHT)
//	{
//		this->surfRepulseAttack->Draw(viewport, this->posX, this->posY, this->clipRepulseAttack[this->frame]);
//	}
//	else if(this->status == REPULSE_ATTACK && this->directionStatus == LEFT)
//	{
//		this->surfRepulseAttackLeft->Draw(viewport, this->posX, this->posY, this->clipRepulseAttackLeft[this->frame]);
//	}
//	else if(this->status == UPPERCUT_ATTACK && this->directionStatus == RIGHT)
//	{
//		this->surfUppercutAttack->Draw(viewport, this->posX, this->posY, this->clipUppercutAttack[this->frame]);
//	}
//	else if(this->status == UPPERCUT_ATTACK && this->directionStatus == LEFT)
//	{
//		this->surfUppercutAttackLeft->Draw(viewport, this->posX, this->posY, this->clipUppercutAttackLeft[this->frame]);
//	}
//
//	if(impulseAttack->Animating())
//	{
//		impulseAttack->Draw(viewport);
//	}
//
//	if(repulseAttack->Animating())
//	{
//		repulseAttack->Draw(viewport);
//	}
//
//	if(dust->Animating())
//	{
//		dust->Draw(viewport);
//	}
//
//	if(airBombAttack->Animating())
//	{
//		airBombAttack->Draw(viewport);
//	}
//
//	if(DEBUG_COLLIDER){
//		this->GetCollisionBox().Draw(viewport, 0, 255, 0);
//	}
//}
//
//
//int Hero::GetPositionY(){
//	return this->posY;
//}
//
//int Hero::GetPositionX(){
//	return this->posX;
//}
//
//int Hero::GetPositionZ(){
//	if(!this->jumping){
//		return this->posY;
//	}else{
//		return this->jumpStartAtPosY;
//	}
//}
//
//void Hero::SetPositionX(int newX){
//	this->posX = newX;
//}
//
//void Hero::SetZLimit(int max, int min){
//	this->maxZ = max;
//	this->minZ = min;
//}
//
//bool Hero::IsJumping(){
//	return this->jumping;
//}
//
//bool Hero::IsJumpingUp(){
//	return this->jumping && this->velocityY <= 0;
//}
//
//int Hero::GetVelocityX(){
//	return this->velocityX;
//}
//
//int Hero::GetLife(){
//	return this->life;
//}
//
//int Hero::GetHealth(){
//	return this->health;
//}
//
//int Hero::GetMovingDirection(){
//	if(this->status == MOVE_H && this->directionStatus == RIGHT){
//		return 1;
//	}else if(this->status == MOVE_H && this->directionStatus == LEFT){
//		return -1;
//	}else{
//		return 0;
//	}
//}
//
//bool Hero::HeroIsInAction()
//{
//	if(this->jumping || this->attackStatus == IMPULSE_ATTACK_STARTED || this->attackStatus == REPULSE_ATTACK_STARTED || this->attackStatus == UPPERCUT_ATTACK_STARTED)
//	{
//		return true;
//	}
//	return false;
//}
//
//void Hero::HandleGetHurted(int type)
//{
//	if( type == 1  && (this->status != GET_HURTED || (this->status == GET_HURTED && this->frame >= NB_SPRITES_GET_HURTED - 1)))
//	{					
//		this->status = GET_HURTED;
//		this->attackStatus = FINISHED;
//		this->frame = 0;
//		this->canJump = false;
//		Mix_PlayChannel(-1, this->hitSound, false);
//	}
//	else if( type == 2  && (this->status != GET_HURTED_HARD || (this->status == GET_HURTED_HARD && this->frame >= NB_SPRITES_GET_HURTED_HARD - 1)))
//	{					
//		this->status = GET_HURTED_HARD;
//		this->attackStatus = FINISHED;
//		this->frame = 0;
//		this->canJump = false;
//		Mix_PlayChannel(-1, this->hitSound, false);
//	}
//}
//
//void Hero::GetHurted(int type)
//{
//	this->frame++;
//	
//	if(type == 1)
//	{
//		if(this->frame >= NB_SPRITES_GET_HURTED && (this->status == GET_HURTED))
//		{
//			this->attackStatus = FINISHED;
//			this->status = STAND;
//			this->canJump = true;
//		}
//		else if(this->status == GET_HURTED && this->frame >= 4)
//		{	
//			this->canJump = true;
//		}
//	}
//	else if(type == 2)
//	{
//		this->posX += (this->directionStatus == RIGHT) ? -10 : 10;
//
//		if(this->frame >= NB_SPRITES_GET_HURTED_HARD && (this->status == GET_HURTED_HARD))
//		{
//			this->attackStatus = FINISHED;
//			this->status = STAND;
//			this->canJump = true;
//		}
//		else if(this->status == GET_HURTED_HARD && this->frame >= 4)
//		{	
//			this->canJump = true;
//		}
//	}
//}
//
//void Hero::Hit(Projectile* proj){
//
//	if(proj->GetDirection().X > 0){
//		this->directionStatus = LEFT;
//	}else if(proj->GetDirection().X < 0){
//		this->directionStatus = RIGHT;
//	}
//
//	this->health -= (int)proj->GetPower();
//	if(this->health <= 0){
//		this->life--;
//		this->health = 100;
//	}
//	if((int)proj->GetPower() < 10){
//		this->HandleGetHurted(1);
//	}else{
//		this->HandleGetHurted(2);
//	}
//}
//
//Rectangle Hero::GetCollisionBox(){
//	return Rectangle((float)this->posX + 75, (float)this->posY + 50, 30, 30);
//}
//
//void Hero::HandleAttack()
//{
//	if(this->attackStatus == FINISHED || (this->attackStatus == ATTACK_STARTED && this->frame >= NB_SPRITES_ATTACK / 2))
//	{					
//		this->status = ATTACK;
//		this->attackStatus = ATTACK_STARTED;
//		this->frame = 0;
//		this->canJump = false;
//		Mix_PlayChannel(-1, this->hitSound, false);
//	}	
//}
//
//void Hero::Attack(std::vector<Projectile*>& heroProjectiles)
//{
//	if(this->frame == 0){
//		int direction = 1;
//		if(this->directionStatus == LEFT){
//			direction = -1;
//		}
//		heroProjectiles.push_back(this->CreateProjectile());
//	}
//
//	this->frame++;
//	
//	if(this->frame >= NB_SPRITES_ATTACK && (this->attackStatus == AIR_ATTACK_STARTED || this->status == ATTACK))
//    {
//		this->attackStatus = FINISHED;
//		this->status = STAND;
//		this->canJump = true;
//    }
//	else if(this->attackStatus == ATTACK_STARTED && this->frame >= 1)
//	{	
//		this->canJump = true;
//	}
//}
//
//Projectile* Hero::CreateProjectile(){
//	int direction = 1;
//	if(this->directionStatus == LEFT){
//		direction = -1;
//	}
//	return new Projectile(this->GetCollisionBox(), 0.25f, 10.0f, Vector2(direction, 0), 250);
//}
//
//void Hero::HandleAirAttack()
//{
//    if(this->attackStatus == FINISHED || (this->attackStatus == AIR_ATTACK_STARTED && this->frame >= NB_SPRITES_AIR_ATTACK / 2))
//	{	
//		this->attackStatus = AIR_ATTACK_STARTED;
//		this->frame = 0;
//		this->canJump = false;
//		Mix_PlayChannel(-1, this->hitSound, false);
//	}
//}
//
//void Hero::AirAttack(std::vector<Projectile*>& heroProjectiles)
//{
//	if(this->frame == 0){
//		int direction = 1;
//		if(this->directionStatus == LEFT){
//			direction = -1;
//		}
//		heroProjectiles.push_back(this->CreateProjectile());
//	}
//	this->frame++;
//	
//	if(this->frame >= NB_SPRITES_AIR_ATTACK && this->attackStatus == AIR_ATTACK_STARTED)
//    {
//		this->attackStatus = FINISHED;
//		this->status = STAND;
//		this->canJump = true;
//    }
//}
//
//void Hero::HandleAirTakeDownAttack()
//{
//    if(this->attackStatus == FINISHED || (this->attackStatus == AIR_ATTACK_STARTED && this->frame >= NB_SPRITES_AIR_ATTACK / 2))
//	{	
//        this->attackStatus = AIR_TAKE_DOWN_ATTACK_STARTED;
//		this->frame = 0;
//		this->canJump = false;
//		Mix_PlayChannel(-1, this->impulseSound, false);
//	}
//}
//
//void Hero::AirTakeDownAttack()
//{
//    this->frame++;
//	
//    if(this->frame >= NB_SPRITES_AIR_TAKE_DOWN_ATTACK && this->attackStatus == AIR_TAKE_DOWN_ATTACK_STARTED)
//    {
//		this->attackStatus = FINISHED;
//		this->status = STAND;
//		this->canJump = true;
//    }
//}
//
//void Hero::HandleAirBombAttack()
//{
//    if(this->attackStatus == FINISHED || (this->attackStatus == AIR_ATTACK_STARTED && this->frame >= NB_SPRITES_AIR_ATTACK / 2))
//	{	
//        this->attackStatus = AIR_BOMB_ATTACK_STARTED;
//		this->frame = 0;
//		this->canJump = false;
//		Mix_PlayChannel(-1, this->repulseSound, false);
//	}
//}
//
//void Hero::AirBombAttack()
//{
//    this->frame++;
//    if(this->posY < this->jumpStartAtPosY)
//    {
//        this->posY += AIR_BOMB_ATTACK_SPEED;
//    }
//    else
//    {
//        this->posY = this->jumpStartAtPosY;
//    }
//	
//	if(this->frame == NB_SPRITES_AIR_BOMB_ATTACK - 6 && this->attackStatus == AIR_BOMB_ATTACK_STARTED)
//    {
//		int direction = (this->directionStatus == RIGHT) ? 1 : -1;
//		airBombAttack->Start(this->posX, this->posY, direction);
//    }
//    else if(this->frame >= NB_SPRITES_AIR_BOMB_ATTACK && this->attackStatus == AIR_BOMB_ATTACK_STARTED)
//    {
//		this->attackStatus = FINISHED;
//		this->status = STAND;
//		this->canJump = true;
//    }
//}
//
//void Hero::HandleImpulseAttack()
//{
//	if(!this->HeroIsInAction())
//	{
//		this->status = IMPULSE_ATTACK; 
//		this->attackStatus = IMPULSE_ATTACK_STARTED;
//		this->frame = 0;
//		this->canJump = false;
//		Mix_PlayChannel(-1, this->impulseSound, false);
//	}
//}
//
//void Hero::ImpulseAttack()
//{
//	this->frame++;
//
//	if(this->frame >= NB_SPRITES_IMPULSE_ATTACK && this->status == IMPULSE_ATTACK)
//    {
//		int direction = (this->directionStatus == RIGHT) ? 1 : -1;
//		impulseAttack->Start(this->posX, this->posY, direction);
//		this->attackStatus = FINISHED;
//		this->status = STAND;
//		this->canJump = true;
//    }
//}
//
//void Hero::HandleRepulseAttack()
//{
//	if(!this->HeroIsInAction())
//	{
//		this->status = REPULSE_ATTACK; 
//		this->attackStatus = REPULSE_ATTACK_STARTED;
//		this->frame = 0;
//		this->health -= 5;
//		this->canJump = false;
//		Mix_PlayChannel(-1, this->repulseSound, false);
//	}
//}
//
//void Hero::RepulseAttack()
//{
//	this->frame++;
//
//	if(this->frame == NB_SPRITES_REPULSE_ATTACK / 2 && this->status == REPULSE_ATTACK)
//    {
//		int direction = (this->directionStatus == RIGHT) ? 1 : -1;
//		repulseAttack->Start(this->posX, this->posY, direction);
//    }
//	if(this->frame >= NB_SPRITES_REPULSE_ATTACK && this->status == REPULSE_ATTACK)
//	{
//		this->attackStatus = FINISHED;
//		this->status = STAND;
//		this->canJump = true;
//	}
//}
//
//void Hero::HandleUppercutAttack()
//{
//	if(!this->HeroIsInAction())
//	{
//		this->status = UPPERCUT_ATTACK; 
//		this->attackStatus = UPPERCUT_ATTACK_STARTED;
//		this->frame = 0;
//		this->canJump = false;
//		Mix_PlayChannel(-1, this->hitSound, false);
//	}
//	else if(this->attackStatus == UPPERCUT_ATTACK_STARTED && this->frame >= NB_SPRITES_UPPERCUT_ATTACK / 2)
//	{			
//		this->attackStatus = UPPERCUT_ATTACK_STARTED;
//		this->frame = 0;
//		this->canJump = false;
//		Mix_PlayChannel(-1, this->hitSound, false);
//	}	
//}
//
//void Hero::UppercutAttack()
//{
//	this->frame++;
//
//	if(this->frame >= NB_SPRITES_UPPERCUT_ATTACK && this->status == UPPERCUT_ATTACK)
//    {
//		this->attackStatus = FINISHED;
//		this->status = STAND;
//		this->canJump = true;
//    }
//	else if(this->attackStatus == UPPERCUT_ATTACK_STARTED && this->frame >= 1)
//	{	
//		this->canJump = true;
//	}
//}
//
//void Hero::HandleMove(int direction)
//{
//	this->velocityX += HERO_SPEED_X * direction;
//}
//
//void Hero::HandleAxis(int direction){
//	this->velocityZ += HERO_SPEED_Z * direction;
//}
//
//void Hero::Move()
//{
//    if(this->jumping)
//    {
//	    this->posX += velocityX/2;
//    }
//    else
//    {
//        this->posX += velocityX;
//    }
//	
//    if(this->posX < 0)
//    {
//		this->posX = 0;
//    }
//
//    if(this->posX + HERO_WIDTH> RES_WIDTH)
//    {
//		this->posX = RES_WIDTH - HERO_WIDTH;
//    }
//
//    if(this->velocityX < 0)
//    {
//		this->status = MOVE_H;
//		this->directionStatus = LEFT;
//		this->animationTimer = ANIMATION_TIMER_MOVE;
//    }
//    else if(this->velocityX > 0)
//    {
//		this->status = MOVE_H;
//		this->directionStatus = RIGHT;
//		this->animationTimer = ANIMATION_TIMER_MOVE;
//    }
//    else
//    {
//		this->status = STAND;
//		this->animationTimer = ANIMATION_TIMER_STAND;
//    }
//
//	if(this->velocityZ != 0 && !this->jumping){
//		if(this->posY + velocityZ < this->maxZ && this->posY + velocityZ > this->minZ){
//			this->posY += velocityZ;
//		}
//		this->animationTimer = ANIMATION_TIMER_MOVE;
//		this->status = MOVE_H;
//	}else if(this->jumping){
//		if(this->jumpStartAtPosY + velocityZ < this->maxZ && this->jumpStartAtPosY + velocityZ > this->minZ){
//			this->posY += velocityZ/2;
//			this->jumpStartAtPosY += velocityZ/2;
//		}
//	}
//
//	this->frame++;
//
//    if(this->frame >= NB_SPRITES_STAND && this->status == STAND)
//    {
//		this->frame = 0;
//    }
//	else if(this->frame >= NB_SPRITES_MOVE && this->status == MOVE_H)
//    {
//		this->frame = 0;
//    }
//}
//
//void Hero::HandleJump()
//{
//	if(!this->jumping && this->canJump)
//	{
//        this->velocityY = -JUMP_MAX_VELOCITY;
//		this->jumpStartAtPosY = this->posY;
//		this->jumping = true;		
//		dust->Start(this->posX, this->posY, 1);
//		Mix_PlayChannel(-1, this->jumpSound, false);
//	}
//}
//
//void Hero::Jump()
//{
//    
//    this->posY += this->velocityY;
//    if(this->velocityY < JUMP_MAX_VELOCITY)
//    {
//        this->velocityY += JUMP_THRESHOLD;
//    }
//    else
//    {
//        this->velocityY = JUMP_MAX_VELOCITY;
//    }
//
//    if(this->posY >= this->jumpStartAtPosY)
//    {
//        this->posY = this->jumpStartAtPosY;
//        this->velocityY = 0;
//		this->jumping = false;
//		Mix_PlayChannel(-1, this->jumpSound, false);
//    }
//}
