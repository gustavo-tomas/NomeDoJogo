#ifndef SPAWNER_H
#define SPAWNER_H

#include "GameObject.h"
#include "Timer.h"

struct NoteInfo{
    int h;
    float time;
};

class Spawner : public Component {
    public:
        Spawner(GameObject& associated);
        Spawner(GameObject& associated, string sheetMusic);
        ~Spawner();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
    private:
        float speed;
        vector<NoteInfo> notes;
        int ind;
        Timer timer;
};

#endif // SPAWNER_H