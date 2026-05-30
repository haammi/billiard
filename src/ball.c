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
void ball_draw(Ball *b, void *renderer, TTF_Font *font) {
    if (!b->active) return;
    SDL_Renderer *r = (SDL_Renderer *)renderer;

    int cx = (int)b->x;
    int cy = (int)b->y;
    int rad = (int)b->radius;
    
    // shadow
    SDL_SetRenderDrawColor(r, 0, 0, 0, 80);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    for (int w = -rad; w <= rad; w++)
        for (int h = -rad; h <= rad; h++)
            if (w*w + h*h <= rad*rad)
                SDL_RenderDrawPoint(r, cx + w + 4, cy + h + 4);
    
    // ball
    if (b->number >= 9 && b->number <= 15) {
    // white base
    SDL_SetRenderDrawColor(r, 240, 240, 240, 255);
    for (int w = -rad; w <= rad; w++)
        for (int h = -rad; h <= rad; h++)
            if (w * w + h * h <= rad * rad)
                SDL_RenderDrawPoint(r, cx + w, cy + h);
        // color stripe
            SDL_SetRenderDrawColor(r, b->r, b->g, b->b, 255);
            int stripe = rad / 2;
            for (int w = -rad; w <= rad; w++)
                for (int h = -stripe; h <= stripe; h++)
                    if (w * w + h * h <= rad * rad)
                        SDL_RenderDrawPoint(r, cx + w, cy + h);
        } else {
            // solid ball
            SDL_SetRenderDrawColor(r, b->r, b->g, b->b, 255);
            for (int w = -rad; w <= rad; w++)
                for (int h = -rad; h <= rad; h++)
                    if (w * w + h * h <= rad  *rad)
                        SDL_RenderDrawPoint(r, cx + w, cy + h);
        }

   
    
    // glare on the balls
    SDL_SetRenderDrawColor(r, 255, 255, 255, 100);
    int highlight_r = rad / 3;
    for (int w = -highlight_r; w <= highlight_r; w++)
        for (int h = -highlight_r; h <= highlight_r; h++)
            if (w * w + h * h <= highlight_r * highlight_r)
                SDL_RenderDrawPoint(r, cx + w - rad/3, cy + h - rad/3);
    
    // num on ball
    if (b->number > 0 && font) {
        char buf[4];
        snprintf(buf, sizeof(buf), "%d", b->number);

        SDL_Color text_color = {255, 255, 255, 255};
        // dark num on light ball
        if (b->r > 180 && b->g > 180)
            text_color = (SDL_Color){30, 30, 30, 255};

        SDL_Surface *surf = TTF_RenderText_Blended(font, buf, text_color);
        if (surf) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(r, surf);
            SDL_FreeSurface(surf);
            if (tex) {
                int tw, th;
                SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
                SDL_Rect dst = {cx - tw/2, cy - th/2, tw, th};
                SDL_RenderCopy(r, tex, NULL, &dst);
                SDL_DestroyTexture(tex);
            }
        }
    }
}
