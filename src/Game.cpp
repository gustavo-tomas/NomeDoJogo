#include "../header/Game.h"
#include "../header/InputManager.h"
#include "../header/EventManager.h"
#include "../header/GameData.h"
#include "../header/CameraFollower.h"
#include "../header/Resources.h"

Game* Game::instance = nullptr;

Game::Game(const char* title, int width, int height)
{
    if (instance != nullptr)
    {
        cout << "A game instance already exists" << endl;
        exit(1);
    }
    else
        instance = this;

    // Initializes SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
    {
        cout << "Error initializing SDL" << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }

    // Initializes SDL_Image
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) == 0)
    {
        cout << "Error initializing IMG" << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }

    // Initializes TTF
    if (TTF_Init() != 0)
    {
        cout << "Error initializing TTF" << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }

    // Initializes SDL_Mixer
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0)
    {
        cout << "Error initializing MIX" << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }

    // Initializes OpenAudio
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0)
    {
        cout << "Error initializing OpenAudio" << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }

    // Allocates audio channels
    Mix_AllocateChannels(32);

    // Creates game window
    if ((window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        0)) == nullptr)
    {
        cout << "Error creating window" << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }
    // SDL_SetWindowResizable(window, SDL_TRUE);
    // SDL_SetWindowFullscreen(window, SDL_TRUE);

    // Creates renderer
    if ((renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED)) == nullptr)
    {
        cout << "Error creating renderer" << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }

    // Initializes dt and frameStart
    frameStart = SDL_GetTicks();
    dt = 0;

    // Seeds RNG
    srand(time(NULL));

    // Creates state
    storedState = nullptr;
}

void Game::UpdateDelay(Uint64 start, Uint64 end)
{
    float elapsed = (end - start) / (float) SDL_GetPerformanceFrequency();
    GameData::delay = (1000.0f / GameData::targetFPS) - (elapsed * 1000.0f);
}

void Game::UpdateFPS(Uint64 start, Uint64 end)
{
    float elapsed = (end - start) / (float) SDL_GetPerformanceFrequency();
    GameData::currentFPS = 1.0f / elapsed;
}

void Game::CalculateDeltaTime()
{
    dt = (SDL_GetTicks() - frameStart) / 1000.0;
    frameStart = SDL_GetTicks();
}

float Game::GetDeltaTime()
{
    return dt;
}

SDL_Window* Game::GetWindow(){
    return window;
}

Game& Game::GetInstance()
{
    if (instance == nullptr)
        instance = new Game(GameData::TITLE, GameData::WIDTH, GameData::HEIGHT);

    return *instance;
}

SDL_Renderer* Game::GetRenderer()
{
    return renderer;
}

State& Game::GetCurrentState()
{
    return *(stateStack.top());
}

void Game::Push(State* state)
{
    storedState = state;
}

void Game::Run()
{
    if (storedState == nullptr)
    {
        delete this;
        return;
    }

    stateStack.push(unique_ptr<State>(storedState));
    stateStack.top()->Start();
    storedState = nullptr;

    while (!stateStack.empty() && !stateStack.top()->QuitRequested())
    {
        Uint64 start = SDL_GetPerformanceCounter();

        if (stateStack.top()->PopRequested())
        {
            stateStack.pop();
            if (!stateStack.empty())
                stateStack.top()->Resume();
            else
                break;
        }
        
        if (storedState != nullptr)
        {
            stateStack.top()->Pause();
            stateStack.push(unique_ptr<State>(storedState));
            stateStack.top()->Start();
            storedState = nullptr;
        }
        
        CalculateDeltaTime();
        EventManager::GetInstance().Update();
        InputManager::GetInstance().Update();
        if(InputManager::GetInstance().KeyPress(F_KEY)){
            swap(GameData::HEIGHT, GameData::PREV_HEIGHT);
            swap(GameData::WIDTH, GameData::PREV_WIDTH);
            GameData::fullscreenUpdateCounter = 2;
            if(GameData::isFullScreen){
                SDL_SetWindowSize(Game::GetInstance().GetWindow(), GameData::WIDTH, GameData::HEIGHT);
                SDL_SetWindowFullscreen(window, SDL_FALSE);
            } else{
                SDL_DisplayMode dm;
                SDL_GetCurrentDisplayMode(0, &dm);
                GameData::HEIGHT = dm.h;
                GameData::WIDTH = dm.w;
                SDL_SetWindowSize(Game::GetInstance().GetWindow(), GameData::WIDTH, GameData::HEIGHT);
                SDL_SetWindowFullscreen(window, SDL_TRUE);
            }
            GameData::isFullScreen = !GameData::isFullScreen;
        }
        if(GameData::fullscreenUpdateCounter > 0){
            GameData::fullscreenUpdateCounter--;
        }
        stateStack.top()->Update(dt);
        stateStack.top()->Render();

        SDL_RenderPresent(renderer);

        Uint64 end = SDL_GetPerformanceCounter();
        UpdateDelay(start, end);
    
        if (GameData::delay > 0.0) SDL_Delay(GameData::delay);
        UpdateFPS(start, SDL_GetPerformanceCounter());
    }

    delete this;
}

Game::~Game()
{
    // Free resources
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
    Resources::ClearFonts();

    if (storedState != nullptr)
        delete storedState;

    while (!stateStack.empty())
        stateStack.pop();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    cout << "\n\nGame deleted successfully!" << endl;
}
