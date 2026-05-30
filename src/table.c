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
static void draw_ring(SDL_Renderer *r, int cx, int cy, int rad, int thickness) {
    for (int w = -rad; w <= rad; w++)
        for (int h = -rad; h <= rad; h++) {
            int d = w * w + h * h;
            if (d <= rad * rad && d >= (rad-thickness)*(rad-thickness))
                SDL_RenderDrawPoint(r, cx + w, cy + h);
        }
}

void table_draw(SDL_Renderer *r) {
    // Sides
    SDL_Rect border = {TABLE_X - 20, TABLE_Y - 20, TABLE_W + 40, TABLE_H + 40};
    SDL_SetRenderDrawColor(r, 50, 25, 5, 255);
    SDL_RenderFillRect(r, &border);
    
    SDL_Rect inner = {TABLE_X - 18, TABLE_Y - 18, TABLE_W + 36, TABLE_H + 36};
    SDL_SetRenderDrawColor(r, 100, 55, 15, 255);
    SDL_RenderFillRect(r, &inner);
    
    // Green field
    SDL_Rect felt = {TABLE_X, TABLE_Y, TABLE_W, TABLE_H};
    SDL_SetRenderDrawColor(r, 25, 110, 55, 255);
    SDL_RenderFillRect(r, &felt);
    
    // central line
    SDL_SetRenderDrawColor(r, 20, 90, 45, 255);
    for (int y = TABLE_Y + 10; y < TABLE_Y + TABLE_H; y += 20)
        SDL_RenderDrawLine(r, TABLE_X + TABLE_W/2, y,
                           TABLE_X + TABLE_W/2, y + 10);
    
    // position points
    draw_circle(r, TABLE_X + (int)(TABLE_W * 0.25f), TABLE_Y + TABLE_H/2, 4);
    draw_circle(r, TABLE_X + (int)(TABLE_W * 0.75f), TABLE_Y + TABLE_H/2, 4);
    
    // 6 pocket
    int px[6] = {
        TABLE_X + 10, TABLE_X + TABLE_W / 2, TABLE_X + TABLE_W - 10,
        TABLE_X + 10, TABLE_X + TABLE_W / 2, TABLE_X + TABLE_W - 10
    };
    int py[6] = {
        TABLE_Y + 10, TABLE_Y, TABLE_Y + 10,
        TABLE_Y + TABLE_H - 10, TABLE_Y + TABLE_H, TABLE_Y + TABLE_H - 10
    };
    
    for (int i = 0; i < 6; i++){
        // pockets shadow
        SDL_SetRenderDrawColor(r, 0, 0, 0, 100);
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        draw_circle(r, px[i] + 3, py[i] + 3, POCKET_RADIUS + 2);
        
        // black hole effect
        SDL_SetRenderDrawColor(r, 5, 5, 5, 255);
        draw_circle(r, px[i], py[i], POCKET_RADIUS);
        
        // brown ring
        SDL_SetRenderDrawColor(r, 80, 35, 5, 255);
        draw_ring(r, px[i], py[i], POCKET_RADIUS + 5, 5);
        
        // light highlight on the rim
        SDL_SetRenderDrawColor(r, 140, 70, 20, 255);
        draw_ring(r, px[i], py[i], POCKET_RADIUS + 6, 2);
    }
}

int table_check_pockets(float x, float y) {
    int px[6] = {
        TABLE_X + 10, TABLE_X + TABLE_W / 2, TABLE_X + TABLE_W - 10,
        TABLE_X + 10, TABLE_X + TABLE_W / 2, TABLE_X + TABLE_W - 10
    };
    int py[6] = {
        TABLE_Y + 10, TABLE_Y, TABLE_Y + 10,
        TABLE_Y + TABLE_H - 10, TABLE_Y + TABLE_H, TABLE_Y + TABLE_H - 10
    };
    
    for (int i = 0; i < 6; i++) {
        float dx = x - px[i];
        float dy = y - py[i];
        if (dx * dx + dy * dy <= (float)(POCKET_RADIUS * POCKET_RADIUS))
            return 1;
    }
    return 0;
}
