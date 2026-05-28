//
//  audio.h
//  
//
//  Created by Abdulkhamid Khamidullin on 27.05.2026.
//

#pragma once

#include <SDL_mixer.h>

typedef struct {
    Mix_Chunk *hitSound;    // when balls hit each other
    Mix_Chunk *sideSound;   // when ball hit the side of table
    Mix_Chunk *pocketSound; // when ball fall into pocket
} Audio;

int  audio_init(Audio *a);
void audio_play_hit(Audio *a);
void audio_play_wall(Audio *a);
void audio_play_pocket(Audio *a);
void audio_quit(Audio *a);
