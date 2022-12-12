#include "../header/CameraFollower.h"
#include "../header/Camera.h"
#include <string>

using namespace std;

CameraFollower::CameraFollower(GameObject& go, Vec2 offset) : Component(go)
{
    this->offset = offset;
}

void CameraFollower::Update(float dt)
{
    associated.box.SetVec(Camera::pos + offset);
}

void CameraFollower::Render()
{

}

bool CameraFollower::Is(const char* type)
{
    string str_type = type;
    return str_type == "CameraFollower";
}