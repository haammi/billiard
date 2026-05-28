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
#define FONT_SIZE 28

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
    SDL_Color white  = {255, 255, 255, 255};
    SDL_Color yellow = {255, 220, 50,  255};
    SDL_Color p1     = {100, 180, 255, 255};
    SDL_Color p2     = {255, 120, 100, 255};

    // Score
    snprintf(buf, sizeof(buf), "Score: %d", hud->score);
    draw_text(hud, r, buf, 20, 8, yellow);

    draw_text(hud, r, "Hold LMB to aim | F - fullscreen", 20, 36, white);
    
    if (current_player == 1)
        draw_text(hud, r, "Player 1", WINDOW_WIDTH - 150, 8, p1);
    else
        draw_text(hud, r, "Player 2", WINDOW_WIDTH - 150, 8, p2);
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
