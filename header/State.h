#ifndef STATE_H
#define STATE_H

#include "GameObject.h"
#include <vector>
#include <memory>

using namespace std;

class State {
    public:
        State();
        virtual ~State();
        void virtual LoadAssets() = 0; // pure
        void virtual Update(float dt) = 0; // pure
        void virtual Render() = 0; // pure
        void virtual Start() = 0; // pure
        void virtual Pause() = 0; // pure
        void virtual Resume() = 0; // pure
        virtual weak_ptr<GameObject> AddObject(GameObject* object);
        virtual weak_ptr<GameObject> GetObjectPtr(GameObject* object);
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
};

#endif // STATE_H