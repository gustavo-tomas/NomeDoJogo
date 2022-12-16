#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"
#include "GameObject.h"

class Collider : public Component {
    public:
        Collider(GameObject& associated, Vec2 scale = Vec2(1, 1), Vec2 offset = Vec2(0, 0));
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void SetScale(Vec2 scale);
        void SetOffset(Vec2 offset);
        void SetVelocity(Vec2 velocity);
        void SetRestitution(float restitution);
        void SetMass(float mass);
        Rect box;
        Vec2 velocity = Vec2(0, 0);
        float restitution = 1.0;
        float mass = 1000000;

    private:
        Vec2 scale;
        Vec2 offset;
        static bool debug;
};

#endif // COLLIDER_H