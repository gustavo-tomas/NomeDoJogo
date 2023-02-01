#ifndef PLAYER_H
#define PLAYER_H

#include "Component.h"
#include "GameObject.h"
#include "Timer.h"

class Player : public Component {
    public:
        Player(GameObject& associated, bool moveLimits = false);
        ~Player();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);
        void Shoot();
        
        void ResetAttackPower() {
            attackPower = 0;
        }

        void AddAttackPower(float value) {
            attackPower += value;
        }

        static Player* player;

    private:
        float linearSpeed;
        float angle;
        int hp;
        int mana;
        float attackPower;
        bool moveLimits;
        Timer shootTimer;
};

#endif // PLAYER_H
