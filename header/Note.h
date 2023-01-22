#ifndef NOTE_H
#define NOTE_H

#include "Component.h"
#include "GameObject.h"

class Note : public Component {
    public:
        Note(GameObject& associated);
        Note(GameObject& associated, float speed);
        ~Note();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);

    private:
        float speed;
};

#endif // NOTE_H
