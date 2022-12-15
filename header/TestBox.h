#ifndef TESTBOX_H
#define TESTBOX_H

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

using namespace std;

class TestBox : public Component {
    public:
        TestBox(GameObject& associated, Vec2 position = Vec2(0, 0));
        ~TestBox();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);
};

#endif // TESTBOX_H