//
//  ball.c
//  
//
//  Created by Abdulkhamid Khamidullin on 25.05.2026.
//

#include "ball.h"
#include <SDL.h>

void ball_update(Ball *b, float dt, Audio *audio) {
    if (!b->active) return;
    
    // friction
    b->vx *= FRICTION;
    b->vy *= FRICTION;

    // stops if it's almost stopped.
    if (b->vx * b->vx + b->vy * b->vy < 0.5f) {
        b->vx = 0;
        b->vy = 0;
    }
    
    b->x += b->vx * dt;
    b->y += b->vy * dt;

    // Bouncing off walls
    if (b->x - b->radius < TABLE_X &&
        !table_check_pockets(b->x, b->y)) {
        b->x = TABLE_X + b->radius;
        b->vx = -b->vx;
        audio_play_wall(audio);
    }
    if (b->x + b->radius > TABLE_X + TABLE_W &&
        !table_check_pockets(b->x, b->y)) {
        b->x = TABLE_X + TABLE_W - b->radius;
        b->vx = -b->vx;
        audio_play_wall(audio);
    }
    if (b->y - b->radius < TABLE_Y &&
        !table_check_pockets(b->x, b->y)) {
        b->y = TABLE_Y + b->radius;
        b->vy = -b->vy;
        audio_play_wall(audio);
    }
    if (b->y + b->radius > TABLE_Y + TABLE_H &&
        !table_check_pockets(b->x, b->y)) {
        b->y = TABLE_Y + TABLE_H - b->radius;
        b->vy = -b->vy;
        audio_play_wall(audio);
    }
}

// Drawing cirle through the dots
void ball_draw(Ball *b, void *renderer) {
    if (!b->active) return;
    SDL_Renderer *r = (SDL_Renderer *)renderer;
    SDL_SetRenderDrawColor(r, b->r, b->g, b->b, 255);

    int cx = (int)b->x;
    int cy = (int)b->y;
    int rad = (int)b->radius;

    for (int w = -rad; w <= rad; w++) {
        for (int h = -rad; h <= rad; h++) {
            if (w * w + h * h <= rad * rad) {
                SDL_RenderDrawPoint(r, cx + w, cy + h);
            }
        }
    }
}
