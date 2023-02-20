#include "../header/CutsceneState.h"
#include "../header/GameObject.h"
#include "../header/Sprite.h"
#include "../header/InputManager.h"
#include "../header/CameraFollower.h"
#include "../header/GameData.h"
#include "../header/Resources.h"

CutsceneState::CutsceneState(vector<string> scenes, float duration, vector<string> dialogs, float dialogDuration, State* nextState) : State()
{
    cout << "\nCutscene State\n";
    this->scenes = scenes;
    this->duration = duration;
    this->sceneCounter = 0;

    this->dialogs = dialogs;
    this->dialogDuration = dialogDuration;
    this->dialogCounter = 0;

    this->nextState = nextState;
}

CutsceneState::~CutsceneState()
{
    objectArray.clear();
    backgroundMusic.Stop(1000);
    cout << "CutsceneState deleted successfully!\n";
}

void CutsceneState::Start()
{
    LoadAssets();
    StartArray();
    started = true;
}

void CutsceneState::LoadAssets()
{
    backgroundMusic = Music((GameData::audiosPath + "Soundtrack/Prelude.mp3").c_str(), 25);
    backgroundMusic.Play(-1);

    // Load sprites beforehand
    for (auto scene : scenes)
        Resources::GetImage(scene);

    for (auto dialog : dialogs)
        Resources::GetImage(dialog);

    // Background
    GameObject* bgGo = new GameObject();
    Sprite* bg = new Sprite(*bgGo, (GameData::imagesPath + "background.png").c_str());
    bg->SetScale(GameData::BASE_HEIGHT * 1.0f / bg->GetUnscaledHeight(), GameData::BASE_WIDTH * 1.0f / bg->GetUnscaledWidth()); // Original resolution: 1920x1080
    bg->isProportionActive = true;
    CameraFollower* cf = new CameraFollower(*bgGo);

    bgGo->AddComponent(bg);
    bgGo->AddComponent(cf);
    AddObject(bgGo, -GameData::HEIGHT - 10000);

    // Initial scene
    GameObject* sceneGo = new GameObject();
    Sprite* sceneSprite = new Sprite(*sceneGo, scenes[sceneCounter].c_str());
    sceneSprite->SetScale(GameData::BASE_HEIGHT * 1.0f / sceneSprite->GetUnscaledHeight(), GameData::BASE_WIDTH * 1.0f / sceneSprite->GetUnscaledWidth()); // Original resolution: 1920x1080
    sceneSprite->isProportionActive = true;
    CameraFollower* sceneCf = new CameraFollower(*sceneGo);

    sceneGo->AddComponent(sceneSprite);
    sceneGo->AddComponent(sceneCf);
    currentScene = AddObject(sceneGo, -GameData::HEIGHT);

    // Initial dialog
    if (dialogs.size() < 1) return;

    GameObject* dialogGO = new GameObject();
    Sprite* dialogSprite = new Sprite(*dialogGO, dialogs[dialogCounter].c_str());
    dialogSprite->SetScale(0.4, 0.4);
    
    dialogGO->AddComponent(dialogSprite);
    dialogGO->box.SetCenter(Vec2(GameData::BASE_WIDTH / 2.0, GameData::HEIGHT - dialogGO->box.h / 2.0));
    CameraFollower* dialogCf = new CameraFollower(*dialogGO, dialogGO->box.GetVec());

    dialogGO->AddComponent(dialogCf);
    currentDialog = AddObject(dialogGO, 20002);
}

void CutsceneState::Update(float dt)
{
    // Sets quit requested
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY) || sceneCounter >= scenes.size())
    {
        popRequested = true;
        Game::GetInstance().Push(nextState);
        return;
    }

    sceneTimer.Update(dt);

    if (sceneTimer.Get() > duration)
    {
        sceneTimer.Restart();
        sceneCounter++;

        if (sceneCounter > 0 && sceneCounter < scenes.size())
            SetScene(sceneCounter);
    }

    dialogTimer.Update(dt);

    if (dialogTimer.Get() > dialogDuration)
    {
        dialogTimer.Restart();
        dialogCounter++;

        if (dialogCounter > 0 && dialogCounter < dialogs.size())
            SetDialog(dialogCounter);
    }

    UpdateArray(dt);
}

void CutsceneState::SetScene(unsigned index)
{
    if(!currentScene.expired()){
        ((Sprite *) currentScene.lock()->GetComponent("Sprite"))->ChangeSprite(scenes[index].c_str());
    }
}

void CutsceneState::SetDialog(unsigned index)
{
    if(!currentDialog.expired()){
        Sprite * dialogSprite = ((Sprite *) currentDialog.lock()->GetComponent("Sprite"));
        dialogSprite->ChangeSprite(dialogs[index].c_str());

        currentDialog.lock()->box.SetCenter(Vec2(GameData::BASE_WIDTH / 2.0, GameData::HEIGHT - 150));
    }
}

void CutsceneState::Render()
{
    RenderArray();
}

void CutsceneState::Pause()
{
}

void CutsceneState::Resume()
{
}
