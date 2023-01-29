#include "../header/Collider.h"

// Debug 
#include "../header/Camera.h"
#include "../header/Game.h"
#include "../header/InputManager.h"

bool Collider::debug = true;
Timer Collider::debugTimer;

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset, bool activeCollison) : Component(associated)
{
    this->scale = scale;
    this->offset = offset;
    this->activeCollison = activeCollison;
    this->box = associated.box;
}

void Collider::Start()
{
    // GetRotated is not used in this particular case
    box.SetVec(associated.box.GetVec() + offset.GetRotated(associated.angleDeg));
    UpdatePosition();
}

void Collider::Update(float dt)
{
    // Debug is toggled ON/OFF by pressing Tab :)
    debugTimer.Update(dt);
    if (InputManager::GetInstance().KeyPress(TAB_KEY) && debugTimer.Get() >= 0.4)
    {
        debug = !debug;
        debugTimer.Restart();
    }

    box.SetVec(box.GetVec() + velocity * dt);
    associated.box.SetVec(Vec2(
        box.x + (box.w - associated.box.w) / 2.0 - offset.x,
        box.y + (box.h - associated.box.h) / 2.0 - offset.y
    ));
}

void Collider::UpdatePosition()
{
    box.w = associated.box.w * scale.x;
    box.h = associated.box.h * scale.y;
    
    box.SetVec(Vec2(
        box.x = associated.box.x + (associated.box.w - box.w) / 2.0 + offset.x,
        box.y = associated.box.y + (associated.box.h - box.h) / 2.0 + offset.y
    ));
}

void Collider::ResolveCollisionUpdate(float dt)
{
    if (!activeCollison) return;
    
    // Apply correction (if any) and update the associated box
    box.SetVec(box.GetVec() + correction);
    associated.box.SetVec(Vec2(
        box.x + (box.w - associated.box.w) / 2.0 - offset.x,
        box.y + (box.h - associated.box.h) / 2.0 - offset.y
    ));
    correction = Vec2(0, 0);
}

void Collider::Render() {

    if (!debug)
        return;

    Vec2 center(box.GetCenter());
    SDL_Point points[5];

    Vec2 point = (Vec2(box.x, box.y) - center).GetRotated(associated.angleDeg) + center - Camera::pos;
    points[0] = { (int) point.x, (int) point.y };
    points[4] = { (int) point.x, (int) point.y };
    
    point = (Vec2(box.x + box.w, box.y) - center).GetRotated(associated.angleDeg) + center - Camera::pos;
    points[1] = { (int) point.x, (int) point.y };
    
    point = (Vec2(box.x + box.w, box.y + box.h) - center).GetRotated(associated.angleDeg) + center - Camera::pos;
    points[2] = { (int) point.x, (int) point.y };
    
    point = (Vec2(box.x, box.y + box.h) - center).GetRotated( associated.angleDeg) + center - Camera::pos;
    points[3] = { (int) point.x, (int) point.y };

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
}

bool Collider::Is(const char* type)
{
    string str_type = type;
    return str_type == "Collider";
}

void Collider::ApplyImpulse(Vec2 impulse)
{
    float inv = 1.f / this->mass;
    this->velocity = this->velocity + (impulse * inv);
}

void Collider::SetScale(Vec2 scale)
{
    this->scale = scale;
    UpdatePosition();
}

void Collider::SetOffset(Vec2 offset)
{
    this->offset = offset;
    UpdatePosition();
}

void Collider::SetVelocity(Vec2 velocity)
{
    this->velocity = velocity;
}

void Collider::SetRestitution(float restitution)
{
    this->restitution = restitution;
}

void Collider::SetMass(float mass)
{
    this->mass = mass;
    this->invMass = 1.f / mass;
}
