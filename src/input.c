//
//  input.c
//
//
//  Created by Abdulkhamid Khamidullin on 26.05.2026.
//

#include "input.h"
#include <SDL.h>
#include <math.h>
#include <SDL_image.h>

#define MAX_POWER     1200.0f
#define POWER_SPEED   1.0f    // force accumulation velocity

void input_handle(Input *inp, Ball *cue_ball, SDL_Event *e) {
    if (e->type == SDL_MOUSEMOTION) {
        inp->mouse_x = (float)e->motion.x;
        inp->mouse_y = (float)e->motion.y;
        
        if (!inp->aiming) {
            // follow mouse for aiming direction
            inp->locked_x = inp->mouse_x;
            inp->locked_y = inp->mouse_y;
        } else {
            // Calculate power from pull distance
            float ax = cue_ball->x - inp->aim_x;
            float ay = cue_ball->y - inp->aim_y;
            float alen = sqrtf(ax * ax + ay * ay);
            
            if (alen > 0) {
                float mx = inp->mouse_x - inp->aim_x;
                float my = inp->mouse_y - inp->aim_y;
                float proj = (mx * ax + my * ay) / alen;
                
                // power increases when pulling back, resets when pushing forward
                if (proj > 0) {
                    inp->power = proj / 150.0f;
                    if (inp->power > 1.0f) inp->power = 1.0f;
                } else {
                    inp->power = 0.0f;
                }
            }
        }
    }
    
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
        inp->aiming = 1;
        inp->power  = 0.0f;
        inp->aim_x  = (float)e->button.x;
        inp->aim_y  = (float)e->button.y;
    }
    if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT) {
        if (inp->aiming && inp->power > 0.05f) {
            // Direction from mouse to ball
            float dx = inp->locked_x - cue_ball->x;
            float dy = inp->locked_y - cue_ball->y;
            float dist = sqrtf(dx * dx + dy * dy);
            if (dist > 0.0f) {
                cue_ball->vx = (dx / dist) * MAX_POWER * inp->power;
                cue_ball->vy = (dy / dist) * MAX_POWER * inp->power;
            }
        }
        inp->aiming = 0;
        inp->power  = 0.0f;
    }
}

void input_load_texture(Input *inp, SDL_Renderer *r) {
    SDL_Surface *surf = IMG_Load("assets/cue.png");
    if (!surf) {
        printf("cue.png error: %s\n", IMG_GetError());
        inp->cue_texture = NULL;
        return;
    }
    inp->cue_texture = SDL_CreateTextureFromSurface(r, surf);
    SDL_FreeSurface(surf);
}

void input_free_texture(Input *inp) {
    if (inp->cue_texture) SDL_DestroyTexture(inp->cue_texture);
}

void input_draw(Input *inp, Ball *cue_ball, SDL_Renderer *r, Ball *balls, int ball_count, int balls_moving) {
    if (balls_moving) return;
    // Aiming line
    float dx = inp->locked_x - cue_ball->x;
    float dy = inp->locked_y - cue_ball->y;
    float dist = sqrtf(dx * dx + dy * dy);
    
    if (dist < 1.0f) return;
    
    float nx = dx / dist;
    float ny = dy / dist;
    
    // searching ball on way
    float hit_x = -1, hit_y = -1;
    float min_t = 2000.0f;
    Ball *hit_ball = NULL;
    
    for (int i = 0; i < ball_count; i++) {
        Ball *b = &balls[i];
        if (!b->active || b->is_cue) continue;
        
        float bx = b->x - cue_ball->x;
        float by = b->y - cue_ball->y;
        float proj = bx * nx + by * ny;
        if (proj < 0) continue;
        
        float perp = (bx - proj * nx) * (bx - proj * nx) + (by - proj * ny) * (by - proj * ny);
        float min_dist = (cue_ball->radius + b->radius) * (cue_ball->radius + b->radius);
        
        if (perp <= min_dist && proj < min_t) {
            min_t  = proj - sqrtf(min_dist - perp);
            hit_x  = cue_ball->x + nx * min_t;
            hit_y  = cue_ball->y + ny * min_t;
            hit_ball = b;
        }
    }
    // dotted line
    float line_end = (hit_x >= 0) ? min_t : 2000.0f;
    for (float t = cue_ball->radius + 5; t < line_end; t += 14.0f) {
        int alpha = (int)(200.0f * (1.0f - t / 2000.0f));
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 255, 255, 200, alpha);
        int x1 = (int)(cue_ball->x + nx * t);
        int y1 = (int)(cue_ball->y + ny * t);
        int x2 = (int)(cue_ball->x + nx * (t + 7.0f));
        int y2 = (int)(cue_ball->y + ny * (t + 7.0f));
        SDL_RenderDrawLine(r, x1, y1, x2, y2);
        SDL_RenderDrawLine(r, x1+1, y1, x2+1, y2);
    }
    // ghost ball
    if (hit_x >= 0) {
        int grad = (int)cue_ball->radius;
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        
        SDL_SetRenderDrawColor(r, 255, 255, 255, 180);
        for (int w = -grad; w <= grad; w++)
            for (int h = -grad; h <= grad; h++) {
                int d = w * w + h * h;
                if (d <= grad * grad && d >= (grad-2) * (grad-2))
                    SDL_RenderDrawPoint(r, (int)hit_x + w, (int)hit_y + h);
            }
        
        SDL_SetRenderDrawColor(r, 255, 255, 255, 50);
        for (int w = -grad; w <= grad; w++)
            for (int h = -grad; h <= grad; h++)
                if (w * w + h * h <= grad * grad)
                    SDL_RenderDrawPoint(r, (int)hit_x + w, (int)hit_y + h);
        // the line where the ball will go after strike
        float dnx = nx, dny = ny;
        if (hit_ball) {
            dnx = hit_ball->x - hit_x;
            dny = hit_ball->y - hit_y;
            float dlen2 = sqrtf(dnx * dnx + dny * dny);
            if (dlen2 > 0) { dnx /= dlen2; dny /= dlen2; }
        }
        SDL_SetRenderDrawColor(r, 255, 200, 50, 150);
        for (float t = grad + 5; t < 200.0f; t += 12.0f) {
            SDL_RenderDrawLine(r,
                (int)(hit_x + dnx * t),
                (int)(hit_y + dny * t),
                (int)(hit_x + dnx * (t + 6.0f)),
                (int)(hit_y + dny * (t + 6.0f)));
        }
    }
    
    
    // cue
    float cue_start = cue_ball->radius + 10.0f + inp->power * 30.0f;
    float cue_len = 200.0f;
    
    if (inp->cue_texture) {
        // rotation angle
        float angle = atan2f(ny, nx) * 180.0f / 3.14159f;
        
        // cue size
        int draw_w = 400;
        int draw_h = 100;
        
        int cx = (int)(cue_ball->x - nx * cue_start);
        int cy = (int)(cue_ball->y - ny * cue_start);
        
        SDL_Rect dst = {cx - draw_w, cy - draw_h/2, draw_w, draw_h};
        SDL_Point pivot = {draw_w, draw_h/2};
        
        SDL_RenderCopyEx(r, inp->cue_texture, NULL, &dst, angle, &pivot, SDL_FLIP_NONE);
    } else {
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
    }
    if (inp->aiming) {
        // Power bar
        int bar_w = 200;
        int bar_h = 18;
        int bar_x = 20;
        int bar_y = 62;
        int fill_w = (int)(bar_w * inp->power);
        int red = (int)(255 * inp->power);
        int green = (int)(255 * (1.0f - inp->power));
        
        // background
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 0, 0, 0, 160);
        SDL_Rect bar_bg = {bar_x - 2, bar_y - 2, bar_w + 4, bar_h + 4};
        SDL_RenderFillRect(r, &bar_bg);
        
        // gradient
        SDL_SetRenderDrawColor(r, red, green, 0, 255);
        SDL_Rect bar = {bar_x, bar_y, fill_w, bar_h};
        SDL_RenderFillRect(r, &bar);
        
        // outline
        SDL_SetRenderDrawColor(r, 200, 200, 200, 200);
        SDL_RenderDrawRect(r, &bar_bg);
    }
}
