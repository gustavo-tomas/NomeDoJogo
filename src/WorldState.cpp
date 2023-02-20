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
#include "../header/StageState.h"
#include "../header/CutsceneState.h"
#include "../header/TreeState.h"
#include "../header/NPC.h"
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
    // Music
    GameObject* bgMusic = new GameObject();
    Sound* music = new Sound(*bgMusic, "./assets/audio/musics/background.mp3", 15);
    
    bgMusic->AddComponent(music);
    music->Play(-1);

    backgroundMusic = AddObject(bgMusic);

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
    map->SetScale(0.15, 0.15);

    mapGo->AddComponent(map);
    AddObject(mapGo, 5002);
    
    auto mapHeight = map->GetHeight();
    auto mapWidth = map->GetWidth();

    // Player
    GameObject* playerGo = new GameObject();
    Player* playerComp = new Player(*playerGo);
    playerGo->box.SetVec(Vec2(537, 1314));
    playerGo->AddComponent(playerComp);
    player = AddObject(playerGo, 10020);

    // GameObject* border1GO = new GameObject();
    // border1GO->box.x = 0;
    // border1GO->box.y = 120;
    // border1GO->box.w = 90;
    // border1GO->box.h = mapHeight;
    // Collider* border1collider = new Collider(*border1GO, Vec2(1, 1), Vec2(0, 0));
    // border1GO->AddComponent(border1collider);
    // AddObject(border1GO, 1000000);

    // GameObject* border2GO = new GameObject();
    // border2GO->box.x = 0;
    // border2GO->box.w = mapWidth;
    // border2GO->box.h = 120;
    // Collider* border2collider = new Collider(*border2GO, Vec2(1, 1), Vec2(0, 0));
    // border2GO->AddComponent(border2collider);
    // AddObject(border2GO, 1000000);

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

    // // NPC 1
    // GameObject* npcGo = new GameObject();
    // Sprite npcSprite = Sprite(*npcGo, "./assets/image/250_scout.png");
    // npcSprite.SetScale(0.5, 0.5);
    // NPC* npc = new NPC(*npcGo, "Scout", Vec2(1800, 500), npcSprite);
    // npc->AddSpeech("Ola!");
    // npc->AddSpeech("Bom Dia!");

    // npcGo->AddComponent(npc);
    // AddObject(npcGo, 10020);

    // NPC 2
    // GameObject* npcGo2 = new GameObject();
    // NPC* npc2 = new NPC(*npcGo2, "Mage", Vec2(850, 300), Sprite(*npcGo2, "./assets/image/mage-1-85x94.png", 4, 2, 0.2));
    // npc2->AddSpeech("Lorem ipsum dolor amet."
    //                 " Eu esqueci o resto da frase."
    //                 " Aqui vai uma receita de bolo entao: "
    //                 " ... eu nao sei fazer bolo :(");
    
    // npcGo2->AddComponent(npc2);
    // AddObject(npcGo2, 10020);

    // // Partituras
    // for (int i = 0; i < 3; i++)
    // {
    //     GameObject* npcGo3 = new GameObject();
    //     SheetMusic* npc3 = new SheetMusic(*npcGo3, "Mage", Vec2(1050 + (i * 70), 300), Sprite(*npcGo3, "./assets/image/mage-1-85x94.png", 4, 2, 0.2), "assets/audio/Combate/Pre-Score(Luna).mp3");
    //     npc3->AddSpeech("Lorem ipsum dolor amet."
    //                     " Eu esqueci o resto da frase."
    //                     " Aqui vai uma receita de bolo entao: "
    //                     " ... eu nao sei fazer bolo :(");
    //     npcGo3->AddComponent(npc3);
    //     AddObject(npcGo3, 10020);
    // }

        GameObject* npcGo3 = new GameObject();
        SheetMusic* npc3 = new SheetMusic(*npcGo3, "Mage", Vec2(1700, 1300), Sprite(*npcGo3, "./assets/image/mage-1-85x94.png", 4, 2, 0.2), "assets/audio/Combate/Pre-Score(Luna).mp3");
        npc3->AddSpeech("Você encontrou uma partitura de música, experimente tocá-la!");
        npcGo3->AddComponent(npc3);
        AddObject(npcGo3, 10020);

        npcGo3 = new GameObject();
        npc3 = new SheetMusic(*npcGo3, "Mage", Vec2(900, 800), Sprite(*npcGo3, "./assets/image/mage-1-85x94.png", 4, 2, 0.2), "assets/audio/Combate/Pre-Score(Luna).mp3");
        npc3->AddSpeech("Você encontrou uma partitura de música, experimente tocá-la!");
        npcGo3->AddComponent(npc3);
        AddObject(npcGo3, 10020);

        npcGo3 = new GameObject();
        npc3 = new SheetMusic(*npcGo3, "Mage", Vec2(600, 150), Sprite(*npcGo3, "./assets/image/mage-1-85x94.png", 4, 2, 0.2), "assets/audio/Combate/Pre-Score(Luna).mp3");
        npc3->AddSpeech("Você encontrou uma partitura de música, experimente tocá-la!");
        npcGo3->AddComponent(npc3);
        AddObject(npcGo3, 10020);



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

    // World Objects
    vector<WorldObject> objects = {};

    // ifstream mappingFile; 
    // mappingFile.open("./assets/map/world_objects.txt");

    // string buff;
    // getline(mappingFile, buff);

    // Format: < name, position, scale, >
    // while (!getline(mappingFile, buff).eof())
    // {
    //     size_t pos = 0;
    //     bool nameFound = false;
    //     string name;
    //     vector<float> values;

    //     while ((pos = buff.find(",")) != string::npos)
    //     {
    //         string token = buff.substr(0, pos);
            
    //         if (!nameFound)
    //         {
    //             name = token;
    //             nameFound = true;
    //         }

    //         else
    //             values.push_back(stof(token));

    //         buff.erase(0, pos + 1);
    //     }

    //     objects.push_back({
    //         .name = name,
    //         .position = {values[0], values[1]},
    //         .scale = {values[2], values[3]},
    //         .colliderScale = {values[4], values[5]},
    //         .colliderOffset = {values[6], values[7]}
    //     });
    // }

    // mappingFile.close();

    for (auto object : objects)
    {
        GameObject* objectGo = new GameObject();

        Sprite* objectSprite = new Sprite(*objectGo, (GameData::objectsPath + object.name).c_str());
        objectSprite->SetScale(object.scale.x, object.scale.y);
        objectGo->AddComponent(objectSprite);

        Collider* objectCollider = new Collider(*objectGo, object.colliderScale, object.colliderOffset);
        objectGo->AddComponent(objectCollider);
        
        objectGo->box.SetVec(object.position);
        AddObject(objectGo, 10000);
    }
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
            if(colliderArray[i].expired() || colliderArray[j].expired())
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
    for(uint32_t i = 0; i < objectArray.size(); i++)
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
