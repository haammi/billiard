//
//  game.c
//
//
//  Created by Abdulkhamid Khamidullin on 25.05.2026.
//

#include "game.h"
#include <stdio.h>
#include "physics.h"

int game_init(Game *g) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 0;
    }

    g->window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    SDL_SetWindowMinimumSize(g->window, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if (!g->window) {
        printf("Window error: %s\n", SDL_GetError());
        return 0;
    }

    g->renderer = SDL_CreateRenderer(
        g->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!g->renderer) {
        printf("Renderer error: %s\n", SDL_GetError());
        return 0;
    }

    g->running = 1;
   
    table_init(WINDOW_WIDTH, WINDOW_HEIGHT);
    game_reset_balls(g);
    
    return 1;
}

void game_run(Game *g) {
    SDL_Event e;
    Uint32 prev = SDL_GetTicks();

    while (g->running) {
        // Delta time
        Uint32 now = SDL_GetTicks();
        float dt = (now - prev) / 1000.0f;
        prev = now;
        // Event handling
        while (SDL_PollEvent(&e)) {
            input_handle(&g->input, &g->balls[0], &e);
            
            if (e.type == SDL_QUIT) g->running = 0;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                g->running = 0;
    
            // gonna fullscreen when you tapped F
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f) {
                g->fullscreen = !g->fullscreen;
                SDL_SetWindowFullscreen(
                    g->window,
                    g->fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0
                );
                int w, h;
                SDL_GetWindowSize(g->window, &w, &h);
                table_init(w, h);
                // change balls position
                game_reset_balls(g);
            }
            // gonna fullscreen when u tapped fullscreen button
            if (e.type == SDL_WINDOWEVENT &&
                e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                int w, h;
                SDL_GetWindowSize(g->window, &w, &h);
                table_init(w, h);
                game_reset_balls(g);
            }
        }
        
        input_update(&g->input);
        
        for (int i = 0; i < BALL_COUNT; i++) {
            ball_update(&g->balls[i], dt);
        }
        check_collisions(g->balls, BALL_COUNT);
        
        for (int i = 0; i < BALL_COUNT; i++) {
            if (!g->balls[i].active) continue;
            if (table_check_pockets(g->balls[i].x, g->balls[i].y)) {
                g->balls[i].active = 0;
                g->balls[i].vx = 0;
                g->balls[i].vy = 0;
            }
        }
        
        // Rendering
        SDL_SetRenderDrawColor(g->renderer, 30, 20, 10, 255);
        SDL_RenderClear(g->renderer);
        
        table_draw(g->renderer);
        
        for (int i = 0; i < BALL_COUNT; i++) {
            ball_draw(&g->balls[i], g->renderer);
        }

        input_draw(&g->input, &g->balls[0], g->renderer);
        
        SDL_RenderPresent(g->renderer);
    }
}

void game_quit(Game *g) {
    if (g->renderer) SDL_DestroyRenderer(g->renderer);
    if (g->window)   SDL_DestroyWindow(g->window);
    SDL_Quit();
}

void game_reset_balls(Game *g) {
    int colors[BALL_COUNT - 1][3] = {
        {255, 220, 50},
        {255, 80,  80},
        {80,  120, 255},
        {80,  220, 120},
    };

    g->balls[0].x = TABLE_X + 250.0f;
    g->balls[0].y = TABLE_Y + TABLE_H / 2.0f;
    g->balls[0].vx = 0.0f;
    g->balls[0].vy = 0.0f;
    g->balls[0].radius = 15.0f;
    g->balls[0].r = 255;
    g->balls[0].g = 255;
    g->balls[0].b = 255;
    g->balls[0].is_cue = 1;
    g->balls[0].active = 1;

    float start_x = TABLE_X + TABLE_W - 300.0f;
    float start_y = TABLE_Y + TABLE_H / 2.0f;
    float spacing = 31.0f;

    float positions[4][2] = {
        {start_x, start_y},
        {start_x + spacing, start_y - spacing * 0.5f},
        {start_x + spacing, start_y + spacing * 0.5f},
        {start_x + spacing*2, start_y},
    };

    for (int i = 0; i < BALL_COUNT - 1; i++) {
        g->balls[i + 1].x = positions[i][0];
        g->balls[i + 1].y = positions[i][1];
        g->balls[i + 1].vx = 0.0f;
        g->balls[i + 1].vy = 0.0f;
        g->balls[i + 1].radius = 15.0f;
        g->balls[i + 1].r = colors[i][0];
        g->balls[i + 1].g = colors[i][1];
        g->balls[i + 1].b = colors[i][2];
        g->balls[i + 1].is_cue = 0;
        g->balls[i + 1].active = 1;
    }
}
