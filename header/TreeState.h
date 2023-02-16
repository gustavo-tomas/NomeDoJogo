#ifndef TREESTATE_H
#define TREESTATE_H

#include "State.h"

using namespace std;

class TreeState : public State {
    public:
        TreeState();
        ~TreeState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();
};

#endif // TREESTATE_H
