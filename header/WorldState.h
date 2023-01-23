#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include "Music.h"
#include "GameObject.h"
#include "State.h"

using namespace std;

class WorldState : public State {
    public:
        WorldState();
        ~WorldState();
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
        GameObject* playerGo;
        vector<weak_ptr<GameObject>> colliderArray;
};

#endif // WORLDSTATE_H
