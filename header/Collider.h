#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

class Collider : public Component {
    public:
        Collider(GameObject& associated, Vec2 scale = Vec2(1, 1), Vec2 offset = Vec2(0, 0));
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void ResolveCollisionUpdate(float dt);
        void ApplyImpulse(Vec2 impulse);
        void SetScale(Vec2 scale);
        void SetOffset(Vec2 offset);
        void SetVelocity(Vec2 velocity);
        void SetRestitution(float restitution);
        void SetMass(float mass);
        Rect box;
        Vec2 velocity = Vec2(0.f, 0.f);
        Vec2 correction = Vec2(0.f, 0.f);
        float restitution = 1.f;
        float mass = 1.f;
        bool kinematic = false;

    private:
        Vec2 scale;
        Vec2 offset;
        static bool debug;
};

#endif // COLLIDER_H