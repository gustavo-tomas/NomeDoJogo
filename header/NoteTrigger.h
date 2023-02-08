#ifndef NOTETRIGGER_H
#define NOTETRIGGER_H

#include "Component.h"
#include "GameObject.h"

class NoteTrigger : public Component {
    public:
        NoteTrigger(GameObject& associated, int triggerKey = 0, int32_t spriteNum = 0);
        ~NoteTrigger();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void NotifyCollision(GameObject& other);

    private:
        int triggerKey;
};

#endif // NOTE_H
