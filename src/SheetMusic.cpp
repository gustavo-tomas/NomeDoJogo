#include "../header/SheetMusic.h"
#include "../header/Collider.h"
#include "../header/Game.h"
#include "../header/InputManager.h"
#include "../header/GameData.h"
#include "../header/Sound.h"
#include "../header/WorldState.h"
#include "../header/Player.h"

using namespace std;

unsigned int SheetMusic::sheetCounter = 0;

SheetMusic::SheetMusic(GameObject& associated, const char* name, Vec2 pos, Sprite sprite, string soudPath) : Component(associated)
{
    associated.box.SetVec(pos);

    this->name = name;

    Sprite* npcSprite = new Sprite(sprite);
    associated.AddComponent(npcSprite);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    sound = new Sound(associated, soudPath.c_str(), 15);
    associated.AddComponent(sound);

    SheetMusic::sheetCounter++;
}

void SheetMusic::Update(float dt)
{
    if (!sound->IsOpen() && isMusicPlaying)
    {
        WorldState &worldState = dynamic_cast<WorldState&>(Game::GetInstance().GetCurrentState());
        Sound* bgSound = (Sound*) worldState.backgroundMusic.lock()->GetComponent("Sound");
        bgSound->Resume();
        isMusicPlaying = false;
        associated.RequestDelete();

    }

    if(talking == true)
        Interact();

    else if
    (
        InputManager::GetInstance().KeyPress(E_KEY) && !isMusicPlaying &&
        associated.box.GetCenter().GetDistance(GameData::playerPos) < (associated.box.w/2 + associated.box.h/2 + 25) 
    )
    {
        if(speechs.size() > currentSpeech)
        {
            GameObject* textGo = new GameObject();
            dialog = new DialogBox(*textGo, name, speechs[currentSpeech], Vec2(25, GameData::HEIGHT - 100), 1000);
            textGo->AddComponent(dialog);
            Game::GetInstance().GetCurrentState().AddObject(textGo);
            talking = true;
        } 
        else
        {
            WorldState &worldState = dynamic_cast<WorldState&>(Game::GetInstance().GetCurrentState());
            Sound* bgSound = (Sound*) worldState.backgroundMusic.lock()->GetComponent("Sound");
            bgSound->Pause();
            sound->Play();
            isMusicPlaying = true;
        }
    }
}

void SheetMusic::Render(){}

bool SheetMusic::Is(const char* type)
{
    string str_type = type;
    return (str_type == "SheetMusic");
}

void SheetMusic::AddSpeech(const char* text)
{
    speechs.push_back(text);
}

void SheetMusic::Interact()
{
    if(associated.box.GetCenter().GetDistance(GameData::playerPos) > associated.box.w/2 + associated.box.h/2 + 25)
    {
        dialog->Close();
        talking = false;
        currentSpeech = 0;
    }
    else if(InputManager::GetInstance().KeyPress(E_KEY) && !isMusicPlaying)
    {
        if(++currentSpeech >= speechs.size())
        {
            dialog->Close();
            currentSpeech = 0;
            talking = false;

            WorldState &worldState = dynamic_cast<WorldState&>(Game::GetInstance().GetCurrentState());
            Sound* bgSound = (Sound*) worldState.backgroundMusic.lock()->GetComponent("Sound");
            bgSound->Pause();
            sound->Play();
            isMusicPlaying = true;

            Player::player->SetAction(Player::Action::PRACTING);
            associated.RemoveComponent(associated.GetComponent("Sprite"));
            WorldState::collectedSongs++;
        } 
        else 
            dialog->SetText(speechs[currentSpeech]);
    }
}
