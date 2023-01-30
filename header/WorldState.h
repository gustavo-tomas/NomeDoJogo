#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include "Music.h"
#include "GameObject.h"
#include "State.h"

using namespace std;

typedef struct WorldObject {
    string name;
    Vec2 position = {0, 0};
    Vec2 scale = {1, 1};
    Vec2 colliderScale = {1, 1};
    Vec2 colliderOffset = {0, 0};
} WorldObject;

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
