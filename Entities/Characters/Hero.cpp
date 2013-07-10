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
    #if PSP
    this->velocity = _velocity * 1.25;
    #else
    this->velocity = _velocity;
    #endif
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
        this->heroProjectiles->push_back(this->CreateProjectile(Vector2(1, 0), Vector2(0,0), 10, 0.6f, 1.0f, 6.0f, 300));
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
        this->heroProjectiles->push_back(this->CreateProjectile(Vector2(0, -1), Vector2(40, 0), 5, 0.1f, 1.5f, 1.0f, 100));
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
