//
//  input.h
//  
//
//  Created by Abdulkhamid Khamidullin on 26.05.2026.
//

#ifndef input_h
#define input_h

#pragma once
#include <SDL.h>
#include "ball.h"

typedef struct {
    float  mouse_x, mouse_y;
    int    aiming;       // aiming when mouse is pressed
    float  power;        // power of strike
} Input;

void input_handle(Input *inp, Ball *cue_ball, SDL_Event *e);
void input_draw(Input *inp, Ball *cue_ball, SDL_Renderer *r);
void input_update(Input *inp);

#endif /* input_h */
