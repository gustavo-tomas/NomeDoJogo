#include "../header/Rect.h"

Rect::Rect(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

bool Rect::Contains(vector<float> point)
{
    float pointX = point[0];
    float pointY = point[1];
    
    if (pointX >= x && pointX <= x + w)
        if (pointY >= y && pointY <= y + h)
            return true;
    
    return false;
}

bool Rect::Contains(Vec2 point)
{
    if (point.x >= x && point.x <= x + w)
        if (point.y >= y && point.y <= y + h)
            return true;
    
    return false;
}

Vec2 Rect::GetVec()
{
    return Vec2(x, y);
}

void Rect::SetVec(Vec2 vec)
{
    x = vec.x;
    y = vec.y;
}

Vec2 Rect::GetCenter()
{
    return Vec2(x + w / 2.0, y + h / 2.0);
}