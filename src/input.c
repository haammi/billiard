//
//  input.c
//  
//
//  Created by Abdulkhamid Khamidullin on 26.05.2026.
//

#include "input.h"
#include <SDL.h>
#include <math.h>

#define MAX_POWER     1200.0f
#define POWER_SPEED   1.0f    // force accumulation velocity

void input_handle(Input *inp, Ball *cue_ball, SDL_Event *e) {
    if (e->type == SDL_MOUSEMOTION) {
        inp->mouse_x = (float)e->motion.x;
        inp->mouse_y = (float)e->motion.y;
    }

    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
        inp->aiming = 1;
        inp->power  = 0.0f;
    }

    if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT) {
        if (inp->aiming) {
            // Direction from mouse to ball
            float dx = cue_ball->x - inp->mouse_x;
            float dy = cue_ball->y - inp->mouse_y;
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist > 0.0f) {
                cue_ball->vx = (dx / dist) * MAX_POWER * inp->power;
                cue_ball->vy = (dy / dist) * MAX_POWER * inp->power;
            }
        }
        inp->aiming = 0;
        inp->power  = 0.0f;
    }
}

void input_update(Input *inp) {
    if (inp->aiming) {
        inp->power += POWER_SPEED * 0.016f;  // ~60fps
        if (inp->power > 1.0f) inp->power = 1.0f;
    }
}

void input_draw(Input *inp, Ball *cue_ball, SDL_Renderer *r) {
    if (!inp->aiming) return;

    // Aiming line
    float dx = cue_ball->x - inp->mouse_x;
    float dy = cue_ball->y - inp->mouse_y;
    float dist = sqrtf(dx*dx + dy*dy);

    if (dist > 0.0f) {
        float nx = dx / dist;
        float ny = dy / dist;

        SDL_SetRenderDrawColor(r, 255, 255, 255, 180);
        SDL_RenderDrawLine(r,
            (int)cue_ball->x,
            (int)cue_ball->y,
            (int)(cue_ball->x + nx * 200.0f),
            (int)(cue_ball->y + ny * 200.0f)
        );
    }

    // Power bar
    SDL_Rect bar_bg = {20, 20, 200, 20};
    SDL_SetRenderDrawColor(r, 60, 60, 60, 255);
    SDL_RenderFillRect(r, &bar_bg);

    SDL_Rect bar = {20, 20, (int)(200 * inp->power), 20};
    SDL_SetRenderDrawColor(r, 255, 100, 50, 255);
    SDL_RenderFillRect(r, &bar);
}
