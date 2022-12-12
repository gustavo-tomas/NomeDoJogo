#ifndef BULLET_H
#define BULLET_H

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

class Bullet : public Component {
    public:
        Bullet(GameObject& associated, float angle, float speed,
            int damage, float maxDistance, const char* sprite,
            int frameCount = 1, float frameTime = 1, bool targetsPlayer = true);
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);
        int GetDamage();
        bool targetsPlayer;

    private:
        Vec2 speed;
        float distanceLeft;
        int damage;
};

#endif // BULLET_H