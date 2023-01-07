#include "../header/SpriteRect.h"
#include "../header/Game.h"
#include "../header/Resources.h"
#include "../header/Camera.h"

SpriteRect::SpriteRect(GameObject& associated) : Component(associated)
{
    texture = nullptr;
    scale = Vec2(1, 1);
    color = 0; 
    height = 100; 
    width = 100;
    SetClip(0, 0, width, height);
}

SpriteRect::SpriteRect(GameObject& associated, uint32_t color, float height, float width, int frameCount, float frameTime, float secondsToSelfDestruct) : SpriteRect(associated)
{
    this->frameCount = frameCount;
    this->frameTime = frameTime;
    this->secondsToSelfDestruct = secondsToSelfDestruct;
    this->color = color;
    this->height = height;
    this->width = width;
    timeElapsed = 0;
    currentFrame = 0;
    SetClip(0, 0, width, height);
}

SpriteRect::~SpriteRect()
{
    
}

void SpriteRect::SetClip(int x, int y, int width, int height)
{
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = width;
    clipRect.h = height;
}

void SpriteRect::Update(float dt)
{
    timeElapsed += dt;
    if (timeElapsed > frameTime)
    {
        currentFrame = (currentFrame + 1) % frameCount;
        int frameWidth = width / frameCount;
        SetClip(frameWidth * currentFrame, 0, frameWidth, height);
        timeElapsed = 0;
    }

    if (secondsToSelfDestruct > 0)
    {
        selfDestructCount.Update(dt);
        if (selfDestructCount.Get() >= secondsToSelfDestruct)
            associated.RequestDelete();
    }
}

void SpriteRect::Render()
{
    Render(associated.box.x - Camera::pos.x, associated.box.y - Camera::pos.y);
}

void SpriteRect::Render(int x, int y)
{
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = clipRect.w * scale.x;
    dstRect.h = clipRect.h * scale.y;

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), color&0xff, (color>>8)&0xff, (color>>16)&0xff, (color>>24)&0xff);
    if (SDL_RenderFillRect(
        Game::GetInstance().GetRenderer(),
        &dstRect) < 0)
    {
        cout << "Error filling copy" << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }
    // if (SDL_RenderCopyEx(
    //     Game::GetInstance().GetRenderer(),
    //     texture,
    //     &clipRect,
    //     &dstRect,
    //     associated.angleDeg * DEG, // 0° to 360° clockwise
    //     nullptr,
    //     SDL_FLIP_NONE) < 0)
    // {
    //     cout << "Error rendering copy" << endl;
    //     cout << SDL_GetError() << endl;
    //     exit(1);
    // }
}

bool SpriteRect::Is(const char* type)
{
    string str_type = type;
    return (str_type == "SpriteRect");
}

int SpriteRect::GetWidth()
{
    return (width / frameCount) * scale.x;
}

int SpriteRect::GetHeight()
{
    return height * scale.y;
}

void SpriteRect::SetScale(float scaleX, float scaleY)
{
    scale = Vec2(scaleX, scaleY);

    if (scaleX > 0)
    {
        float diff = ((width / frameCount * scaleX) - width / frameCount) / 2.0;
        associated.box.x -= diff;
        associated.box.w = (width / frameCount) * scaleX;
    }

    if (scaleY > 0)
    {
        float diff = ((height * scaleY) - height) / 2.0;
        associated.box.y -= diff;
        associated.box.h = height * scaleY;
    }
}

Vec2 SpriteRect::GetScale()
{
    return scale;
}

void SpriteRect::SetFrame(int frame)
{
    currentFrame = frame;
    int frameWidth = width / frameCount;
    SetClip(frameWidth * currentFrame, 0, frameWidth, height);
}

void SpriteRect::SetFrameCount(int frameCount)
{
    this->frameCount = frameCount;
    this->currentFrame = 0;
    associated.box.w = (width / frameCount) * scale.x;
}

void SpriteRect::SetFrameTime(float frameTime)
{
    this->frameTime = frameTime;
}

bool SpriteRect::IsOpen()
{
    if (texture != nullptr)
        return true;
    return false;
}