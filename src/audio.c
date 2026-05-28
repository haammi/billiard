//
//  audio.c
//  
//
//  Created by Abdulkhamid Khamidullin on 27.05.2026.
//

#include "audio.h"
#include <stdio.h>

int audio_init(Audio *a) {
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        printf("Mix_OpenAudio error: %s\n", Mix_GetError());
        return 0;
    }

    a->hitSound = Mix_LoadWAV("assets/hitSound.wav");
    a->sideSound = Mix_LoadWAV("assets/sideSound.wav");
    a->pocketSound = Mix_LoadWAV("assets/pocketSound.wav");

    return 1;
}

void audio_play_hit(Audio *a) {
    if (a->hitSound) Mix_PlayChannel(-1, a->hitSound, 0);
}

void audio_play_wall(Audio *a) {
    if (a->sideSound) Mix_PlayChannel(-1, a->sideSound, 0);
}

void audio_play_pocket(Audio *a) {
    if (a->pocketSound) Mix_PlayChannel(-1, a->pocketSound, 0);
}

void audio_quit(Audio *a) {
    if (a->hitSound) Mix_FreeChunk(a->hitSound);
    if (a->sideSound) Mix_FreeChunk(a->sideSound);
    if (a->pocketSound) Mix_FreeChunk(a->pocketSound);
    Mix_CloseAudio();
}
