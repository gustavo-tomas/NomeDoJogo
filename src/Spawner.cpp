#include "../header/Spawner.h"
#include <fstream>
#include <string>


Spawner::Spawner(GameObject& associated, string sheetMusic) : Component(associated), notes(){
    std::ifstream myfile; 
    myfile.open("sheetMusic");

    std::string buff;
    getline(myfile, buff, ':');
    getline(myfile, buff);
    speed = stoll(buff);
    ind = 0;

    while (getline(myfile, buff, ':') && buff == "note")
    {
        getline(myfile, buff, ',');
        int line = stoi(buff);
        getline(myfile, buff);
        float time = stof(buff);
        notes.push_back(NoteInfo{line, time});
    }
}

void Spawner::Update(float dt){
    timer.Update(dt);
    while(ind < notes.size())
    {
        if(abs(timer.Get() - notes[ind].time) <= 0.00001){
            // Spawn note;
            ind++;
        } else{
            break;
        }
    }
}

void Spawner::Render(){
    
}

bool Spawner::Is(const char* type){
    string str_type = type;
    return str_type == "Spawner";
}
