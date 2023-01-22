#include "../header/Camera.h"
#include "../header/InputManager.h"
#include "../header/GameData.h"

GameObject* Camera::focus = nullptr;
Vec2 Camera::pos = Vec2(0, 0);
Vec2 Camera::speed = Vec2(0, 0);
bool Camera::movement = false;

void Camera::Follow(GameObject* newFocus)
{
    focus = newFocus;
}

void Camera::Unfollow()
{
    focus = nullptr;
}

void Camera::Update(float dt)
{
    if (focus != nullptr)
        pos = focus->box.GetCenter() - Vec2(GameData::WIDTH / 2.0, GameData::HEIGHT / 2.0);

    if (!movement)
        return;
    
    // X = Xo + vo * dt + (a * dt^2) / 2
    pos = pos + (speed * dt);
    
    float maxSpeed = 500.0;
    float incSpeed = 50.0;
    float decSpeed = 10.0;

    if (InputManager::GetInstance().IsKeyDown(LEFT_ARROW_KEY) || InputManager::GetInstance().IsKeyDown(A_KEY))
        speed.x = speed.x > -maxSpeed ? speed.x - incSpeed : -maxSpeed;

    if (InputManager::GetInstance().IsKeyDown(RIGHT_ARROW_KEY) || InputManager::GetInstance().IsKeyDown(D_KEY))
        speed.x = speed.x < +maxSpeed ? speed.x + incSpeed : +maxSpeed;

    if (!InputManager::GetInstance().IsKeyDown(LEFT_ARROW_KEY) && !InputManager::GetInstance().IsKeyDown(A_KEY) &&
        !InputManager::GetInstance().IsKeyDown(RIGHT_ARROW_KEY) && !InputManager::GetInstance().IsKeyDown(D_KEY))
    {
        speed.x = speed.x > 0 ? speed.x - decSpeed : 
                    speed.x < 0 ? speed.x + decSpeed :
                    0;
    }

    if (InputManager::GetInstance().IsKeyDown(UP_ARROW_KEY) || InputManager::GetInstance().IsKeyDown(W_KEY))
        speed.y = speed.y > -maxSpeed ? speed.y - incSpeed : -maxSpeed;
    
    if (InputManager::GetInstance().IsKeyDown(DOWN_ARROW_KEY) || InputManager::GetInstance().IsKeyDown(S_KEY))
        speed.y = speed.y < +maxSpeed ? speed.y + incSpeed : +maxSpeed;

    if (!InputManager::GetInstance().IsKeyDown(UP_ARROW_KEY) && !InputManager::GetInstance().IsKeyDown(W_KEY) &&
        !InputManager::GetInstance().IsKeyDown(DOWN_ARROW_KEY) && !InputManager::GetInstance().IsKeyDown(S_KEY))
    {
        speed.y = speed.y > 0 ? speed.y - decSpeed : 
                    speed.y < 0 ? speed.y + decSpeed :
                    0;
    }
}