//
//  menu.h
//  
//
//  Created by Abdulkhamid Khamidullin on 31.05.2026.
//

#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct {
    int hovered_play;
    int hovered_exit;
} Menu;

void menu_draw(Menu *m, SDL_Renderer *r, TTF_Font *font, TTF_Font *font_large);
void menu_handle(Menu *m, SDL_Event *e, int *start, int *quit);
