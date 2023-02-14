#include "../header/Music.h"
#include "../header/Resources.h"

Music::Music()
{
    music = nullptr;
}

Music::Music(const char* file, int volume)
{
    this->volume = volume;
    music = nullptr;
    Open(file);
}

void Music::Play(int times)
{
    SetVolume(volume);
    if (Mix_PlayMusic(music, times) < 0)
    {
        cout << "Error playing music" << endl;
        cout << SDL_GetError() << endl;
    }
}

void Music::Pause()
{
    Mix_PauseMusic();
}

void Music::Resume()
{
    Mix_ResumeMusic();
}

void Music::SetVolume(int volume)
{
    Mix_VolumeMusic(volume);
}

void Music::Stop(int msToStop)
{
    Mix_FadeOutMusic(msToStop);
}

void Music::Open(const char* file)
{
    music = Resources::GetMusic(file);
}

bool Music::IsOpen()
{
    return (music == nullptr);
}

Music::~Music()
{
    Stop(0);
}