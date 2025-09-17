/*
    audio.c - API for dfplayer
    Wrapper for DFPlayer_mini for ease of use
    Needs dfplayer driver
*/

#include "../main.h"
#include "Drivers/DFPlayer_mini.h"

void audio_init(){
    df_init();
    df_setVolume(20);
}

void audio_play_shooting(){
    df_playFolder(1, 1);
}

void audio_play_explosion(){
    df_playFolder(1, 2);
}

void audio_play_gameOver(){
    df_playFolder(1, 3);
}

void audio_stop(){
    df_stop();
}