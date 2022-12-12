#ifndef PENGUINBODY_H
#define PENGUINBODY_H

#include "Component.h"
#include "GameObject.h"

class PenguinBody : public Component {
    public:
        PenguinBody(GameObject& associated);
        ~PenguinBody();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);
        static PenguinBody* player;

    private:
        weak_ptr<GameObject> pcannon;
        Vec2 speed;
        float linearSpeed;
        float angle;
        int hp;
};

#endif // PENGUINBODY_H