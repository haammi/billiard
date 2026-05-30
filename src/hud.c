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
#define FONT_SIZE 18

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
    
    hud->font_large = TTF_OpenFont(FONT_PATH, 48);
    if (!hud->font_large) {
        printf("Font large error: %s\n", TTF_GetError());
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

static void draw_text_centered(Hud *hud, SDL_Renderer *r, const char *text,
                                int y, SDL_Color color) {
    SDL_Surface *surf = TTF_RenderText_Blended(hud->font, text, color);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(r, surf);
    SDL_FreeSurface(surf);
    if (!tex) return;
    int tw, th;
    SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
    SDL_Rect dst = {hud->screen_w/2 - tw/2, y, tw, th};
    SDL_RenderCopy(r, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

void hud_draw(Hud *hud, SDL_Renderer *r, int current_player, int score1, int score2) {
    char buf[64];
    
    // top panel background
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 140);
    SDL_Rect panel = {0, 0, hud->screen_w, 50};
    SDL_RenderFillRect(r, &panel);
    
    SDL_Color white  = {255, 255, 255, 255};
    SDL_Color yellow = {255, 220, 50,  255};
    SDL_Color p1 = {100, 180, 255, 255};
    SDL_Color p2 = {255, 120, 100, 255};
    SDL_Color gray = {180, 180, 180, 255};
    
    // player 1
    SDL_Rect p1_bg = {10, 8, 160, 34};
    SDL_SetRenderDrawColor(r, 20, 40, 80, 200);
    SDL_RenderFillRect(r, &p1_bg);
    if (current_player == 1) {
        SDL_SetRenderDrawColor(r, 100, 180, 255, 255);
        SDL_RenderDrawRect(r, &p1_bg);
    }
    snprintf(buf, sizeof(buf), "P1: %d", score1);
    draw_text(hud, r, buf, 20, 14, p1);
    
    // player 2
    SDL_Rect p2_bg = {hud->screen_w - 170, 8, 160, 34};
    SDL_SetRenderDrawColor(r, 80, 20, 20, 200);
    SDL_RenderFillRect(r, &p2_bg);
    if (current_player == 2) {
        SDL_SetRenderDrawColor(r, 255, 120, 100, 255);
        SDL_RenderDrawRect(r, &p2_bg);
    }
    snprintf(buf, sizeof(buf), "P2: %d", score2);
    draw_text(hud, r, buf, hud->screen_w - 155, 14, p2);

    // help message
    SDL_SetRenderDrawColor(r, 0, 0, 0, 100);
    SDL_Rect hint_bg = {0, hud->screen_h - 30, hud->screen_w, 30};
    SDL_RenderFillRect(r, &hint_bg);
    draw_text(hud, r, "Hold LMB and pull back to shoot | F - fullscreen |  R - restart", hud->screen_w/2 - 200, hud->screen_h - 24, gray);
}

void hud_add_score(Hud *hud, int points) {
    hud->score += points;
}

void hud_draw_game_over(Hud *hud, SDL_Renderer *r, int score1, int score2, int winner) {
    
    char buf[64];
    SDL_Color yellow = {255, 220, 50,  255};
    SDL_Color white  = {255, 255, 255, 255};
    SDL_Color gray   = {180, 180, 180, 255};
    
    // Screen dimming effect
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, hud->screen_w, hud->screen_h};
    SDL_RenderFillRect(r, &overlay);
    
    //frame
    SDL_SetRenderDrawColor(r, 40, 25, 5, 230);
    SDL_Rect box = {hud->screen_w/2 - 220, hud->screen_h/2 - 120, 440, 240};
    SDL_RenderFillRect(r, &box);
    
    // golden frame
    SDL_SetRenderDrawColor(r, 180, 140, 20, 255);
    SDL_RenderDrawRect(r, &box);
    
    // inner lining
    SDL_Rect box2 = {box.x + 3, box.y + 3, box.w - 6, box.h - 6};
    SDL_SetRenderDrawColor(r, 100, 80, 10, 255);
    SDL_RenderDrawRect(r, &box2);
    
    const char *winner_text;
    if (winner == 1)
        winner_text = "Player 1 Wins!";
    else if (winner == 2)
        winner_text = "Player 2 Wins!";
    else
        winner_text = "Draw!";
    
    // YOU WIN! in large font
    SDL_Surface *surf = TTF_RenderText_Blended(hud->font_large, winner_text, yellow);
    if (surf) {
        SDL_Texture *tex = SDL_CreateTextureFromSurface(r, surf);
        SDL_FreeSurface(surf);
        if (tex) {
            int tw, th;
            SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
            SDL_Rect dst = {hud->screen_w/2 - tw/2, hud->screen_h/2 - 90, tw, th};
            SDL_RenderCopy(r, tex, NULL, &dst);
            SDL_DestroyTexture(tex);
        }
    }

    snprintf(buf, sizeof(buf), "P1: %d  |  P2: %d", score1, score2);
    draw_text_centered(hud, r, buf, hud->screen_h/2 - 10, white);

    draw_text_centered(hud, r, "Press R to restart", hud->screen_h/2 + 50, gray);
}

void hud_quit(Hud *hud) {
    if (hud->font) TTF_CloseFont(hud->font);
    if (hud->font_large) TTF_CloseFont(hud->font_large);
    TTF_Quit();
}
