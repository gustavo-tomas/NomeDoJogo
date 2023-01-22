#ifndef STAGESTATE_H
#define STAGESTATE_H

#include "Sprite.h"
#include "Music.h"
#include "GameObject.h"
#include "State.h"
#include "TileSet.h"
#include <vector>
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
        void AddColliderObject(weak_ptr<GameObject>& object);

    private:
        Music backgroundMusic;
        GameObject* fpsCounter;
        vector<weak_ptr<GameObject>> colliderArray;
};

#endif // STAGESTATE_H
