#include "../main.h"
#include "DFPlayer_mini.h"

void audio_init(){
    DFPM_set_device(1);     //for TF card
    DFPM_set_EQ(0);         //for normal eq
    DFPM_set_volume(2);
}

void audio_play_shooting(){
    DFPM_play_track(2);
}

void audio_play_startSong(){
    DFPM_play_track(1);
}

void audio_play_gameOverSong(){
    DFPM_play_track(3);
}

void audio_stop(){
    DFPM_stop();
}