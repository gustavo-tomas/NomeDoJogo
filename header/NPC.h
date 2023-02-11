#ifndef NPC_H
#define NPC_H

#include "Component.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Vec2.h"

class NPC : public Component
{
public:
    NPC(GameObject& associated, Vec2 pos, Sprite sprite);
    void Update(float dt);
    void Render();
    bool Is(const char* type);
};

#endif
