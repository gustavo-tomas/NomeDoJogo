#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "Component.h"
#include "GameObject.h"

class UserInterface : public Component {
    public:
        UserInterface(GameObject& associated, Vec2 position = Vec2(0, 0), bool flipped = false);
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void UpdateHeart(int value);
        void UpdateLifebar(int value);
        void UpdateManabar(int value);

    private:
        map<string, GameObject*> ui;
};

#endif // USERINTERFACE_H
