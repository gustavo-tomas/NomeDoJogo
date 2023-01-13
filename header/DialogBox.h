#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include "GameObject.h"

class DialogBox : public Component {
    public:
        DialogBox(GameObject& associated, string dialogText);
        void Update(float dt);
        void Render();
        bool Is(const char* type);
};

#endif // DIALOGBOX_H