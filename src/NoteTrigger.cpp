#include "../header/NoteTrigger.h"
#include "../header/Collider.h"
#include "../header/Note.h"
#include "../header/SpriteRect.h"


NoteTrigger::NoteTrigger(GameObject& associated) : Component(associated)
{
    associated.box.h = associated.box.w = 20;

    Collider *collider = new Collider(associated);   
    associated.AddComponent(collider);

    // SpriteRect* sr = new SpriteRect(associated, 0xFF3333FF ^ random(), 20, 20);
    // associated.AddComponent(sr);
}

NoteTrigger::~NoteTrigger()
{
    
}

void NoteTrigger::Update(float dt)
{
    
}

void NoteTrigger::Render()
{
    
}

bool NoteTrigger::Is(const char* type)
{
    string str_type = type;
    return str_type == "NoteTrigger";
}

void NoteTrigger::NotifyCollision(GameObject& other)
{
    auto component = other.GetComponent("Note");
    if(component != nullptr){
        Note* noteComponent = (Note*)component;
        auto auxValue = min(abs((associated.box.x + associated.box.w) - other.box.x), abs((other.box.x + other.box.w) - associated.box.x));
        auto percentage = ceil(auxValue / 5) * 5;
        cout << "auxValue = " << auxValue << "\n";
        cout << "percentage = " << percentage << "\n";
    }
}

