//
//  menu.c
//
//
//  Created by Abdulkhamid Khamidullin on 31.05.2026.
//

#include "menu.h"
#include "ball.h"

void menu_handle(Menu *m, SDL_Event *e, int *start, int *quit) {
    int mx = 0, my = 0;
    SDL_GetMouseState(&mx, &my);
    
    // buttons
    SDL_Rect play_btn = {WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 - 10, 240, 55};
    SDL_Rect exit_btn = {WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 80, 240, 55};
    
    m->hovered_play = (mx >= play_btn.x && mx <= play_btn.x + play_btn.w &&
                       my >= play_btn.y && my <= play_btn.y + play_btn.h);
    m->hovered_exit = (mx >= exit_btn.x && mx <= exit_btn.x + exit_btn.w &&
                       my >= exit_btn.y && my <= exit_btn.y + exit_btn.h);
    
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
        if (m->hovered_play) *start = 1;
        if (m->hovered_exit) *quit  = 1;
    }
}

static void draw_button(SDL_Renderer *r, SDL_Rect rect, int hovered) {
    // shadow
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 80);
    SDL_Rect shadow = {rect.x + 4, rect.y + 4, rect.w, rect.h};
    SDL_RenderFillRect(r, &shadow);
    
    // buttons background
    if (hovered)
        SDL_SetRenderDrawColor(r, 180, 140, 20, 255);
    else
        SDL_SetRenderDrawColor(r, 40, 25, 5, 220);
    SDL_RenderFillRect(r, &rect);
    
    // frame
    SDL_SetRenderDrawColor(r, 180, 140, 20, 255);
    SDL_RenderDrawRect(r, &rect);
}

static void draw_text_centered(TTF_Font *font, SDL_Renderer *r,
                               const char *text, int y, SDL_Color color) {
    SDL_Surface *surf = TTF_RenderText_Blended(font, text, color);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(r, surf);
    SDL_FreeSurface(surf);
    if (!tex) return;
    int tw, th;
    SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
    SDL_Rect dst = {WINDOW_WIDTH/2 - tw/2, y, tw, th};
    SDL_RenderCopy(r, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

void menu_draw(Menu *m, SDL_Renderer *r, TTF_Font *font, TTF_Font *font_large) {
    // menu background
    SDL_SetRenderDrawColor(r, 15, 40, 20, 255);
    SDL_Rect bg = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(r, &bg);
    
    // dark table on background
    SDL_SetRenderDrawColor(r, 20, 60, 30, 255);
    SDL_Rect table = {80, 100, WINDOW_WIDTH - 160, WINDOW_HEIGHT - 200};
    SDL_RenderFillRect(r, &table);
    
    SDL_SetRenderDrawColor(r, 80, 40, 10, 255);
    SDL_Rect border = {60, 80, WINDOW_WIDTH - 120, WINDOW_HEIGHT - 160};
    SDL_RenderDrawRect(r, &border);
    
    // darkness above
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 140);
    SDL_RenderFillRect(r, &bg);
    
    // title
    SDL_Color yellow = {255, 220, 50, 255};
    SDL_Color white  = {255, 255, 255, 255};
    SDL_Color gray   = {180, 180, 180, 255};
    
    draw_text_centered(font_large, r, "BILLIARD", WINDOW_HEIGHT/2 - 160, yellow);
    
    // line under title
    SDL_SetRenderDrawColor(r, 180, 140, 20, 255);
    SDL_RenderDrawLine(r, WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 - 95,
                       WINDOW_WIDTH/2 + 150, WINDOW_HEIGHT/2 - 95);
    
    // play button
    SDL_Rect play_btn = {WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 - 10, 240, 55};
    draw_button(r, play_btn, m->hovered_play);
    SDL_Color btn_color = m->hovered_play ? (SDL_Color){20, 10, 0, 255} : white;
    draw_text_centered(font, r, "PLAY", play_btn.y + 14, btn_color);
    
    // exit button
    SDL_Rect exit_btn = {WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 80, 240, 55};
    draw_button(r, exit_btn, m->hovered_exit);
    SDL_Color exit_color = m->hovered_exit ? (SDL_Color){20, 10, 0, 255} : gray;
    draw_text_centered(font, r, "EXIT", exit_btn.y + 14, exit_color);
}
