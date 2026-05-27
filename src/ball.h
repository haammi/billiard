//
//  ball.h
//  
//
//  Created by Abdulkhamid Khamidullin on 25.05.2026.
//

#ifndef ball_h
#define ball_h

#pragma once

#include "table.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

#define BALL_COUNT    5
#define FRICTION      0.98f   // speed multiplier every frame

typedef struct {
    float x, y;   // position
    float vx, vy; // velocity
    float radius;
    int r, g, b;  // color
    int is_cue;
    int active;
} Ball;

void ball_update(Ball *b, float dt);
void ball_draw(Ball *b, void *renderer);

#endif /* ball_h */
