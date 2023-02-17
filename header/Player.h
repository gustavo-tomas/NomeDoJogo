#ifndef PLAYER_H
#define PLAYER_H

#include "Component.h"
#include "GameObject.h"
#include "Timer.h"
#include <map>
#include <string>

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
        void ResetAttackPower();
        void AddAttackPower(float value);
        void ResetMana();
        void AddMana(int value);

        static Player* player;

        struct SpriteInfo
        {
            const char* fileName;
            int frameCountX, frameCountY, framesMissing = 0;
        };

        enum Action
        {
            IDLE,
            WALKING_LEFT,
            WALKING_UP,
            WALKING_RIGHT,
            WALKING_DOWN,
            IDLE_PERFORMING,
            WALK_PERFORMING,
            PREPARING,
            LOSS,
            PRACTING
        };

        void SetAction(Action action);

    private:
        void ActionsHandler(Vec2 velocity);

        float linearSpeed;
        float angle;
        int hp;
        int mana;
        float attackPower;
        bool moveLimits;
        Timer stunTimer, actionTimer;
        int stunHeat;
        
        Action currentAction, previousAction;
};

#endif // PLAYER_H
