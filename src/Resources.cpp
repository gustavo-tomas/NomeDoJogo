#include "../header/Resources.h"
#include "../header/Game.h"
#include <iostream>

// Create resources
unordered_map<string, SDL_Texture*> Resources::imageTable;
unordered_map<string, Mix_Music*> Resources::musicTable;
unordered_map<string, Mix_Chunk*> Resources::soundTable;
unordered_map<string, TTF_Font*> Resources::fontTable;

SDL_Texture* Resources::GetImage(string file)
{
    // Check if texture is already loaded
    if (imageTable.find(file) != imageTable.end())
        return imageTable[file];
    
    SDL_Texture* texture;
    if ((texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str())) == nullptr)
    {
        cout << "Error loading texture " << file << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }

    else
    {
        imageTable[file] = texture;
        cout << "\nTexture " << file << " loaded successfully!" << endl;
        return imageTable[file];
    }
}

Mix_Music* Resources::GetMusic(string file)
{
    // Check if music is already loaded
    if (musicTable.find(file) != musicTable.end())
        return musicTable[file];
    
    Mix_Music* music;
    if ((music = Mix_LoadMUS(file.c_str())) == nullptr)
    {
        cout << "Error loading music " << file << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }

    else
    {
        musicTable[file] = music;
        cout << "\nMusic " << file << " loaded successfully!" << endl;
        return musicTable[file];
    }
}

Mix_Chunk* Resources::GetSound(string file)
{
    // Check if sound is already loaded
    if (soundTable.find(file) != soundTable.end())
        return soundTable[file];
    
    Mix_Chunk* chunk;
    if ((chunk = Mix_LoadWAV(file.c_str())) == nullptr)
    {
        cout << "Error loading sound " << file << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }

    else
    {
        soundTable[file] = chunk;
        cout << "\nSound " << file << " loaded successfully!" << endl;
        return soundTable[file];
    }
}

TTF_Font* Resources::GetFont(string file, int fontSize)
{
    // Concatenates file with fontSize to create key
    string fileName = file + to_string(fontSize);

    if (fontTable.find(fileName.c_str()) != fontTable.end())
        return fontTable[fileName.c_str()];

    TTF_Font* font;
    if ((font = TTF_OpenFont(file.c_str(), fontSize)) == nullptr)
    {
        cout << "Error loading font " << file << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }

    else
    {
        fontTable[fileName.c_str()] = font;
        cout << "\nFont " << fileName << " loaded successfully!" << endl;
        return fontTable[fileName.c_str()];
    }
}

void Resources::ClearImages()
{
    for (auto i : imageTable)
        SDL_DestroyTexture(i.second);

    imageTable.clear();
    cout << "Textures destroyed successfully!" << endl;
}

void Resources::ClearMusics()
{
    for (auto m : musicTable)
        Mix_FreeMusic(m.second);

    musicTable.clear();
    cout << "Musics freed successfully!" << endl;
}

void Resources::ClearSounds()
{
    for (auto c : soundTable)
        Mix_FreeChunk(c.second);

    soundTable.clear();
    cout << "Sounds freed successfully!" << endl;
}

void Resources::ClearFonts()
{
    for (auto f : fontTable)
        TTF_CloseFont(f.second);

    fontTable.clear();
    cout << "Fonts closed successfully!" << endl;
}