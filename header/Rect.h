#ifndef RECT_H
#define RECT_H

#include "Vec2.h"
#include <vector>

using namespace std;

class Rect {
    public:
        Rect(float x = 0, float y = 0, float w = 0, float h = 0);
        float x;
        float y;
        float w;
        float h;
        bool Contains(vector<float> point);
        bool Contains(Vec2 point);
        Vec2 GetVec();
        Vec2 GetCenter();
        void SetCenter(Vec2 vec);
        void SetVec(Vec2 vec);
};


#endif // RECT_H