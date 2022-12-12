#include "../header/State.h"

State::State()
{
    popRequested = false;
    quitRequested = false;
    started = false;
}

State::~State()
{
    objectArray.clear();
}

void State::LoadAssets()
{

}

void State::Start()
{

}

void State::Pause()
{

}

void State::Resume()
{

}

void State::Update(float dt)
{

}

void State::Render()
{

}

weak_ptr<GameObject> State::AddObject(GameObject* go)
{
    auto ptr = shared_ptr<GameObject>(go);
    objectArray.push_back(ptr);
    if (started)
        go->Start();
    
    return weak_ptr<GameObject>(ptr);
}

weak_ptr<GameObject> State::GetObjectPtr(GameObject* go)
{
    for (auto& obj : objectArray)
        if (obj.get() == go)
            return weak_ptr<GameObject>(obj);

    return weak_ptr<GameObject>();
}

void State::StartArray()
{
    for (unsigned int i = 0; i < objectArray.size(); i++)
        objectArray[i]->Start();
}

void State::UpdateArray(float dt)
{
    for (unsigned int i = 0; i < objectArray.size(); i++)
        objectArray[i]->Update(dt);
}

void State::RenderArray()
{
    for (unsigned int i = 0; i < objectArray.size(); i++)
        objectArray[i]->Render();
}

bool State::PopRequested()
{
    return popRequested;
}

bool State::QuitRequested()
{
    return quitRequested;
}