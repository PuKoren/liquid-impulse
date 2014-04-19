#ifndef GAMEOVER_H
#define GAMEOVER_H
#include <SDL.h>
#include "Config.h"
#include "Colors.h"
#include "Surface.h"
#include "DrawString.h"

class GameOver
{
private:
    int score;
    SDL_Surface* scoreSurf;
    SDL_Surface* gameoverSurf;
public:
    GameOver(int score);
    ~GameOver();
    void Event(SDL_Event *, GameState *);
    void Update(Uint32);
    void Draw(SDL_Surface *);
};

#endif // GAMEOVER_H
