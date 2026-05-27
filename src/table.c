//
//  table.c
//  
//
//  Created by Abdulkhamid Khamidullin on 26.05.2026.
//

#include "table.h"
#include <math.h>

int TABLE_X = 0;
int TABLE_Y = 0;
int TABLE_W = 0;
int TABLE_H = 0;

void table_init(int window_w, int window_h) {
    TABLE_X = window_w * 0.06f;
    TABLE_Y = window_h * 0.11f;
    TABLE_W = window_w * 0.88f;
    TABLE_H = window_h * 0.78f;
}

static void draw_circle(SDL_Renderer *r, int cx, int cy, int rad) {
    for (int w = -rad; w <= rad; w++)
        for (int h = -rad; h <= rad; h++)
            if (w * w + h * h <= rad * rad)
                SDL_RenderDrawPoint(r, cx + w, cy + h);
}

void table_draw(SDL_Renderer *r) {
    // Sides
    SDL_Rect border = {TABLE_X - 20, TABLE_Y - 20, TABLE_W + 40, TABLE_H + 40};
    SDL_SetRenderDrawColor(r, 80, 40, 10, 255);
    SDL_RenderFillRect(r, &border);

    // Green field
    SDL_Rect felt = {TABLE_X, TABLE_Y, TABLE_W, TABLE_H};
    SDL_SetRenderDrawColor(r, 20, 100, 50, 255);
    SDL_RenderFillRect(r, &felt);

    // 6 pocket
    int px[6] = {
        TABLE_X, TABLE_X + TABLE_W / 2, TABLE_X + TABLE_W,
        TABLE_X, TABLE_X + TABLE_W / 2, TABLE_X + TABLE_W
    };
    int py[6] = {
        TABLE_Y, TABLE_Y, TABLE_Y,
        TABLE_Y + TABLE_H, TABLE_Y + TABLE_H, TABLE_Y + TABLE_H
    };

    SDL_SetRenderDrawColor(r, 10, 10, 10, 255);
    for (int i = 0; i < 6; i++)
        draw_circle(r, px[i], py[i], POCKET_RADIUS);
}

int table_check_pockets(float x, float y) {
    int px[6] = {
        TABLE_X, TABLE_X + TABLE_W / 2, TABLE_X + TABLE_W,
        TABLE_X, TABLE_X + TABLE_W / 2, TABLE_X + TABLE_W
    };
    int py[6] = {
        TABLE_Y, TABLE_Y, TABLE_Y,
        TABLE_Y + TABLE_H, TABLE_Y + TABLE_H, TABLE_Y + TABLE_H
    };

    for (int i = 0; i < 6; i++) {
        float dx = x - px[i];
        float dy = y - py[i];
        if (dx * dx + dy * dy <= (float)(POCKET_RADIUS * POCKET_RADIUS))
            return 1;
    }
    return 0;
}
