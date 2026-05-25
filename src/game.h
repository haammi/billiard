#pragma once

#include <SDL.h>

#define WINDOW_TITLE  "Billiard"
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    int           running;
} Game;

int  game_init(Game *g);
void game_run(Game *g);
void game_quit(Game *g);
