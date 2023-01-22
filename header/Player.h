#ifndef PLAYER_H
#define PLAYER_H

#include "Component.h"
#include "GameObject.h"

class Player : public Component {
    public:
        Player(GameObject& associated);
        ~Player();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);

        void ResetAttackPower(){
            attackPower = 0;
        }
        void AddAttackPower(float value){
            attackPower += value;
        }

        static Player* player;

    private:
        float linearSpeed;
        float angle;
        int hp;
        int mana;
        float attackPower;
};

#endif // PLAYER_H
