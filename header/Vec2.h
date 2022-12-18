#ifndef VEC2_H
#define VEC2_H

#include <cmath>

class Vec2 {
    public:
        Vec2(float x = 0, float y = 0);
        float GetAngle();
        float GetMagnitude();
        float GetAngle(Vec2 terminal);
        float GetDistance(Vec2 vec);
        float GetDot(Vec2 vec);
        Vec2 GetRotated(float angle);
        Vec2 GetNormalized();
        Vec2 operator + (const Vec2 &obj);
        Vec2 operator - (const Vec2 &obj);
        Vec2 operator * (const float obj);
        Vec2 operator / (const float obj);
        float x;
        float y;
};

#endif // VEC2_H