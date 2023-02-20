#include "../header/NoteSpawner.h"
#include "../header/GameObject.h"
#include "../header/State.h"
#include "../header/Game.h"
#include "../header/CameraFollower.h"
#include "../header/GameData.h"
#include "../header/Note.h"
#include <fstream>
#include <string>

NoteSpawner::NoteSpawner(GameObject& associated) : Component(associated), notes()
{
    spawnerX = 0;
}

NoteSpawner::NoteSpawner(GameObject& associated, string sheetMusic, int32_t spawnerX) : Component(associated), notes()
{    
    ifstream myfile;
    myfile.open(sheetMusic);

    if (!myfile) return;

    this->spawnerX = spawnerX;

    string buff;
    getline(myfile, buff, ':');
    getline(myfile, buff);
    speed = stold(buff);
    ind = 0;

    CameraFollower *spawnerCF = new CameraFollower(associated, Vec2(GameData::WIDTH + 20, GameData::HEIGHT - 20*4 - 40));
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

NoteSpawner::~NoteSpawner()
{
    
}

void NoteSpawner::Update(float dt)
{
    State& state = Game::GetInstance().GetCurrentState();
    while ((unsigned long int) ind < notes.size())
    {
        if (timer.Get() >= notes[ind].time * speed - 1 / speed) 
        {
            GameObject *noteGo = new GameObject();
            noteGo->box.SetVec(Vec2(associated.box.x, associated.box.y + 20 * notes[ind].h + 1));
            Note *note = new Note(*noteGo, (GameData::WIDTH + 20 - spawnerX) * speed, 3 - notes[ind].h);
            noteGo->AddComponent(note);
            state.AddObject(noteGo, 10050);
            ind++;
        } 
        else
            break;
    }
    timer.Update(dt);
}

void NoteSpawner::Render()
{
    
}

bool NoteSpawner::Is(const char* type)
{
    string str_type = type;
    return str_type == "NoteSpawner";
}
