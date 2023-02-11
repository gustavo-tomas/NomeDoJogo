#include "../header/NPC.h"
#include "../header/Collider.h"

NPC::NPC(GameObject& associated, Vec2 pos, Sprite sprite) : Component(associated){
    associated.box.SetVec(pos);

    Sprite* npcSprite = new Sprite(sprite);
    associated.AddComponent(npcSprite);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
}

void NPC::Update(float dt){}

void NPC::Render(){}

bool NPC::Is(const char* type){
    string str_type = type;
    return (str_type == "NPC");
}
