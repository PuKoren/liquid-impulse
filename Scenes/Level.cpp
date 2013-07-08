#include "Level.h"
#include "../Tools/DrawString.h"
#include "../Tools/Colors.h"
#include <iostream>

Enemy * terr_neg;

Level::Level(){
	this->backgroundSurfaces = new BackgroundManager("town", 8);
    this->hero = new Hero(&this->heroProjectiles);
	this->Load();
	this->HeroPosition = 200*RATIO;
	this->BackgroundForce = 0.4f*RATIO;
	this->freeMove = true;
	this->hero->SetPositionX(this->HeroPosition);
	this->arrow_right = new Surface("Resources/Textures/UI/arrow_right.png");
	this->hero_icon = new Surface("Resources/Textures/UI/hero_icon.png");
    this->arrow_text = DrawString::GetSurface("Go!", FONT_INGAME_UI, WHITE);
    this->score_text = DrawString::GetSurface(0, FONT_SCORE, WHITE);
    this->lifes_text = DrawString::GetSurface(this->hero->GetLife(), FONT_INGAME_UI, WHITE);
	this->virtualX = 0;
	this->Score = 0;
	this->ScoreMultiplier = 0;
	this->wave = 1;
	this->gravity = 0.003f*RATIO;
	this->maxWave = 20;
	this->difficulty = 1;
	this->bloodEngine = ParticleEngine("blood", this->gravity);

	terr_neg = new Enemy("neg_terry", 100, 0.1f, 100);

	this->GenerateWave();

	this->firstRun = true;
}

Level::~Level(){
    SDL_FreeSurface(this->arrow_text);
    SDL_FreeSurface(this->score_text);
    SDL_FreeSurface(this->lifes_text);

	delete this->backgroundSurfaces;
	delete this->hero;
	delete this->hero_icon;
	delete this->arrow_right;

	unsigned int i = 0;
    for(i = 0; i < this->enemies.size(); i++){
		delete this->enemies[i];
	}
	this->enemies.clear();

	for(i = 0; i < this->enemiesProjectiles.size(); i++){
		delete this->enemiesProjectiles[i];
	}
	this->enemiesProjectiles.clear();

	for(i = 0; i < this->heroProjectiles.size(); i++){
		delete this->heroProjectiles[i];
	}
	this->heroProjectiles.clear();

	delete terr_neg;
	Mix_FreeMusic(this->backgroundMusic);
}

void Level::Load(){
	this->backgroundMusic = Mix_LoadMUS("Resources/Sounds/Music/Danjyon_Kimura_Count_End.ogg");
	Mix_PlayMusic(this->backgroundMusic,-1);
}

void Level::Event(SDL_Event *e, GameState *gs){
	if(e->key.keysym.sym == SDLK_ESCAPE){
		*gs = GAME_LOAD_MENU;
	}
	this->hero->Event(e);
}


int arrow_blink_timer = 0;
int arrow_blink_speed = 300;
bool arrow_blink_status = true;

//smoother to move the background with a float value instead of int
float backgroundStatus = 0;
void Level::Update(Uint32 gameTime){

	if(firstRun){
		gameTime = 0;
		firstRun = false;
	}
	//update hero logic
    this->hero->Update(gameTime, this->heroProjectiles);
	//if scene is in "free move" state
	if(this->freeMove){
		//get the current direction of the hero and increment the virtual X of the scene
		if(this->hero->GetMovingDirection() == 1){
			this->virtualX += gameTime;
		}else if(this->hero->GetMovingDirection() == -1){
			this->virtualX -= gameTime;
		}

		//smooth background movement
		backgroundStatus += this->BackgroundForce * gameTime * RATIO;
		this->backgroundSurfaces->Move((int)backgroundStatus * this->hero->GetMovingDirection());
		if(backgroundStatus >= 1){
			backgroundStatus = 0;
		}

		//arrow blink timer, make it blink !
		arrow_blink_timer += gameTime;
		if(arrow_blink_timer > arrow_blink_speed){
			arrow_blink_status = !arrow_blink_status;
			arrow_blink_timer -= arrow_blink_speed;
		}

		//we replace the hero since we dont want it to move on screen
        this->hero->SetPositionX(this->HeroPosition);

		//if hero moved to the right for 1 second, we lock the screen
		if(this->virtualX > 1000){
			this->freeMove = false;
		}
	}else{
		//scene is in fight mode !
		//enemy logic
		//new way to manage depth, using the main loop
		//exact order is not realy need at every frame so we admit a bit of errors for better perfs
		int size = this->enemies.size();
		for(int i = 0; i < size; i++){
			this->enemies.at(i)->Update(gameTime, *this->hero, this->heroProjectiles, this->enemiesProjectiles);
			if(i > 0){
				if(this->enemies[i-1]->GetPosition().Y > this->enemies[i]->GetPosition().Y){
					Enemy *tmp = this->enemies[i-1];
					this->enemies[i-1] = this->enemies[i];
					this->enemies[i] = tmp;
				}
			}
		}

		//check if fight mode is over
		if(this->enemies.size() == 0 && this->wave < this->maxWave){
			this->freeMove = true;
			this->virtualX = 0;
			this->GenerateWave();
		}
	}
	this->CollisionDetection(gameTime);
	this->bloodEngine.Update(gameTime);
}

void Level::CollisionDetection(Uint32 gameTime){
	
	//enemy hits collision detection
	int size = this->enemiesProjectiles.size();
	Rectangle heroCollision = this->hero->GetCollisionBox();
    bool hit = false;
	for(int i = 0; i < size; i++){
		this->enemiesProjectiles[i]->Update(gameTime);
		if(this->enemiesProjectiles[i]->GetCollisionBox().CollideWith(&heroCollision)){
			this->enemiesProjectiles[i]->SetAlive(false);
			this->SpreadBlood(heroCollision.Position, this->enemiesProjectiles[i]->GetDirection(), this->enemiesProjectiles[i]->GetPower());
			this->ScoreMultiplier = 0;
			this->hero->Hit(this->enemiesProjectiles[i]);
            hit = true;
		}
		if(!this->enemiesProjectiles[i]->IsAlive()){
			delete (Projectile *)this->enemiesProjectiles[i];
			this->enemiesProjectiles[i] = NULL;
			this->enemiesProjectiles.erase(this->enemiesProjectiles.begin()+i);
			size--;
			i--;
		}
	}
    if(hit){
        SDL_FreeSurface(this->lifes_text);
        this->lifes_text = DrawString::GetSurface(this->hero->GetLife(), FONT_INGAME_UI, WHITE);
    }

	//hero hits collision detection
	size = this->heroProjectiles.size();
	for(int i = 0; i < size; i++){
		this->heroProjectiles[i]->Update(gameTime);
		int enemySize = this->enemies.size();
		Rectangle projCollision = this->heroProjectiles[i]->GetCollisionBox();
		for(int j = 0; j < enemySize; j++){
			if(this->enemies[j]->GetCollisionBox().CollideWith(&projCollision)){
				if(this->enemies[j]->Hit(this->heroProjectiles[i])){
					this->SpreadBlood(this->enemies[j]->GetCollisionBox().Position, this->heroProjectiles[i]->GetDirection(), this->heroProjectiles[i]->GetPower());
					this->heroProjectiles[i]->AddCollision();
					if(!this->enemies[j]->IsAlive()){
						delete (Enemy *)this->enemies[j];
						this->enemies.erase(this->enemies.begin()+j);
						enemySize --;
						j --;
					}
				}
			}
		}

		if(!this->heroProjectiles[i]->IsAlive()){
            SDL_FreeSurface(this->score_text);
            this->score_text = DrawString::GetSurface(this->Score, FONT_SCORE, WHITE);
			this->Score += (int)(this->ScoreMultiplier * this->heroProjectiles[i]->GetPower() * this->heroProjectiles[i]->GetCollision());
			this->ScoreMultiplier += this->heroProjectiles[i]->GetCollision();
			delete (Projectile *)this->heroProjectiles[i];
			this->heroProjectiles.erase(this->heroProjectiles.begin()+i);
			size --;
			i --;
		}
	}
}

void Level::SpreadBlood(Vector2 position, Vector2 direction, float power){
	for(int i = 0; i < power*2*RATIO; i++){
		this->bloodEngine.AddParticle(position, Vector2((float)(direction.X * (rand()%100)), (float)(-1 * (rand()%100))).Normalize(), (rand()%10+1) * 0.025f, 500);
	}
}

void Level::GenerateWave(){
	for(int i = 0; i < this->wave * (this->difficulty*2); i++){
		this->enemies.push_back(new Enemy(*terr_neg, this->difficulty*100, 0.1f, 750/this->difficulty));
	}
	this->wave++;
}

//return the number of 0 in one number
//ie: 1000 = 3, 10 = 1
int GetNumberOfZero(int number){
	if(number > 100000){
		return 5;
	}else if(number > 10000) {
		return 4;
	}else if(number > 1000) {
		return 3;
	}else if(number > 100) {
		return 2;
	}else if(number > 10) {
		return 1;
	}
	return 0;
}

//draw the current scene
void Level::Draw(SDL_Surface * viewport){
	//background
	this->backgroundSurfaces->Draw(viewport);
	this->bloodEngine.Draw(viewport);

	if(this->freeMove){
		this->hero->Draw(viewport);
	}else{
		//do a depth buffer to draw properly enemies and hero
		bool hero_drawn = false;
		for(unsigned int i = 0; i < this->enemies.size(); i++){
			if(!hero_drawn){
				if(this->enemies[i]->GetPosition().Y > this->hero->GetPositionZ()){
					this->hero->Draw(viewport);
					hero_drawn = true;
				}
			}
			this->enemies[i]->Draw(viewport);
		}
		if(!hero_drawn){
			this->hero->Draw(viewport);
		}
	}

    if(DEBUG_COLLIDER){
		unsigned int i = 0;
        for(i = 0; i < this->enemiesProjectiles.size(); i++){
			this->enemiesProjectiles[i]->Draw(viewport);
		}
		for(i = 0; i < this->heroProjectiles.size(); i++){
			this->heroProjectiles[i]->Draw(viewport);
		}
	}

	//arrow "go to next screen" blinking
	if(this->freeMove && arrow_blink_status){
		this->arrow_right->Draw(viewport, RES_WIDTH - (this->arrow_right->GetWidth() + 10), RES_HEIGHT/2 - this->arrow_right->GetHeight()/2);
        SDL_Rect goRect; goRect.x = RES_WIDTH - (this->arrow_right->GetWidth()/2); goRect.y = RES_HEIGHT/2-this->arrow_right->GetHeight();
        SDL_BlitSurface(this->arrow_text, NULL, viewport, &goRect);
		//DrawString::Draw(viewport, "Go!", FONT_INGAME_UI, WHITE, RES_WIDTH - (this->arrow_right->GetWidth()/2), RES_HEIGHT/2-this->arrow_right->GetHeight());
	}

	//hero health bar
	this->hero_icon->Draw(viewport, 10*RATIO, 10*RATIO);

    SDL_Rect DestR; DestR.x = this->hero_icon->GetWidth()+15; DestR.y = 10*RATIO; DestR.h = 30 * RATIO;
	//maximum life
    DestR.h = 30 * RATIO; DestR.w = 100 * RATIO * 3;
    SDL_FillRect(viewport, &DestR, SDL_MapRGB(viewport->format, RED.r, RED.g, RED.b));
    //current life
    DestR.w = this->hero->GetHealth() * RATIO * 3;
    SDL_FillRect(viewport, &DestR, SDL_MapRGB(viewport->format, YELLOW.r, YELLOW.g, YELLOW.b));

	//score
    SDL_Rect scorePos; scorePos.x = (RES_WIDTH - (this->score_text->w + 30*RATIO)); scorePos.y = 15*RATIO;
    SDL_BlitSurface(this->score_text, NULL, viewport, &scorePos);
	//DrawString::Draw(viewport, this->Score, FONT_SCORE, WHITE, x, 15*RATIO);
	
	//lifes remaining
    SDL_Rect lifePos; lifePos.x = this->hero_icon->GetWidth()+15*RATIO; lifePos.y = 40*RATIO;
    SDL_BlitSurface(this->lifes_text, NULL, viewport, &lifePos);
	//DrawString::Draw(viewport, this->hero->GetLife(), FONT_INGAME_UI, WHITE, this->hero_icon->GetWidth()+40*RATIO, 40*RATIO);
}
