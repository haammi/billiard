//
//  table.h
//  
//
//  Created by Abdulkhamid Khamidullin on 26.05.2026.
//

#pragma once

#include <SDL.h>

extern int TABLE_X;
extern int TABLE_Y;
extern int TABLE_W;
extern int TABLE_H;

#define POCKET_RADIUS 18

void table_init(int window_w, int window_h);
void table_draw(SDL_Renderer *r);
int table_check_pockets(float x, float y);


