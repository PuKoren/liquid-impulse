#include "Effect.h"

Effect::Effect(std::string _type, std::string _effectName, int _nbFrame, int _animationMs, Vector2 _shiftPosition, float _velocity, int _delay, bool _reverse){    
    this->Load(_type, _effectName, _nbFrame, _animationMs, _shiftPosition, _reverse);
    this->Init(_velocity, _delay);
}

Effect::~Effect(){
    delete this->surface;
}

// Initialise effect with position, direction and velocity
void Effect::Init(float _velocity, int _delay){
    this->position = Vector2(0,0);
    this->direction = Vector2(0,0);
    this->velocity = _velocity;
    this->currentDelay = 0;
    this->delay = _delay;
    this->effectEnded = true;
}

// Load effect content
void Effect::Load(std::string _type, std::string _effectName, int _nbFrame, int _animationMs, Vector2 _shiftPosition, bool _reverse){
	// Load effect sprites into a surface
	this->surface = new Surface("Resources/Effects/"+ _type + "/" + _effectName +".png", _nbFrame, _animationMs, _shiftPosition, _reverse);
}

void Effect::Update(Uint32 gameTime){
    if(this->effectEnded || this->EffectEnded())
		return;

    this->currentDelay += gameTime;

    if(this->currentDelay >= this->delay){
        this->position.X += this->direction.X * gameTime * this->velocity;
        this->position.Y += this->direction.Y * gameTime * this->velocity;
        this->surface->Update(gameTime);
    }
}

void Effect::Draw(SDL_Surface * viewport){
	// Draw the animation sprite
    if(!this->effectEnded && this->currentDelay >= this->delay){
	    this->surface->DrawSprite(viewport, (int)this->position.X, (int)this->position.Y);
	    if(DEBUG_COLLIDER){
		    this->GetCollisionBox().Draw(viewport, 255, 0, 0);
	    }
    }
}

void Effect::StartEffect(Vector2 _position, Vector2 _direction){
    this->effectEnded = false;
    this->currentDelay = 0;
    this->position = _position;
    this->direction = _direction;
    this->surface->ResetLoop();
    this->surface->ResetSprite();
}

bool Effect::EffectEnded(){
    if(!this->effectEnded && this->surface->HasLooped()){
        this->effectEnded = true;
        this->position = Vector2(0 - this->surface->GetWidth(), 0 - this->surface->GetHeight());
        return true;
    }
    return false;
}

// Return the collision box
Rectangle Effect::GetCollisionBox(){
	return Rectangle(this->position.X + 75, this->position.Y + 50, 30, 30);
}



//#include "Effect.h"
//
//Effect::Effect(int type)
//{
//	this->type = type;
//
//	if(this->type == 1)
//	{
//		this->posX = 0 - EFFECT_WIDTH_IMPULSE_ATTACK;
//		this->posY = 0 - EFFECT_HEIGHT_IMPULSE_ATTACK;
//
//		this->animationTimer = ANIMATION_TIMER_IMPULSE_ATTACK;
//
//		this->surfMove = new Surface("Resources/Effects/Attacks/impulse_attack_effect.png");
//		this->surfMoveLeft = new Surface("Resources/Effects/Attacks/impulse_attack_effect_left.png");
//	}
//	else if(this->type == 2)
//	{
//		this->posX = 0 - EFFECT_WIDTH_REPULSE_ATTACK;
//		this->posY = 0 - EFFECT_HEIGHT_REPULSE_ATTACK;
//
//		this->animationTimer = ANIMATION_TIMER_REPULSE_EFFECT;
//
//		this->surfMove = new Surface("Resources/Effects/Attacks/repulse_attack_effect.png");		
//		//SDL_SetAlpha(this->surfMove->surface, SDL_SRCALPHA, 190); 
//		this->surfMoveLeft = NULL;
//	}
//	else if(this->type == 3)
//	{
//		this->posX = 0 - EFFECT_WIDTH_DUST;
//		this->posY = 0 - EFFECT_HEIGHT_DUST;
//
//		this->animationTimer = ANIMATION_TIMER_DUST;
//
//		this->surfMove = new Surface("Resources/Effects/Environment/jump_dust.png");
//		this->surfMoveLeft = NULL;
//	}
//	else if(this->type == 4)
//	{
//		this->posX = 0 - EFFECT_WIDTH_AIR_BOMB;
//		this->posY = 0 - EFFECT_HEIGHT_AIR_BOMB;
//
//		this->animationTimer = ANIMATION_TIMER_AIR_BOMB;
//
//		this->surfMove = new Surface("Resources/Effects/Attacks/air_bomb_attack_effect.png");
//		this->surfMoveLeft = NULL;
//	}
//			
//	this->frame = 0;
//	this->direction = 1;
//	this->animationStatus = 0;
//	this->velocityX = 0;
//	this->started = false;
//	this->SetMoveClips();
//}
//
//void Effect::SetMoveClips()
//{
//	if(this->type == 1)
//	{
//		/* Move right */
//		for(int i=0; i<NB_SPRITES_IMPULSE_EFFECT; i++)
//		{
//			this->clipMove[i].x = EFFECT_WIDTH_IMPULSE_ATTACK * i;
//			this->clipMove[i].y = 0;
//			this->clipMove[i].w = EFFECT_WIDTH_IMPULSE_ATTACK;
//			this->clipMove[i].h = EFFECT_HEIGHT_IMPULSE_ATTACK;
//		}
//
//		/* Move left */
//		for(int i=NB_SPRITES_IMPULSE_EFFECT-1; i>-1; i--)
//		{
//			this->clipMoveLeft[i].x = EFFECT_WIDTH_IMPULSE_ATTACK * (NB_SPRITES_IMPULSE_EFFECT - i - 1);
//			this->clipMoveLeft[i].y = 0;
//			this->clipMoveLeft[i].w = EFFECT_WIDTH_IMPULSE_ATTACK;
//			this->clipMoveLeft[i].h = EFFECT_HEIGHT_IMPULSE_ATTACK;
//		}
//	}
//	else if(this->type == 2)
//	{
//		/* Animation */
//		for(int i=0; i<NB_SPRITES_REPULSE_EFFECT; i++)
//		{
//			this->clipMove[i].x = EFFECT_WIDTH_REPULSE_ATTACK * i;
//			this->clipMove[i].y = 0;
//			this->clipMove[i].w = EFFECT_WIDTH_REPULSE_ATTACK;
//			this->clipMove[i].h = EFFECT_HEIGHT_REPULSE_ATTACK;
//		}
//	}
//	else if(this->type == 3)
//	{
//		/* Animation */
//		for(int i=0; i<NB_SPRITES_DUST_EFFECT; i++)
//		{
//			this->clipMove[i].x = EFFECT_WIDTH_DUST * i;
//			this->clipMove[i].y = 0;
//			this->clipMove[i].w = EFFECT_WIDTH_DUST;
//			this->clipMove[i].h = EFFECT_HEIGHT_DUST;
//		}
//	}
//	else if(this->type == 4)
//	{
//		/* Animation */
//		for(int i=0; i<NB_SPRITES_AIR_BOMB_EFFECT; i++)
//		{
//			this->clipMove[i].x = EFFECT_WIDTH_AIR_BOMB * i;
//			this->clipMove[i].y = 0;
//			this->clipMove[i].w = EFFECT_WIDTH_AIR_BOMB;
//			this->clipMove[i].h = EFFECT_HEIGHT_AIR_BOMB;
//		}
//	}
//}
//
//Effect::~Effect()
//{
//	delete this->surfMove;
//	delete this->surfMoveLeft;
//}
//
//void Effect::Event(SDL_Event * e)
//{
//	
//}
//
//void Effect::Update(Uint32 gameTime){
//	if(this->started == true)
//	{
//		this->animationStatus += gameTime;
//		if(animationStatus > animationTimer){
//			animationStatus -= animationTimer;
//
//			this->frame++;
//
//			if(this->type == 1)
//			{
//				this->posX += velocityX;
//				if(this->frame >= NB_SPRITES_IMPULSE_EFFECT)
//				{
//					this->Finish();
//				}	
//			}
//			else if(this->type == 2 && this->frame >= NB_SPRITES_REPULSE_EFFECT)
//			{
//				this->Finish();
//			}
//			else if(this->type == 3 && this->frame >= NB_SPRITES_DUST_EFFECT)
//			{
//				this->Finish();
//			}
//			else if(this->type == 4 && this->frame >= NB_SPRITES_AIR_BOMB_EFFECT)
//			{
//				this->Finish();
//			}
//		}
//	}
//}
//
//void Effect::Draw(SDL_Surface *viewport)
//{  
//	if(this->started && this->type == 1)
//	{
//		if(this->direction == 1)
//		{
//			this->surfMove->Draw(viewport, this->posX, this->posY - 70, this->clipMove[this->frame]);
//		}
//		else
//		{
//			this->surfMoveLeft->Draw(viewport, this->posX - 50, this->posY - 70, this->clipMoveLeft[this->frame]);
//		}
//	}
//	else if(this->started && this->type == 2)
//	{
//		if(this->direction == 1)
//		{
//			this->surfMove->Draw(viewport, this->posX - 60, this->posY - 25, this->clipMove[this->frame]);
//		}
//		else
//		{
//			this->surfMove->Draw(viewport, this->posX - 20, this->posY - 25, this->clipMove[this->frame]);
//		}
//	}
//	else if(this->started && this->type == 3)
//	{
//		this->surfMove->Draw(viewport, this->posX + 15, this->posY, this->clipMove[this->frame]);
//	}
//	else if(this->started && this->type == 4)
//	{
//		if(this->direction == 1)
//		{
//			this->surfMove->Draw(viewport, this->posX - 50, this->posY - 100, this->clipMove[this->frame]);
//		}
//		else
//		{
//			this->surfMove->Draw(viewport, this->posX - 20, this->posY - 100, this->clipMove[this->frame]);
//		}
//	}
//}
//
//void Effect::Start(int x, int y, int direction)
//{
//	this->started = true;
//	this->posX = x;
//	this->posY = y;
//	this->direction = direction;
//	this->velocityX = this->direction * EFFECT_SPEED_IMPULSE_ATTACK;
//	this->frame = 0;
//}
//
//void Effect::Finish()
//{
//	this->started = false;
//
//	if(this->type == 1)
//	{
//		this->posX = 0 - EFFECT_WIDTH_IMPULSE_ATTACK;
//		this->posY = 0 - EFFECT_HEIGHT_IMPULSE_ATTACK;
//	}
//	else if(this->type == 2)
//	{
//		this->posX = 0 - EFFECT_WIDTH_REPULSE_ATTACK;
//		this->posY = 0 - EFFECT_HEIGHT_REPULSE_ATTACK;
//	}
//	else if(this->type == 3)
//	{
//		this->posX = 0 - EFFECT_WIDTH_DUST;
//		this->posY = 0 - EFFECT_HEIGHT_DUST;
//	}
//	else if(this->type == 4)
//	{
//		this->posX = 0 - EFFECT_WIDTH_AIR_BOMB;
//		this->posY = 0 - EFFECT_HEIGHT_AIR_BOMB;
//	}
//}
//
//bool Effect::Animating()
//{
//	return this->started;
//}
