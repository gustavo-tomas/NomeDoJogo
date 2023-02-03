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
    objectArray.push_back(ptr);

    auto weakPtr =  weak_ptr<GameObject>(ptr);

    go->SetLayer(layer);
    renderArray.insert({layer, weakPtr});

    if (go->GetComponent("Collider") != nullptr)
        AddColliderObject(weakPtr);
    
    return weakPtr;
}

weak_ptr<GameObject> State::GetObjectPtr(GameObject* go)
{
    for (auto& obj : objectArray)
        if (obj.get() == go)
            return weak_ptr<GameObject>(obj);

    return {};
}

void State::UpdateLayer(GameObject *go, uint32_t newLayer){
    uint32_t layer = go->GetLayer();
    auto obj = GetObjectPtr(go);
    renderArray.erase({layer, obj});
    renderArray.insert({newLayer, obj});
}

void State::StartArray()
{
    for(uint32_t i = 0; i < objectArray.size(); i++)
        objectArray[i]->Start();
}

void State::UpdateArray(float dt)
{
    for(uint32_t i = 0; i < objectArray.size(); i++)
        objectArray[i]->Update(dt);
}

void State::RenderArray()
{
    for(auto &obj: renderArray)
        if(!obj.second.expired())
            obj.second.lock()->Render();
}

bool State::PopRequested()
{
    return popRequested;
}

bool State::QuitRequested()
{
    return quitRequested;
}