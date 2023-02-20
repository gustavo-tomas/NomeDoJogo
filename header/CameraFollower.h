#ifndef CAMERAFOLLOWER_H
#define CAMERAFOLLOWER_H

#include "Component.h"
#include "Vec2.h"

class CameraFollower : public Component {
    public:
        CameraFollower(GameObject& go, Vec2 offset = Vec2(0, 0));
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void setOffset(Vec2 offset);
    
    private:
        Vec2 offset;
};

#endif // CAMERAFOLLOWER_H
