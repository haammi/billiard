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
    // ball init
    int colors[BALL_COUNT][3] = {
        {255, 220, 50},
        {255, 80,  80},
        {80,  120, 255},
        {80,  220, 120},
        {220, 80,  220},
    };
    // Triangle as in billiards
    float start_x = 800.0f;
    float start_y = WINDOW_HEIGHT / 2.0f;
    float spacing = 31.0f;
    
    
    float positions[5][2] = {
        {start_x,              start_y},
        {start_x + spacing,    start_y - spacing * 0.5f},
        {start_x + spacing,    start_y + spacing * 0.5f},
        {start_x + spacing*2,  start_y - spacing},
        {start_x + spacing*2,  start_y},
    };

    for (int i = 0; i < BALL_COUNT; i++) {
        g->balls[i].x      = positions[i][0];
        g->balls[i].y      = positions[i][1];
        g->balls[i].vx     = 0.0f;
        g->balls[i].vy     = 0.0f;
        g->balls[i].radius = 15.0f;
        g->balls[i].r      = colors[i][0];
        g->balls[i].g      = colors[i][1];
        g->balls[i].b      = colors[i][2];
    }
    
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
            if (e.type == SDL_QUIT) g->running = 0;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                g->running = 0;
        }
        for (int i = 0; i < BALL_COUNT; i++) {
            ball_update(&g->balls[i], dt);
        }
        check_collisions(g->balls, BALL_COUNT);
        
        // Rendering
        SDL_SetRenderDrawColor(g->renderer, 20, 80, 40, 255);
        SDL_RenderClear(g->renderer);
        
        for (int i = 0; i < BALL_COUNT; i++) {
            ball_draw(&g->balls[i], g->renderer);
        }

        SDL_RenderPresent(g->renderer);
    }
}

void game_quit(Game *g) {
    if (g->renderer) SDL_DestroyRenderer(g->renderer);
    if (g->window)   SDL_DestroyWindow(g->window);
    SDL_Quit();
}
