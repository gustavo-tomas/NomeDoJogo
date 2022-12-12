#ifndef ALIEN_H
#define ALIEN_H

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"
#include <queue>
#include <memory>

using namespace std;

class Alien : public Component {
    public:
        Alien(GameObject& associated, int nMinions, float timeOffset = 0);
        ~Alien();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);
        static int alienCount;

    private:
        enum AlienState {
            MOVING,
            RESTING
        };
        AlienState state;
        Timer restTimer;
        Vec2 destination;
        Vec2 speed;
        int hp;
        int nMinions;
        float timeOffset;
        vector<weak_ptr<GameObject>> minionArray;
};

#endif // ALIEN_H