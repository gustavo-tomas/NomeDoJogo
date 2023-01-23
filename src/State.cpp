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

void State::AddColliderObject(weak_ptr<GameObject>& object)
{

}

weak_ptr<GameObject> State::AddObject(GameObject* go, uint32_t layer)
{
    auto ptr = shared_ptr<GameObject>(go);
    if (layer < objectArray.size())
    {
        objectArray[layer].push_back(ptr);
        if (started)
            go->Start();
    }

    else if (layer == objectArray.size()) 
    {
        objectArray.push_back({});
        objectArray[layer].push_back(ptr);
        if (started)
            go->Start();
    }

    else
    {
        cerr << "ERROR ADDING OBJECT!!!!\n";
        exit(1);
    }

    auto weakPtr =  weak_ptr<GameObject>(ptr);
    if (go->GetComponent("Collider") != nullptr)
        AddColliderObject(weakPtr);
    
    return weakPtr;
}

weak_ptr<GameObject> State::GetObjectPtr(GameObject* go)
{
    for (auto& layer : objectArray)
        for (auto& obj : layer)
            if (obj.get() == go)
                return weak_ptr<GameObject>(obj);

    return {};
}

void State::StartArray()
{
    for (uint32_t i = 0; i < objectArray.size(); i++) // layer 
        for (uint32_t j = 0; j < objectArray[i].size(); j++) // obj array
            objectArray[i][j]->Start();
}

void State::UpdateArray(float dt)
{
    for (uint32_t i = 0; i < objectArray.size(); i++) // layer 
        for (uint32_t j = 0; j < objectArray[i].size(); j++) // obj array
            objectArray[i][j]->Update(dt);
}

void State::RenderArray()
{
    for (uint32_t i = 0; i < objectArray.size(); i++) // layer 
        for (uint32_t j = 0; j < objectArray[i].size(); j++) // obj array
            objectArray[i][j]->Render();
}

bool State::PopRequested()
{
    return popRequested;
}

bool State::QuitRequested()
{
    return quitRequested;
}