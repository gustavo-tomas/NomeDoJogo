#ifndef NOTESPAWNER_H
#define NOTESPAWNER_H

#include "GameObject.h"
#include "Timer.h"

struct NoteInfo
{
    int h;
    float time;
};

class NoteSpawner : public Component {
    public:
        NoteSpawner(GameObject& associated);
        NoteSpawner(GameObject& associated, string sheetMusic);
        ~NoteSpawner();
        void Update(float dt);
        void Render();
        bool Is(const char* type);

    private:
        float speed;
        vector<NoteInfo> notes;
        int ind;
        Timer timer;
};

#endif // NOTESPAWNER_H
