#include "../header/Vec2.h"

Vec2::Vec2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vec2 Vec2::operator + (const Vec2 &obj)
{
    return Vec2(x + obj.x, y + obj.y);
}

Vec2 Vec2::operator - (const Vec2 &obj)
{
    return Vec2(x - obj.x, y - obj.y);
}

Vec2 Vec2::operator * (const float obj)
{
    return Vec2(x * obj, y * obj);
}

Vec2 Vec2::operator / (const float obj)
{
    return Vec2(x / obj, y / obj);
}

Vec2 Vec2::GetRotated(float angle)
{
    float x2 = x * cos(angle) - y * sin(angle);
    float y2 = y * cos(angle) + x * sin(angle);
    return Vec2(x2, y2);
}

float Vec2::GetAngle()
{
    return atan2(y, x);
}

float Vec2::GetAngle(Vec2 terminal)
{
    return atan2((terminal.y - y), (terminal.x - x));
}

float Vec2::GetMagnitude()
{
    return sqrt(x * x + y * y);
}

float Vec2::GetDistance(Vec2 vec)
{
    float dX = x - vec.x;
    float dY = y - vec.y;
    return sqrt(dX * dX + dY * dY); 
}

float Vec2::GetDot(Vec2 vec)
{
    return x * vec.x + y * vec.y;
}

Vec2 Vec2::GetNormal()
{
    return Vec2(x, y) * (1.f / GetMagnitude());
}