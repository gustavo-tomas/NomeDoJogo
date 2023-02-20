#include "../header/WorldState.h"
#include "../header/Camera.h"
#include "../header/CameraFollower.h"
#include "../header/Collider.h"
#include "../header/Collision.h"
#include "../header/Game.h"
#include "../header/GameData.h"
#include "../header/InputManager.h"
#include "../header/Player.h"
#include "../header/Sprite.h"
#include "../header/Sound.h"
#include "../header/CutsceneState.h"
#include "../header/TreeState.h"
#include "../header/SheetMusic.h"
#include "../header/DialogBox.h"
#include <fstream>
#include <string>

unsigned int WorldState::collectedSongs = 0;

WorldState::
WorldState() : State()
{
    cout << "\nWorld State\n";
    SheetMusic::sheetCounter = 0;
    WorldState::collectedSongs = 0;
}

WorldState::~WorldState()
{
    objectArray.clear();
    cout << "WorldState deleted successfully!\n";
}

void WorldState::Start()
{
    LoadAssets();
    StartArray();
    started = true; 
}

void WorldState::LoadAssets()
{
    // Background
    GameObject* bgGo = new GameObject();
    Sprite* bg = new Sprite(*bgGo, "./assets/image/background.png");
    bg->SetScale(4.0, 4.0);
    CameraFollower* cf = new CameraFollower(*bgGo);

    bgGo->AddComponent(bg);
    bgGo->AddComponent(cf);
    AddObject(bgGo, -GameData::HEIGHT);

    // Map
    GameObject* mapGo = new GameObject();
    Sprite* map = new Sprite(*mapGo, "./assets/image/map.png");
    map->SetScale(0.30, 0.30);

    mapGo->AddComponent(map);
    AddObject(mapGo, 5002);

    // Player
    GameObject* playerGo = new GameObject();
    Player* playerComp = new Player(*playerGo);
    playerGo->box.SetVec(Vec2(537, 1314));
    playerGo->AddComponent(playerComp);
    player = AddObject(playerGo, 10020);

    // vector<Rect> positions{{-1172, -181, 3245, 20}, {-1172, -181, 20, 1892}, {-1172, 1711, 3245, 20}, {2073, -181, 20, 1892}};
    vector<Rect> positions{{-1172, -181, 3245-40, 100}, {-1172, -181, 100, 1892}, {-1172, 1711, 3245, 100}, {2073, -181, 100, 1892}};

    for (auto position : positions)
    {
        GameObject* objectGo = new GameObject();
        objectGo->box = position;

        Collider* objectCollider = new Collider(*objectGo);
        objectGo->AddComponent(objectCollider);
        
        AddObject(objectGo);
    }

    GameObject* mageGo = new GameObject();
    SheetMusic* mage = new SheetMusic(*mageGo, "Mage", Vec2(1700, 1300), Sprite(*mageGo, "./assets/image/mage-1-85x94.png", 4, 2, 0.2), "assets/audio/Combate/Pre-Score(Luna).mp3");
    mage->AddSpeech("Você encontrou uma partitura de música, experimente tocá-la!");
    mageGo->AddComponent(mage);
    AddObject(mageGo, 10020);

    mageGo = new GameObject();
    mage = new SheetMusic(*mageGo, "Mage", Vec2(900, 800), Sprite(*mageGo, "./assets/image/mage-1-85x94.png", 4, 2, 0.2), "assets/audio/Combate/Pre-Score(Luna).mp3");
    mage->AddSpeech("Você encontrou uma partitura de música, experimente tocá-la!");
    mageGo->AddComponent(mage);
    AddObject(mageGo, 10020);

    mageGo = new GameObject();
    mage = new SheetMusic(*mageGo, "Mage", Vec2(600, 150), Sprite(*mageGo, "./assets/image/mage-1-85x94.png", 4, 2, 0.2), "assets/audio/Combate/Pre-Score(Luna).mp3");
    mage->AddSpeech("Você encontrou uma partitura de música, experimente tocá-la!");
    mageGo->AddComponent(mage);
    AddObject(mageGo, 10020);

    // Camera
    Camera::Follow(playerGo);

    // Dialog
    GameObject* dialogGo = new GameObject();
    DialogBox* dialog = new DialogBox(*dialogGo, "Sua missão", "Encontre todas as partituras musicais",
                                      Vec2(GameData::WIDTH - 250, 20));
    dialogGo->AddComponent(dialog);
    AddObject(dialogGo, 20002);

    // Songs counter
    GameObject* songsGo = new GameObject();
    DialogBox* songDialog = new DialogBox(*songsGo, "Partituras", 
                                                    to_string(WorldState::collectedSongs) + "/" +
                                                    to_string(SheetMusic::sheetCounter), Vec2(GameData::WIDTH - 250, 90));

    songsGo->AddComponent(songDialog);
    counterDialog = AddObject(songsGo, 20002);

    // Music
    GameObject* bgMusic = new GameObject();
    Sound* music = new Sound(*bgMusic, "./assets/audio/Soundtrack/Walking_Theme.mp3", 15);
    
    bgMusic->AddComponent(music);
    music->Play(-1);

    backgroundMusic = AddObject(bgMusic);
}

void WorldState::Update(float dt)
{
    if (QuitRequested() || PopRequested())
        return;

    // Sets quit requested
    if (InputManager::GetInstance().QuitRequested())
    {
        quitRequested = true;
        return;
    }

    // Returns to title screen
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY) || GameData::returnToMenu)
    {
        popRequested = true;
        return;
    }

    // Creates new StageState
    if (!player.expired() && (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || collectedSongs == SheetMusic::sheetCounter))
        ((Player *) player.lock().get()->GetComponent("Player"))->SetAction(Player::Action::PREPARING);

    // Creates new TreeState
    if (InputManager::GetInstance().KeyPress(SDLK_t))
    {
        // Game::GetInstance().Push(new TreeState());
        vector<string> scenes { 
            GameData::imagesPath + "loading.png"
        };
        Game::GetInstance().Push(new CutsceneState(scenes, 3.0, {}, 8.0, new TreeState()));

        return;
    }

    // Updates collected
    if (!counterDialog.expired())
    {
        auto dialog = ((DialogBox *) counterDialog.lock().get()->GetComponent("DialogBox"));
        if (dialog != nullptr)
            dialog->SetText(to_string(collectedSongs) + "/" + to_string(SheetMusic::sheetCounter));
    }

    // Updates GOs
    UpdateArray(dt);

    // Deletes GOs
    for (uint32_t i = 0; i < objectArray.size(); i++)
        if (objectArray[i]->IsDead())
            objectArray.erase(objectArray.begin() + i--);

    // Checks for colisions
    for (uint32_t i = 0; i < colliderArray.size(); i++)
    {
        for (uint32_t j = i + 1; j < colliderArray.size(); j++)
        {
            if (colliderArray[i].expired() || colliderArray[j].expired())
                continue;
            
            auto weakColliderA = colliderArray[i].lock().get();
            auto weakColliderB = colliderArray[j].lock().get();

            Collider* colliderA = (Collider*) weakColliderA->GetComponent("Collider");
            Collider* colliderB = (Collider*) weakColliderB->GetComponent("Collider");

            if (colliderA == nullptr || colliderB == nullptr)
                continue;

            if (!Collision::IsColliding(colliderA->box, colliderB->box, weakColliderA->angleDeg, weakColliderB->angleDeg))
                continue;
            
            weakColliderA->NotifyCollision(*weakColliderB);
            weakColliderB->NotifyCollision(*weakColliderA);
            Collision::ResolveCollision(*colliderA, *colliderB);
            
            // Update collisions before next frame
            colliderA->ResolveCollisionUpdate(dt);
            colliderB->ResolveCollisionUpdate(dt);
        }
    }

    // Updates the camera
    Camera::Update(dt);

    // Updates camera followers (o fix mais feio da história)
    for (uint32_t i = 0; i < objectArray.size(); i++)
        if ((CameraFollower *) objectArray[i]->GetComponent("CameraFollower") != nullptr)
            objectArray[i]->GetComponent("CameraFollower") ->Update(dt);
}

void WorldState::Render()
{
    if (GameData::returnToMenu) return;
    RenderArray();
}

void WorldState::Pause()
{
    if (!backgroundMusic.expired())
        ((Sound *) backgroundMusic.lock().get()->GetComponent("Sound"))->Pause();
    if (!player.expired())
        ((Sound *) player.lock().get()->GetComponent("Sound"))->Pause();
}

void WorldState::Resume()
{
    if (!player.expired())
        Camera::Follow(player.lock().get());
    if (!backgroundMusic.expired())
        ((Sound *) backgroundMusic.lock().get()->GetComponent("Sound"))->Resume();
}
