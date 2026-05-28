//
//  hud.h
//  
//
//  Created by Abdulkhamid Khamidullin on 27.05.2026.
//

#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct {
    TTF_Font *font;
    int score;
} Hud;

int  hud_init(Hud *hud);
void hud_draw(Hud *hud, SDL_Renderer *r);
void hud_add_score(Hud *hud, int points);
void hud_draw_game_over(Hud *hud, SDL_Renderer *r, int score);
void hud_quit(Hud *hud);
