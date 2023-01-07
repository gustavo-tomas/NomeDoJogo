#ifndef STATICBOX_H
#define STATICBOX_H

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

using namespace std;

class StaticBox : public Component {
    public:
        StaticBox(GameObject& associated, Vec2 position = Vec2(0, 0));
        ~StaticBox();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);
    private:
        Vec2 lastCameraPos;
};

#endif // TESTBOX_H