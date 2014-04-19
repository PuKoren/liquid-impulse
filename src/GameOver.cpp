#include "GameOver.h"

GameOver::GameOver(int score)
{
    this->score = score;
    this->scoreSurf = DrawString::GetSurface(score, FONT_TITLE, WHITE);
    this->gameoverSurf = DrawString::GetSurface("GAME OVER", FONT_TITLE, WHITE);
}

GameOver::~GameOver(){
    SDL_FreeSurface(this->scoreSurf);
    SDL_FreeSurface(this->gameoverSurf);
}

void GameOver::Event(SDL_Event *e, GameState *gs){
    if(e->type == SDL_KEYDOWN){
        if(e->key.keysym.sym == SDLK_ESCAPE || e->key.keysym.sym == SDLK_s || e->key.keysym.sym == SDLK_RETURN){
            *gs = GAME_LOAD_MENU;
        }
    }
}

void GameOver::Update(Uint32 gameTime){

}

void GameOver::Draw(SDL_Surface *viewport){
    SDL_FillRect(viewport, NULL, 0x0);
    SDL_Rect destR;
    destR.x = viewport->w/2 - this->gameoverSurf->w/2;
    destR.y = viewport->h/2 - this->gameoverSurf->h/2;

    SDL_BlitSurface(this->gameoverSurf, NULL, viewport, &destR);

    destR.x = viewport->w/2 - this->scoreSurf->w/2;
    destR.y = viewport->h/2 - this->scoreSurf->h/2 + this->gameoverSurf->h;
    SDL_BlitSurface(this->scoreSurf, NULL, viewport, &destR);
}
