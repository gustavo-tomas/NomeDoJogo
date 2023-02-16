#ifndef CONTROLSTATE_H
#define CONTROLSTATE_H

#include "State.h"

class ControlState : public State {
    public:
        ControlState();
        ~ControlState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();

    private:
        void AddText(string text, string fontFile, int fontSize, Vec2 position);
};

#endif // CONTROLSTATE_H
