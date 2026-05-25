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
    return 1;
}

void game_run(Game *g) {
    SDL_Event e;

    while (g->running) {
        // Event handling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) g->running = 0;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                g->running = 0;
        }

        // Table rendering
        SDL_SetRenderDrawColor(g->renderer, 20, 80, 40, 255);
        SDL_RenderClear(g->renderer);

        SDL_RenderPresent(g->renderer);
    }
}

void game_quit(Game *g) {
    if (g->renderer) SDL_DestroyRenderer(g->renderer);
    if (g->window)   SDL_DestroyWindow(g->window);
    SDL_Quit();
}
