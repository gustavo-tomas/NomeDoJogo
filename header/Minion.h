#ifndef MINION_H
#define MINION_H

#define ANG_INC M_PI / 4.0

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

class Minion : public Component {
    public:
        Minion(GameObject& associated, weak_ptr<GameObject> alienCenter, float arcOffsetDeg = 0);
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);
        void Shoot(Vec2 target);

    private:
        GameObject* alienCenter;
        float arc;
};

#endif // MINION_H