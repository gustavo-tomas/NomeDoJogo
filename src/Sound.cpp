#include "../header/Sound.h"
#include "../header/Resources.h"

Sound::Sound(GameObject& associated) : Component(associated)
{
    channel = -1;
    chunk = nullptr;
}

Sound::Sound(GameObject& associated, const char* file) : Sound(associated)
{
    channel = -1;
    Open(file);
}

void Sound::Play(int times)
{
    channel = Mix_PlayChannel(channel, chunk, times - 1);
}

void Sound::Stop()
{
    if (chunk != nullptr && channel > -1)
    {
        if (Mix_HaltChannel(channel) != 0)
        {
            cout << "Failed to halt channel: " << channel << "\n";
            cout << SDL_GetError() << "\n";
        }

        else
            cout << "Stopped channel: " << channel << "\n";
    }
}

void Sound::StopAllSounds()
{
    Mix_HaltChannel(-1);
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