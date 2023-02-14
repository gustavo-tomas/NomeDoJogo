#include "../header/Sound.h"
#include "../header/Resources.h"

Sound::Sound(GameObject& associated) : Component(associated)
{
    chunk = nullptr;
}

Sound::Sound(GameObject& associated, const char* file, int volume) : Sound(associated)
{
    this->volume = volume;
    Open(file);
}

void Sound::Play(int times)
{
    channel = Mix_PlayChannel(-1, chunk, times - 1);
    SetVolume(volume);
}

void Sound::SetVolume(int volume)
{
    this->volume = volume;
    Mix_Volume(channel, volume);
}

void Sound::Pause()
{
    Mix_Pause(channel);
}

void Sound::Resume()
{
    Mix_Resume(channel);
}

void Sound::Stop(int msToStop)
{
    if (chunk != nullptr && channel > -1)
    {
        if (Mix_FadeOutChannel(channel, msToStop) < 0)
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
    Stop(0);
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
