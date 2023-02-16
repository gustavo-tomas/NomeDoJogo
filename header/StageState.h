#ifndef STAGESTATE_H
#define STAGESTATE_H

#include "GameObject.h"
#include "Music.h"
#include "State.h"
#include "Timer.h"

using namespace std;

class StageState : public State {
    public:
        StageState();
        ~StageState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();
        static bool playerTurn;

        struct MusicInfo
        {
            string musicFile, notesFile;
            int duration;
        };

    private:
        Music backgroundMusic;
        Timer musicTimer;
        vector<MusicInfo> musics; 
        unsigned int currentMusic;
        weak_ptr<GameObject> player;
        weak_ptr<GameObject> fpsCounter;
};

#endif // STAGESTATE_H
