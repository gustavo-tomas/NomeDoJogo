#ifndef STATE_H
#define STATE_H

#include "GameObject.h"
#include "util.h"
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <utility>

using namespace std;

class State {
    public:
        State();
        virtual ~State();
        virtual void LoadAssets() = 0; // pure
        virtual void Update(float dt) = 0; // pure
        virtual void Render() = 0; // pure
        virtual void Start() = 0; // pure
        virtual void Pause() = 0; // pure
        virtual void Resume() = 0; // pure
        virtual void AddColliderObject(weak_ptr<GameObject>& object);
        virtual weak_ptr<GameObject> AddObject(GameObject* object, uint32_t layer = 0);
        virtual weak_ptr<GameObject> GetObjectPtr(GameObject* object);
        virtual void UpdateLayer(GameObject *go, uint32_t newLayer = 0);
        bool PopRequested();
        bool QuitRequested();

    protected:
        void StartArray();
        virtual void UpdateArray(float dt);
        virtual void RenderArray();
        bool popRequested;
        bool quitRequested;
        bool started;
        vector<shared_ptr<GameObject>> objectArray;
        vector<weak_ptr<GameObject>> colliderArray;
        multiset<pair<uint32_t, weak_ptr<GameObject>>, pointer_compare<pair<uint32_t, weak_ptr<GameObject>>>> renderArray;
};

#endif // STATE_H
