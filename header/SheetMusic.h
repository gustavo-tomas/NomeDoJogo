#ifndef SHEETMUSIC_H
#define SHEETMUSIC_H

#include "Component.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Vec2.h"
#include "DialogBox.h"
#include "Sound.h"
#include <string>

class SheetMusic : public Component
{
private:
    vector<const char*> speechs;
    const char* name;
    bool talking = false;
    unsigned int currentSpeech = 0;
    DialogBox* dialog;
    Sound* sound{};
    bool isMusicPlaying{};

public:
    SheetMusic(GameObject& associated, const char* name, Vec2 pos, Sprite sprite, string soudPath);
    void Update(float dt);
    void Render();
    bool Is(const char* type);
    void AddSpeech(const char* text);
    void Interact();
};

#endif
