#include "../header/State.h"
#include "../header/GameData.h"
#include <algorithm>

State::State()
{
    popRequested = false;
    quitRequested = false;
    started = false;
    currResolution = Vec2(GameData::WIDTH, GameData::HEIGHT);
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
    colliderArray.emplace_back(object);
}

weak_ptr<GameObject> State::AddObject(GameObject* go, int32_t layer)
{
    auto ptr = shared_ptr<GameObject>(go);
    go->SetLayer(layer);
    objectArray.push_back(ptr);

    auto weakPtr =  weak_ptr<GameObject>(ptr);

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

void State::StartArray()
{
    for (uint32_t i = 0; i < objectArray.size(); i++)
        objectArray[i]->Start();
}

void State::UpdateArray(float dt)
{
    for (uint32_t i = 0; i < objectArray.size(); i++)
        objectArray[i]->Update(dt);
}

void State::RenderArray()
{
    std::sort(objectArray.begin(), objectArray.end(), [](shared_ptr<GameObject> &a, shared_ptr<GameObject> &b)
    {
        return a->GetLayer() + a->box.y + a->box.h < b->GetLayer() + b->box.y + b->box.h;
    });
    for (auto &obj: objectArray)
        obj->Render();
}

bool State::PopRequested()
{
    return popRequested;
}

bool State::QuitRequested()
{
    return quitRequested;
}
