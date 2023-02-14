#ifndef NPC_H
#define NPC_H

#include "Component.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Vec2.h"
#include "DialogBox.h"

class NPC : public Component
{
private:
    vector<const char*> speechs;
    const char* name;
    bool talking = false;
    unsigned int currentSpeech = 0;
    DialogBox* dialog;

public:
    NPC(GameObject& associated, const char* name, Vec2 pos, Sprite sprite);
    void Update(float dt);
    void Render();
    bool Is(const char* type);
    void AddSpeech(const char* text);
    void Interact();
};

#endif
