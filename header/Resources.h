#ifndef RESOURCES_H
#define RESOURCES_H

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF

#include "SDL_include.h"
#include <string>
#include <unordered_map>

using namespace std;

class Resources {
    public:
        static SDL_Texture* GetImage(string file);
        static Mix_Music* GetMusic(string file);
        static Mix_Chunk* GetSound(string file);
        static TTF_Font* GetFont(string file, int fontSize);
        static void ClearImages();
        static void ClearMusics();
        static void ClearSounds();
        static void ClearFonts();

    private:
        static unordered_map<string, SDL_Texture*> imageTable;
        static unordered_map<string, Mix_Music*> musicTable;
        static unordered_map<string, Mix_Chunk*> soundTable;
        static unordered_map<string, TTF_Font*> fontTable;
};

#endif // RESOURCES_H