//
//  game.h
//
//
//  Created by Abdulkhamid Khamidullin on 25.05.2026.
//

#pragma once

#include <SDL.h>
#include "ball.h"
#include "input.h"
#include "table.h"
#include "hud.h"
#include "audio.h"

#define WINDOW_TITLE  "Billiard"


typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int running;
    int fullscreen;
    int game_over;
    int current_player;
    int balls_moving;
    Ball balls[BALL_COUNT];
    Input input;
    Hud hud;
    Audio audio;
} Game;

int game_init(Game *g);
void game_run(Game *g);
void game_quit(Game *g);
void game_reset_balls(Game *g);

