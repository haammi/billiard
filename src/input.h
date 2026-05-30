//
//  input.h
//  
//
//  Created by Abdulkhamid Khamidullin on 26.05.2026.
//

#pragma once
#include <SDL.h>
#include "ball.h"

typedef struct {
    float  mouse_x, mouse_y;
    int    aiming;       // aiming when mouse is pressed
    float  power;        // power of strike
    float aim_x, aim_y;
    float locked_x, locked_y;
} Input;

void input_handle(Input *inp, Ball *cue_ball, SDL_Event *e);
void input_draw(Input *inp, Ball *cue_ball, SDL_Renderer *r, Ball *balls, int ball_count, int balls_moving);



