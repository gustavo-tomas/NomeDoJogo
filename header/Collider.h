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
        Rect box;

    private:
        Vec2 scale;
        Vec2 offset;
        static bool debug;
};

#endif // COLLIDER_H