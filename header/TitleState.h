#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "Music.h"
#include "State.h"
#include "CameraFollower.h"
#include "Sprite.h"

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
        CameraFollower* titleCf;
        Sprite* title;
};

#endif // TITLESTATE_H