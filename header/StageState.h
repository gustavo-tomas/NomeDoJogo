#ifndef STAGESTATE_H
#define STAGESTATE_H

#include "GameObject.h"
#include "State.h"
#include <memory>

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
        weak_ptr<GameObject> backgroundMusic;
        weak_ptr<GameObject> fpsCounter;
};

#endif // STAGESTATE_H
