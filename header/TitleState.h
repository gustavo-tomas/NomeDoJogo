#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "Music.h"
#include "State.h"

class TitleState : public State {
    public:
        TitleState();
        ~TitleState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();

    private:
        Music backgroundMusic;
};

#endif // TITLESTATE_H