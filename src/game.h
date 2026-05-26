//
//  game.h
//
//
//  Created by Abdulkhamid Khamidullin on 25.05.2026.
//

#pragma once

#include <SDL.h>
#include "ball.h"

#define WINDOW_TITLE  "Billiard"


typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int running;
    Ball balls[BALL_COUNT];
} Game;

int game_init(Game *g);
void game_run(Game *g);
void game_quit(Game *g);
