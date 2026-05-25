#pragma once

#include <SDL.h>
#include "ball.h"

#define WINDOW_TITLE  "Billiard"


typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int running;
    Ball ball;
} Game;

int game_init(Game *g);
void game_run(Game *g);
void game_quit(Game *g);
