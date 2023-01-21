#ifndef PLAYER_H
#define PLAYER_H

#include "Component.h"
#include "GameObject.h"
#include "Timer.h"

class Player : public Component {
    public:
        Player(GameObject& associated);
        ~Player();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);
        void Shoot();
        static Player* player;

    private:
        float linearSpeed;
        float angle;
        int hp;
        Timer shootTimer;
};

#endif // PLAYER_H