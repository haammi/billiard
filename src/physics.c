//
//  physics.c
//  
//
//  Created by Abdulkhamid Khamidullin on 26.05.2026.
//

#include "physics.h"
#include <math.h>



void check_collisions(Ball *balls, int count) {
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            Ball *a = &balls[i];
            Ball *b = &balls[j];

            float dx   = b->x - a->x;
            float dy   = b->y - a->y;
            float dist = sqrtf(dx*dx + dy*dy);
            float min_dist = a->radius + b->radius;

            if (dist < min_dist && dist > 0.0f) {
                // Normal between balls
                float nx = dx / dist;
                float ny = dy / dist;

                // Separate the balls for not overlapping.
                float overlap = min_dist - dist;
                a->x -= nx * overlap * 0.5f;
                a->y -= ny * overlap * 0.5f;
                b->x += nx * overlap * 0.5f;
                b->y += ny * overlap * 0.5f;

                // Exchange of velocities along the normal (elastic impact)
                float da = a->vx * nx + a->vy * ny;
                float db = b->vx * nx + b->vy * ny;

                a->vx += (db - da) * nx;
                a->vy += (db - da) * ny;
                b->vx += (da - db) * nx;
                b->vy += (da - db) * ny;
            }
        }
    }
}
