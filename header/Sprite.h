#ifndef SPRITE_H
#define SPRITE_H

#define INCLUDE_SDL_IMAGE
#define DEG 57.2958

#include "SDL_include.h"
#include "Component.h"
#include "Vec2.h"
#include "Timer.h"
#include <iostream>

using namespace std;

class Sprite : public Component {
    public:
        Sprite(GameObject& associated);
        Sprite(GameObject& associated, const char* file, int frameCountX = 1, int frameCountY = 1, float frameTime = 1, float secondsToSelfDestruct = 0);
        ~Sprite();
        void Open(const char* file);
        void ChangeSprite(const char* file, int frameCountX = 1, int frameCountY = 1, float frameTime = 1);
        void SetClip(int x, int y, int w, int h);
        void Update(float dt);
        bool Is(const char* type);
        void Render();
        void Render(int x, int y);
        void Render(int x, int y, int w, int h);
        int GetWidth();
        int GetHeight();
        int GetUnscaledWidth();
        int GetUnscaledHeight();
        void SetScale(float scaleX, float scaleY);
        Vec2 GetScale();
        void SetFrame(int frame);
        void SetFrameCount(int frameCountX, int frameCountY);
        void SetFrameTime(float frameTime);
        bool IsOpen();
        bool isProportionActive{};

    private:
        SDL_Texture* texture;
        int width;
        int height;
        Vec2 scale;
        int frameCountX;
        int frameCountY;
        int currentFrame;
        float timeElapsed;
        float frameTime;
        SDL_Rect clipRect;
        Timer selfDestructCount;
        float secondsToSelfDestruct;
};

#endif // SPRITE_H
