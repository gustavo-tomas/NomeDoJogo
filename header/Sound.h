#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>
#define INCLUDE_SDL_MIXER

#include "SDL_include.h"
#include "GameObject.h"
#include <iostream>

using namespace std;

class Sound : public Component {
    public:
        Sound(GameObject& associated);
        Sound(GameObject& associated, const char* file, int volume = MIX_MAX_VOLUME);
        ~Sound();
        void Play(int times = 1);
        void Stop();
        void Open(const char* file);
        bool IsOpen();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void SetVolume(int volume);
        static void StopAllSounds();

    private:
        Mix_Chunk* chunk;
        int channel;
        int volume;
};

#endif // SOUND_H
