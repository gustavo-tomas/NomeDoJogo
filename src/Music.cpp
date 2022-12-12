#include "../header/Music.h"
#include "../header/Resources.h"

Music::Music()
{
    music = nullptr;
}

Music::Music(const char* file)
{
    music = nullptr;
    Open(file);
}

void Music::Play(int times)
{
    if (Mix_PlayMusic(music, times) < 0)
    {
        cout << "Error playing music" << endl;
        cout << SDL_GetError() << endl;
    }
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