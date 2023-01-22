#include "../header/Spawner.h"
#include "../header/GameObject.h"
#include "../header/State.h"
#include "../header/Game.h"
#include "../header/SpriteRect.h"
#include "../header/CameraFollower.h"
#include "../header/GameData.h"
#include "../header/Note.h"
#include <fstream>
#include <string>


Spawner::Spawner(GameObject& associated) : Component(associated), notes(){
    
}

Spawner::Spawner(GameObject& associated, string sheetMusic) : Component(associated), notes(){
    std::ifstream myfile; 
    myfile.open(sheetMusic);

    std::string buff;
    getline(myfile, buff, ':');
    getline(myfile, buff);
    speed = stoll(buff);
    ind = 0;

    SpriteRect *spriteRect = new SpriteRect(associated, 0xFF3333FF, 20*4, 20);
    associated.AddComponent(spriteRect);
    CameraFollower *spawnerCF = new CameraFollower(associated, Vec2(GameData::WIDTH + 20, GameData::HEIGHT - 20*4));
    associated.AddComponent(spawnerCF);

    while (getline(myfile, buff, ':') && buff == "note")
    {
        getline(myfile, buff, ',');
        int line = stoi(buff);
        getline(myfile, buff);
        float time = stof(buff);
        notes.push_back(NoteInfo{line, time});
    }
}

Spawner::~Spawner(){
    
}

void Spawner::Update(float dt){
    State& state = Game::GetInstance().GetCurrentState();
    while (ind < notes.size())
    {
        if (timer.Get() >= notes[ind].time)
        {
            GameObject *noteGo = new GameObject();
            noteGo->box.SetVec(Vec2(associated.box.x, associated.box.y + 20 * notes[ind].h + 1));
            Note *note = new Note(*noteGo, speed);
            noteGo->AddComponent(note);
            state.AddObject(noteGo, 2);
            ind++;
        } else{
            break;
        }
    }
    timer.Update(dt);
}

void Spawner::Render(){
    
}

bool Spawner::Is(const char* type){
    string str_type = type;
    return str_type == "Spawner";
}
