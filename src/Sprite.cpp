#include "../header/Sprite.h"
#include "../header/Game.h"
#include "../header/Resources.h"
#include "../header/Camera.h"

Sprite::Sprite(GameObject& associated) : Component(associated)
{
    texture = nullptr;
    scale = Vec2(1, 1);
}

Sprite::Sprite(GameObject& associated, const char* file, int frameCountX, int frameCountY, float frameTime, float secondsToSelfDestruct) : Sprite(associated)
{
    this->frameCountX = frameCountX;
    this->frameCountY = frameCountY;
    this->frameTime = frameTime;
    this->secondsToSelfDestruct = secondsToSelfDestruct;
    timeElapsed = 0;
    currentFrame = 0;
    Open(file);
}

Sprite::~Sprite()
{
    
}

void Sprite::Open(const char* file)
{
    texture = Resources::GetImage(file);

    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SetClip(0, 0, width / frameCountX, height / frameCountY);

    associated.box.w = width / frameCountX;
    associated.box.h = height / frameCountY;
}

void Sprite::ChangeSprite(const char* file, int frameCountX, int frameCountY, float frameTime)
{
    texture = nullptr;

    this->frameCountX = frameCountX;
    this->frameCountY = frameCountY;
    this->frameTime = frameTime;

    Open(file);
}

void Sprite::SetClip(int x, int y, int width, int height)
{
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = width;
    clipRect.h = height;
}

void Sprite::Update(float dt)
{
    timeElapsed += dt;
    if (timeElapsed > frameTime)
    {
        currentFrame = (currentFrame + 1) % (frameCountX * frameCountY);
        int frameWidth = width / frameCountX;
        int frameHeight = height / frameCountY;
        SetClip(frameWidth * (currentFrame % frameCountX), frameHeight * (currentFrame / frameCountX), frameWidth, frameHeight);
        timeElapsed = 0;
    }

    if (secondsToSelfDestruct > 0)
    {
        selfDestructCount.Update(dt);
        if (selfDestructCount.Get() >= secondsToSelfDestruct)
            associated.RequestDelete();
    }
}

void Sprite::Render()
{
    Render(associated.box.x - Camera::pos.x, associated.box.y - Camera::pos.y);
}

void Sprite::Render(int x, int y)
{
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = clipRect.w * scale.x;
    dstRect.h = clipRect.h * scale.y;

    if (SDL_RenderCopyEx(
        Game::GetInstance().GetRenderer(),
        texture,
        &clipRect,
        &dstRect,
        associated.angleDeg * DEG, // 0° to 360° clockwise
        nullptr,
        SDL_FLIP_NONE) < 0)
    {
        cout << "Error rendering copy" << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }
}

bool Sprite::Is(const char* type)
{
    string str_type = type;
    return (str_type == "Sprite");
}

int Sprite::GetWidth()
{
    return (width / frameCountX) * scale.x;
}

int Sprite::GetHeight()
{
    return (height / frameCountY) * scale.y;
}

void Sprite::SetScale(float scaleX, float scaleY)
{
    scale = Vec2(scaleX, scaleY);

    if (scaleX > 0)
    {
        float diff = ((width / frameCountX * scaleX) - width / frameCountX) / 2.0;
        associated.box.x -= diff;
        associated.box.w = (width / frameCountX) * scaleX;
    }

    if (scaleY > 0)
    {
        float diff = ((height / frameCountY * scaleY) - height / frameCountY) / 2.0;
        associated.box.y -= diff;
        associated.box.h = (height / frameCountY) * scaleY;
    }
}

Vec2 Sprite::GetScale()
{
    return scale;
}

void Sprite::SetFrame(int frame)
{
    currentFrame = frame;
    int frameWidth = width / frameCountX;
    SetClip(frameWidth * currentFrame, 0, frameWidth, height);
}

void Sprite::SetFrameCount(int frameCountX, int frameCountY)
{
    this->frameCountX = frameCountX;
    this->frameCountY = frameCountY;
    this->currentFrame = 0;
    associated.box.w = (width / frameCountX) * scale.x;
    associated.box.h = (height / frameCountY) * scale.y;
}

void Sprite::SetFrameTime(float frameTime)
{
    this->frameTime = frameTime;
}

bool Sprite::IsOpen()
{
    if (texture != nullptr)
        return true;
    return false;
}
