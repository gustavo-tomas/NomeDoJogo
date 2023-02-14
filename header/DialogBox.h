#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include "GameObject.h"

class DialogBox : public Component {
    public:
        DialogBox(GameObject& associated, string title, string text, Vec2 pos = {0,0}, int wrappingLength = 200);
        ~DialogBox();
        void Update(float dt);
        void Render();
        bool Is(const char* type);
        void Close();
        void SetText(string text);
        void SetTitle(string title);
    private: 
        string text, title;
        weak_ptr<GameObject> titleGo, dialogGo;
};

#endif // DIALOGBOX_H
