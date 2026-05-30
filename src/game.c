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
    g->game_over  = 0;
    g->current_player = 1;
    g->balls_moving   = 0;
    g->score1 = 0;
    g->score2 = 0;
    g->scored_this_turn = 0;
    g->winner = 0;
    g->hud.screen_w = WINDOW_WIDTH;
    g->hud.screen_h = WINDOW_HEIGHT;
    
    
    table_init(WINDOW_WIDTH, WINDOW_HEIGHT);
    audio_init(&g->audio);
    game_reset_balls(g);
    input_load_texture(&g->input, g->renderer);
    if (!hud_init(&g->hud)) return 0;
    
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
            if (!g->game_over && !g->balls_moving)
                input_handle(&g->input, &g->balls[0], &e);
            
            //restart
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) {
                g->game_over = 0;
                g->hud.score = 0;
                g->score1 = 0;
                g->score2 = 0;
                g->current_player = 1;
                g->scored_this_turn = 0;
                g->winner = 0;
                game_reset_balls(g);
            }
            
            if (e.type == SDL_QUIT) g->running = 0;
            
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                g->running = 0;
            
    
            // gonna fullscreen when you tapped F
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f) {
                g->fullscreen = !g->fullscreen;
                SDL_SetWindowFullscreen(g->window, g->fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }
            // gonna fullscreen when u tapped fullscreen button
            if (e.type == SDL_WINDOWEVENT &&
                e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                int w, h;
                SDL_GetWindowSize(g->window, &w, &h);
                // save old position values
                float old_x = TABLE_X;
                float old_y = TABLE_Y;
                float old_w = TABLE_W;
                float old_h = TABLE_H;
                
                // save relative positions
                float rx[BALL_COUNT], ry[BALL_COUNT];
                for (int i = 0; i < BALL_COUNT; i++) {
                    rx[i] = (g->balls[i].x - old_x) / old_w;
                    ry[i] = (g->balls[i].y - old_y) / old_h;
                }
                
                
                table_init(w, h);
                g->hud.screen_w = w;
                g->hud.screen_h = h;
                
                // restoring positions
                for (int i = 0; i < BALL_COUNT; i++) {
                    g->balls[i].x = TABLE_X + rx[i] * TABLE_W;
                    g->balls[i].y = TABLE_Y + ry[i] * TABLE_H;
                    
                    
                }
            }
        }
        if (!g->game_over) {
            if (g->balls[0].vx * g->balls[0].vx +
                g->balls[0].vy * g->balls[0].vy > 0.1f) {
                g->balls_moving = 1;
            }
        
            for (int i = 0; i < BALL_COUNT; i++) {
                ball_update(&g->balls[i], dt, &g->audio);
            }
            
            check_collisions(g->balls, BALL_COUNT, &g->audio);
            
            // check balls movement
            int any_moving = 0;
            for (int i = 0; i < BALL_COUNT; i++) {
                if (!g->balls[i].active) continue;
                if (g->balls[i].vx * g->balls[i].vx +
                    g->balls[i].vy * g->balls[i].vy > 0.1f) {
                    any_moving = 1;
                    break;
                }
            }
            
            // change of direction when the balls stop
            if (g->balls_moving && !any_moving) {
                g->balls_moving = 0;
                if (g->scored_this_turn) {
                    // scored - the current player retains the turn
                    g->scored_this_turn = 0;
                } else {
                    g->current_player = (g->current_player == 1) ? 2 : 1;
                }
            }
            if (any_moving) g->balls_moving = 1;
            
            for (int i = 0; i < BALL_COUNT; i++) {
                if (!g->balls[i].active) continue;
                if (table_check_pockets(g->balls[i].x, g->balls[i].y)) {
                    if (g->balls[i].is_cue) {
                        g->balls[i].x  = TABLE_X + 250.0f;
                        g->balls[i].y  = TABLE_Y + TABLE_H / 2.0f;
                        g->balls[i].vx = 0.0f;
                        g->balls[i].vy = 0.0f;
                    } else {
                        g->balls[i].active = 0;
                        g->balls[i].vx     = 0.0f;
                        g->balls[i].vy     = 0.0f;
                        audio_play_pocket(&g->audio);
                        
                        // point to the current player
                        if (g->current_player == 1)
                            g->score1++;
                        else
                            g->score2++;
                        g->scored_this_turn = 1;
                        
                        // black ball - the current player loses
                        if (g->balls[i].number == 8) {
                            g->game_over = 1;
                            g->winner = (g->current_player == 1) ? 2 : 1;
                        }
                        
                        hud_add_score(&g->hud, 1);
                        
                    }
                }
            }
            
            // check end of the game
            int active_count = 0;
            for (int i = 1; i < BALL_COUNT; i++) {
                if (g->balls[i].active) active_count++;
            }
            if (active_count == 0) g->game_over = 1;
        }
        
        // Rendering
        SDL_SetRenderDrawColor(g->renderer, 30, 20, 10, 255);
        SDL_RenderClear(g->renderer);
        
        table_draw(g->renderer);
        
        for (int i = 0; i < BALL_COUNT; i++) {
            ball_draw(&g->balls[i], g->renderer, g->hud.font);
        }
        
        input_draw(&g->input, &g->balls[0], g->renderer, g->balls, BALL_COUNT, g->balls_moving);
        
        hud_draw(&g->hud, g->renderer, g->current_player, g->score1, g->score2);
        
        if (g->game_over)
            hud_draw_game_over(&g->hud, g->renderer, g->score1, g->score2, g->winner);
        
        SDL_RenderPresent(g->renderer);
    }
}

void game_quit(Game *g) {
    hud_quit(&g->hud);
    audio_quit(&g->audio);
    if (g->renderer) SDL_DestroyRenderer(g->renderer);
    if (g->window)   SDL_DestroyWindow(g->window);
    input_free_texture(&g->input);
    SDL_Quit();
}

void game_reset_balls(Game *g) {
    int colors[15][3] = {
        {255, 220, 50},
        {220, 50, 50},
        {50, 50, 220},
        {150, 0, 150},
        {220, 120, 50},
        {50, 180, 50},
        {180, 50, 50},
        {20, 20, 20},
        {255, 220, 50},
        {220, 50, 50},
        {50, 50, 220},
        {150, 0, 150},
        {220, 120, 50},
        {50, 180, 50},
        {180, 50, 50},
    };
    // white ball
    g->balls[0].x = TABLE_X + TABLE_W * 0.25f;
    g->balls[0].y = TABLE_Y + TABLE_H * 0.5f;
    g->balls[0].vx = 0.0f;
    g->balls[0].vy = 0.0f;
    g->balls[0].radius = 15.0f;
    g->balls[0].r = 255;
    g->balls[0].g = 255;
    g->balls[0].b = 255;
    g->balls[0].is_cue = 1;
    g->balls[0].active = 1;
    g->balls[0].number = 0;
    
    // 15 balls triangle
    float cx = TABLE_X + TABLE_W * 0.70f;
    float cy = TABLE_Y + TABLE_H * 0.5f;
    float spacing = 30.5f;
    int idx = 0;
    
    
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col <= row; col++) {
            int i = idx + 1;
            g->balls[i].x = cx + row * spacing * 0.866f;
            g->balls[i].y = cy + (col - row * 0.5f) * spacing;
            g->balls[i].vx = 0.0f;
            g->balls[i].vy = 0.0f;
            g->balls[i].radius = 15.0f;
            g->balls[i].r = colors[idx][0];
            g->balls[i].g = colors[idx][1];
            g->balls[i].b = colors[idx][2];
            g->balls[i].is_cue = 0;
            g->balls[i].active = 1;
            idx++;
            g->balls[i].number = idx;
        }
    }
}
