#ifndef SOUND_H
#define SOUND_H

#define INCLUDE_SDL_MIXER

#include "SDL_include.h"
#include "GameObject.h"
#include <iostream>

using namespace std;

class Sound : public Component {
    public:
        Sound(GameObject& associated);
        Sound(GameObject& associated, const char* file);
        ~Sound();
        void Play(int times = 1);
        void Stop();
        void Open(const char* file);
        bool IsOpen();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        static void StopAllSounds();

    private:
        Mix_Chunk* chunk;
        int channel;
};

#endif // SOUND_H
