//
//  hud.c
//  
//
//  Created by Abdulkhamid Khamidullin on 27.05.2026.
//

#include <stdio.h>
#include "hud.h"
#include "ball.h"

#define FONT_PATH "assets/font.ttf"
#define FONT_SIZE 20

int hud_init(Hud *hud) {
    if (TTF_Init() != 0) {
        printf("TTF_Init error: %s\n", TTF_GetError());
        return 0;
    }

    hud->font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!hud->font) {
        printf("Font error: %s\n", TTF_GetError());
        return 0;
    }

    hud->score = 0;
    return 1;
}

static void draw_text(Hud *hud, SDL_Renderer *r, const char *text,
                      int x, int y, SDL_Color color) {
    SDL_Surface *surf = TTF_RenderText_Blended(hud->font, text, color);
    if (!surf) return;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(r, surf);
    SDL_FreeSurface(surf);
    if (!tex) return;

    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_Rect dst = {x, y, w, h};
    SDL_RenderCopy(r, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

void hud_draw(Hud *hud, SDL_Renderer *r, int current_player) {
    char buf[64];
    
    // top panel background
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 140);
    SDL_Rect panel = {0, 0, WINDOW_WIDTH, 50};
    SDL_RenderFillRect(r, &panel);
    
    SDL_Color white  = {255, 255, 255, 255};
    SDL_Color yellow = {255, 220, 50,  255};
    SDL_Color p1     = {100, 180, 255, 255};
    SDL_Color p2     = {255, 120, 100, 255};

    // Score
    SDL_SetRenderDrawColor(r, 180, 140, 20, 200);
    SDL_Rect score_bg = {10, 8, 130, 34};
    SDL_RenderFillRect(r, &score_bg);
    
    snprintf(buf, sizeof(buf), "Score: %d", hud->score);
    draw_text(hud, r, buf, 18, 12, white);
    
    // help message
    SDL_SetRenderDrawColor(r, 0, 0, 0, 100);
    SDL_Rect hint_bg = {0, WINDOW_HEIGHT - 30, WINDOW_WIDTH, 30};
    SDL_RenderFillRect(r, &hint_bg);

    SDL_Color gray = {180, 180, 180, 255};
    draw_text(hud, r, "Hold LMB to aim | F - fullscreen |  R - restart", WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT - 24, gray);

    // player hud
    SDL_Color pc = (current_player == 1) ? p1 : p2;
    SDL_Rect player_bg = {WINDOW_WIDTH - 140, 8, 130, 34};
    SDL_SetRenderDrawColor(r, 30, 30, 80, 200);
    SDL_RenderFillRect(r, &player_bg);
    
    if (current_player == 1)
        draw_text(hud, r, "Player 1", WINDOW_WIDTH - 125, 12, p1);
    else
        draw_text(hud, r, "Player 2", WINDOW_WIDTH - 125, 12, p2);
}

void hud_add_score(Hud *hud, int points) {
    hud->score += points;
}

void hud_draw_game_over(Hud *hud, SDL_Renderer *r, int score) {
    // Screen dimming effect
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 160);
    SDL_Rect overlay = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(r, &overlay);

    char buf[64];
    SDL_Color yellow = {255, 220, 50,  255};
    SDL_Color white  = {255, 255, 255, 255};

    draw_text(hud, r, "YOU WIN!", WINDOW_WIDTH/2 - 80, WINDOW_HEIGHT/2 - 60, yellow);

    snprintf(buf, sizeof(buf), "Score: %d", score);
    draw_text(hud, r, buf, WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2, white);

    draw_text(hud, r, "Press R to restart", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 50, white);
}

void hud_quit(Hud *hud) {
    if (hud->font) TTF_CloseFont(hud->font);
    TTF_Quit();
}
