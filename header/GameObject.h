#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Rect.h"
#include "Component.h"
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <iostream>

using namespace std;

class GameObject {
    public:
        GameObject();
        ~GameObject();
        void Start();
        void Update(float dt);
        void Render();
        bool IsDead();
        void RequestDelete();
        void AddComponent(Component* cpt);
        void RemoveComponent(Component* cpt);
        Component* GetComponent(const char* type);
        void NotifyCollision(GameObject& other);
        Rect box;
        bool started;
        double angleDeg;

    private:
    
        vector<unique_ptr<Component>> components;
        bool isDead;
};

#endif // GAMEOBJECT_H