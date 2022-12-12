#ifndef PENGUINCANNON_H
#define PENGUINCANNON_H

#include "Component.h"
#include "GameObject.h"
#include "Timer.h"

class PenguinCannon : public Component {
    public:
        PenguinCannon(GameObject& associated, weak_ptr<GameObject> penguinBody);
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);
        void Shoot();

    private:
        weak_ptr<GameObject> pbody;
        float angle;
        Timer timer;
};

#endif // PENGUINCANNON_H