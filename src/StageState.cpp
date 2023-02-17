#include "../header/StageState.h"
#include "../header/Game.h"
#include "../header/Vec2.h"
#include "../header/InputManager.h"
#include "../header/Camera.h"
#include "../header/CameraFollower.h"
#include "../header/Collider.h"
#include "../header/Collision.h"
#include "../header/GameData.h"
#include "../header/EndState.h"
#include "../header/PauseState.h"
#include "../header/Text.h"
#include "../header/Player.h"
#include "../header/Minion.h"
#include "../header/NoteSpawner.h"
#include "../header/NoteTrigger.h"
#include "../header/DialogBox.h"
#include "../header/Sprite.h"
#include "../header/Sound.h"

bool StageState::playerTurn = false;

StageState::StageState() : State()
{
    cout << "\nStageState created successfully!\n" << endl;
    Minion::minionCount = 0;
    StageState::playerTurn = true;
}

void StageState::Start()
{
    LoadAssets();
    StartArray();
    started = true;
    StageState::playerTurn = true;
    
    ((Sound *) player.lock().get()->GetComponent("Sound"))->SetVolume(0);
    ((Player *) player.lock().get()->GetComponent("Player"))->SetAction(Player::Action::IDLE_PERFORMING);
    ((Collider *) player.lock().get()->GetComponent("Collider"))->SetScale({0.7, 0.8});
    ((Collider *) player.lock().get()->GetComponent("Collider"))->SetOffset({5, 0});
}

void StageState::Pause()
{
    backgroundMusic.Pause();
}

void StageState::Resume()
{
    if (!GameData::returnToMenu)
        backgroundMusic.Resume();
}

void StageState::LoadAssets()
{
    // Background Music
    musics = vector<MusicInfo> 
    {
        { GameData::audiosPath + "musics/Pre-Score(Luna).mp3", "./assets/sheet_music/pre_score.txt", 8 },
        { GameData::audiosPath + "musics/Pre-Score(Enemy).mp3", "", 10 },
        { GameData::audiosPath + "musics/1st_Score(Luna).mp3", "./assets/sheet_music/luna_pt1.txt", 9 },
        { GameData::audiosPath + "musics/1st_Score(Enemy).mp3", "", 9 },
        { GameData::audiosPath + "musics/2nd_Score(Luna).mp3", "./assets/sheet_music/luna_pt2.txt", 10 },
        { GameData::audiosPath + "musics/2nd_Score(Enemy).mp3", "", 25 }
    };
    currentMusic = 0;
    if(playerTurn)
    {
        GameObject *spawnerGo = new GameObject(); 
        NoteSpawner *spawner = new NoteSpawner(*spawnerGo, musics[currentMusic].notesFile);
        spawnerGo->AddComponent(spawner);
        AddObject(spawnerGo, 1);
    }

    backgroundMusic = Music(musics[currentMusic].musicFile.c_str(), 12);
    backgroundMusic.Play(1);
    musicTimer.Restart();

    // Dialogs
    dialogs = { "É só tu e eu parceiro", "Nas minhas mãos tu vira pão", "Em terra de cego eu sou rei" };
    currentDialog = 0;

    // Background
    GameObject* bgGo = new GameObject();
    Sprite* bg = new Sprite(*bgGo, "./assets/image/background.png");
    CameraFollower* cf = new CameraFollower(*bgGo);

    bgGo->AddComponent(bg);
    bgGo->AddComponent(cf);
    AddObject(bgGo, -GameData::HEIGHT);

    // Player (muted footsteps)
    GameObject* playerGo = new GameObject();
    Player* playerComp = new Player(*playerGo, true);
    playerGo->box.SetVec(Vec2(104, 154));
    playerGo->AddComponent(playerComp);

    player = AddObject(playerGo, 10000);
    
    // Camera
    Camera::Unfollow();
    Camera::Reset();

    // Minion
    GameObject* minionGo = new GameObject();
    Minion* minion = new Minion(*minionGo, Vec2(704, 150));
    
    minionGo->AddComponent(minion);
    AddObject(minionGo, 10020);

    // Enemy dialog
    GameObject* dialogGo = new GameObject();
    DialogBox* dialog = new DialogBox(*dialogGo, "Robobo", " ", Vec2(GameData::WIDTH - 250, 80));
    dialogGo->AddComponent(dialog);

    this->enemyDialog = AddObject(dialogGo);

    // FPS counter
    GameObject* fps = new GameObject();
    CameraFollower* textFollower = new CameraFollower(*fps, fps->box.GetVec());
    fps->AddComponent(textFollower);

    const char* fontFile = "./assets/font/Inder-Regular.ttf";
    const char* textStr = "FPS ";
    int fontSize = 16;
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {255, 255, 255, 255};
    
    Text* text = new Text(*fps, fontFile, fontSize, style, textStr, color);
    fps->AddComponent(text);
    
    fpsCounter = AddObject(fps, 10020);

    // NoteTriggers
    int triggers[4] = {LEFT_ARROW_KEY, UP_ARROW_KEY, DOWN_ARROW_KEY, RIGHT_ARROW_KEY};
    for (int i = 0; i < 4; i++)
    {
        GameObject *noteTriggerGo = new GameObject(); 
        noteTriggerGo->box.SetVec(Vec2(40, GameData::HEIGHT - 20 * (i + 1) - 40));
        NoteTrigger *noteTrigger = new NoteTrigger(*noteTriggerGo, triggers[4 - i - 1], i);
        noteTriggerGo->AddComponent(noteTrigger);
        AddObject(noteTriggerGo, 1);
    }
}

void StageState::Update(float dt)
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
    if (GameData::returnToMenu)
    {
        popRequested = true;
        return;
    }

    // Pause
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY))
    {
        Game::GetInstance().Push(new PauseState());
        return;
    }

    // Updates the camera
    Camera::Update(dt);

    // Updates GOs
    UpdateArray(dt);

    // Deletes GOs
    for (uint32_t i = 0; i < objectArray.size(); i++)
        if (objectArray[i]->IsDead())
            objectArray.erase(objectArray.begin() + i--);

    // Checks for Player Victory
    if (Minion::minionCount <= 0)
    {
        GameData::playerVictory = true;
        Game::GetInstance().Push(new EndState());
        popRequested = true;
        return;
    }

    // Checks for Player Defeat
    if (Player::player == nullptr)
    {
        GameData::playerVictory = false;
        Game::GetInstance().Push(new EndState());
        popRequested = true;
        return;
    }

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

    // Updates FPS counter
    Text* FPS_Text = (Text*) fpsCounter.lock().get()->GetComponent("Text");
    if (FPS_Text != nullptr)
        FPS_Text->SetText(("FPS " + to_string(floor(GameData::currentFPS))).c_str());

    // Update music
    musicTimer.Update(dt);
    if(musicTimer.Get() > musics[currentMusic].duration)
    {
        currentMusic = (currentMusic + 1) %  musics.size();
        backgroundMusic = Music(musics[currentMusic].musicFile.c_str(), 15);
        backgroundMusic.Play(1);
        musicTimer.Restart();

        // NoteSpawner
        playerTurn = !playerTurn;
        if(playerTurn)
        {
            GameObject *spawnerGo = new GameObject(); 
            NoteSpawner *spawner = new NoteSpawner(*spawnerGo, musics[currentMusic].notesFile);
            spawnerGo->AddComponent(spawner);
            AddObject(spawnerGo, 1);
        }
        


        // Enemy rants
        else
            ((DialogBox *) enemyDialog.lock().get()->GetComponent("DialogBox"))->SetText(dialogs[currentDialog++ % dialogs.size()]);
    
    
    }
}

void StageState::Render()
{
    if (GameData::returnToMenu) return;
    RenderArray();
}

StageState::~StageState()
{
    objectArray.clear();
    colliderArray.clear();
    cout << "Object Array deleted successfully!" << endl;
}
