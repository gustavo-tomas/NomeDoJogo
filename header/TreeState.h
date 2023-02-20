#ifndef TREESTATE_H
#define TREESTATE_H

#include "State.h"
#include "Music.h"

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

    private:
        Music backgroundMusic;
        Rect healingArea;
        weak_ptr<GameObject> player;
};

#endif // TREESTATE_H
