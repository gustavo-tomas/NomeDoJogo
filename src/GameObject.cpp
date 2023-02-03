#include "../header/GameObject.h"
#include <memory>

GameObject::GameObject()
{
    isDead = false;
    started = false;
    angleDeg = 0;
}

void GameObject::Start()
{
    for (auto& cmp : components)
        cmp->Start();
    started = true;
}

GameObject::~GameObject()
{
    components.clear();
    cout << "Game Object deleted successfully!" << endl;
}

void GameObject::Update(float dt)
{
    for (auto& cpt : components)
        if (cpt != nullptr)
            cpt->Update(dt);
}

void GameObject::Render()
{
    for (auto& cpt : components)
        if (cpt != nullptr)
            cpt->Render();
}

bool GameObject::IsDead()
{
    return isDead;
}

void GameObject::RequestDelete()
{
    isDead = true;
}

void GameObject::AddComponent(Component* cpt)
{
    components.emplace_back(cpt);
    if (started)
        cpt->Start();
}

void GameObject::RemoveComponent(Component* cpt)
{
    components.erase(
        remove_if(components.begin(), components.end(),
            [&](unique_ptr<Component> &component) { return component.get() == cpt; }
        ),
        components.end()
    );
}

Component* GameObject::GetComponent(const char* type)
{
    for (auto& cpt : components)
        if (cpt->Is(type))
            return cpt.get();

    return nullptr;
}

void GameObject::NotifyCollision(GameObject& other)
{
    for (auto& cpt : components)
        cpt->NotifyCollision(other);
}

void GameObject::SetLayer(uint32_t layer){
    this->layer = layer;
}

uint32_t GameObject::GetLayer(){
    return layer;
}
