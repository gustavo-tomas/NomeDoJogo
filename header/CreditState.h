#ifndef CREDITSTATE_H
#define CREDITSTATE_H

#include "GameObject.h"
#include "State.h"
#include "CameraFollower.h"
#include "Sprite.h"

class CreditState : public State {
    public:
        CreditState();
        ~CreditState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();

    private:
};

#endif // TITLESTATE_H
