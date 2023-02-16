#ifndef CUTSCENESTATE_H
#define CUTSCENESTATE_H

#include "GameObject.h"
#include "State.h"
#include "Timer.h"

class CutsceneState : public State {
    public:
        CutsceneState(vector<string> scenes, float duration = 5.0, vector<string> dialogs = {}, float dialogDuration = 5.0, State* nextState = nullptr);
        ~CutsceneState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();
        void SetScene(unsigned index);
        void SetDialog(unsigned index);

    private:
        vector<string> scenes, dialogs;
        float duration, dialogDuration;
        Timer sceneTimer, dialogTimer;
        unsigned sceneCounter, dialogCounter;
        weak_ptr<GameObject> currentScene, currentDialog;
        State* nextState;
};

#endif // CUTSCENESTATE_H
