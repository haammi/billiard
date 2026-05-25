#include "game.h"
#include <stdio.h>

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
    g->ball.x = WINDOW_WIDTH  / 2.0f;
    g->ball.y = WINDOW_HEIGHT / 2.0f;
    g->ball.vx = 200.0f;
    g->ball.vy = 150.0f;
    g->ball.radius = 15.0f;
    g->ball.r = 255;
    g->ball.g = 220;
    g->ball.b = 50;
    
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
        ball_update(&g->ball, dt);
        
        // Rendering
        SDL_SetRenderDrawColor(g->renderer, 20, 80, 40, 255);
        SDL_RenderClear(g->renderer);
        
        ball_draw(&g->ball, g->renderer);

        SDL_RenderPresent(g->renderer);
    }
}

void game_quit(Game *g) {
    if (g->renderer) SDL_DestroyRenderer(g->renderer);
    if (g->window)   SDL_DestroyWindow(g->window);
    SDL_Quit();
}
