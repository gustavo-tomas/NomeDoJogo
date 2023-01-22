#ifndef NOTETRIGGER_H
#define NOTETRIGGER_H

#include "Component.h"
#include "GameObject.h"

class NoteTrigger : public Component {
    public:
        NoteTrigger(GameObject& associated);
        ~NoteTrigger();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);

    private:
};

#endif // NOTE_H
