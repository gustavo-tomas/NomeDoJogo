#include "../header/Sound.h"
#include "../header/Resources.h"

Sound::Sound(GameObject& associated) : Component(associated)
{
    chunk = nullptr;
}

Sound::Sound(GameObject& associated, const char* file) : Sound(associated)
{
    Open(file);
}

void Sound::Play(int times)
{
    channel = Mix_PlayChannel(-1, chunk, times - 1);
}

void Sound::Stop()
{
    if (chunk != nullptr && channel >= -1)
        Mix_HaltChannel(channel);
}

void Sound::Open(const char* file)
{
    chunk = Resources::GetSound(file);
}

Sound::~Sound()
{
    Stop();
}

bool Sound::IsOpen()
{
    return Mix_Playing(channel);
}

void Sound::Update(float dt)
{

}

void Sound::Render()
{
    
}

bool Sound::Is(const char* type)
{
    string str_type = type;
    return (str_type == "Sound");
}