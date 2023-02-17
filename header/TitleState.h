#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "GameObject.h"
#include "State.h"
#include "CameraFollower.h"
#include "Sprite.h"
#include "Music.h"
#include "Timer.h"

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
        CameraFollower* titleCf;
        Sprite* title;
        weak_ptr<GameObject> cursor;
        Music backgroundMusic;
        Timer musicTimer;
};

#endif // TITLESTATE_H
