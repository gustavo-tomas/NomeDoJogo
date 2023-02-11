#ifndef STAGESTATE_H
#define STAGESTATE_H

#include "GameObject.h"
#include "Music.h"
#include "State.h"

using namespace std;

class StageState : public State {
    public:
        StageState();
        ~StageState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();

    private:
        Music backgroundMusic;
        weak_ptr<GameObject> player;
        weak_ptr<GameObject> fpsCounter;
};

#endif // STAGESTATE_H
