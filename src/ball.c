//
//  ball.c
//  
//
//  Created by Abdulkhamid Khamidullin on 25.05.2026.
//

#include "ball.h"
#include <SDL.h>

void ball_update(Ball *b, float dt) {
    b->x += b->vx * dt;
    b->y += b->vy * dt;

    // Bouncing off walls
    if (b->x - b->radius < 0) {
        b->x = b->radius;
        b->vx = -b->vx;
    }
    if (b->x + b->radius > WINDOW_WIDTH) {
        b->x = WINDOW_WIDTH - b->radius;
        b->vx = -b->vx;
    }
    if (b->y - b->radius < 0) {
        b->y = b->radius;
        b->vy = -b->vy;
    }
    if (b->y + b->radius > WINDOW_HEIGHT) {
        b->y = WINDOW_HEIGHT - b->radius;
        b->vy = -b->vy;
    }
}

// Drawing cirle through the dots
void ball_draw(Ball *b, void *renderer) {
    SDL_Renderer *r = (SDL_Renderer *)renderer;
    SDL_SetRenderDrawColor(r, b->r, b->g, b->b, 255);

    int cx = (int)b->x;
    int cy = (int)b->y;
    int rad = (int)b->radius;

    for (int w = -rad; w <= rad; w++) {
        for (int h = -rad; h <= rad; h++) {
            if (w*w + h*h <= rad*rad) {
                SDL_RenderDrawPoint(r, cx + w, cy + h);
            }
        }
    }
}
