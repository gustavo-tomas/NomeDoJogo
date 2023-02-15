#ifndef CREDITSTATE_H
#define CREDITSTATE_H

#include "State.h"
#include "Music.h"
#include "Timer.h"

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
        void AddText(string text, string fontFile, int fontSize, Vec2 position);
        Music backgroundMusic;
        Timer scrollTimer;
};

#endif // CREDITSTATE_H
