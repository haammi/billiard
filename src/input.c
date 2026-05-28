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
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < 1.0f) return;
    
    float nx = dx / dist;
    float ny = dy / dist;

    // dotted line
    SDL_SetRenderDrawColor(r, 255, 255, 255, 150);
    for (float t = cue_ball->radius + 2; t < 300.0f; t += 12.0f) {
        int x1 = (int)(cue_ball->x + nx * t);
        int y1 = (int)(cue_ball->y + ny * t);
        int x2 = (int)(cue_ball->x + nx * (t + 6.0f));
        int y2 = (int)(cue_ball->y + ny * (t + 6.0f));
        SDL_RenderDrawLine(r, x1, y1, x2, y2);
    }
    
    // cue
    float cue_start = cue_ball->radius + 10.0f + inp->power * 30.0f;
    float cue_len   = 200.0f;
    
    SDL_SetRenderDrawColor(r, 180, 120, 40, 255);
    for (int offset = -3; offset <= 3; offset++) {
        int ox = (int)(ny * offset);
        int oy = (int)(nx * offset);
        
        SDL_RenderDrawLine(r,
            (int)(cue_ball->x - nx * cue_start) + ox,
            (int)(cue_ball->y - ny * cue_start) + oy,
            (int)(cue_ball->x - nx * (cue_start + cue_len)) + ox,
            (int)(cue_ball->y - ny * (cue_start + cue_len)) + oy);
    }

    // Power bar
    SDL_Rect bar_bg = {20, 62, 200, 16};
    SDL_SetRenderDrawColor(r, 40, 40, 40, 255);
    SDL_RenderFillRect(r, &bar_bg);

    SDL_Rect bar = {20, 62, (int)(200 * inp->power), 16};
    SDL_SetRenderDrawColor(r, 255, 100, 50, 255);
    SDL_RenderFillRect(r, &bar);
    
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
}
