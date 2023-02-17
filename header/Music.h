#ifndef MUSIC_H
#define MUSIC_H

#define INCLUDE_SDL_MIXER

#include "SDL_include.h"
#include <iostream>

using namespace std;

class Music {
    public:
        Music();
        ~Music();
        Music(const char* file, int volume = MIX_MAX_VOLUME);
        void Play(int times = -1);
        void Stop(int msToStop = 1500);
        void Pause();
        void Resume();
        void Open(const char* file);
        bool IsOpen();
        void SetVolume(int volume);
        bool IsPlaying();
        
    private:
        Mix_Music* music;
        int volume;
};

#endif // MUSIC_H