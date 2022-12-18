#include "../header/Collision.h"
#include <algorithm>
#include <cmath>

bool Collision::IsColliding(Rect& a, Rect& b, float angleOfA, float angleOfB)
{
    Vec2 A[] = { Vec2(a.x, a.y + a.h), Vec2(a.x + a.w, a.y + a.h), Vec2(a.x + a.w, a.y), Vec2(a.x, a.y) };
    Vec2 B[] = { Vec2(b.x, b.y + b.h), Vec2(b.x + b.w, b.y + b.h), Vec2(b.x + b.w, b.y), Vec2(b.x, b.y) };

    for (auto& v : A)
        v = (v - a.GetCenter()).GetRotated(angleOfA) + a.GetCenter();

    for (auto& v : B)
        v = (v - b.GetCenter()).GetRotated(angleOfB) + b.GetCenter();

    Vec2 axes[] = { (A[0] - A[1]).GetNormalized(), (A[1] - A[2]).GetNormalized(), (B[0] - B[1]).GetNormalized(), (B[1] - B[2]).GetNormalized() };

    for (auto& axis : axes)
    {
        float P[4];

        for (int i = 0; i < 4; ++i)
            P[i] = A[i].GetDot(axis);

        float minA = *min_element(P, P + 4);
        float maxA = *max_element(P, P + 4);

        for (int i = 0; i < 4; ++i)
            P[i] = B[i].GetDot(axis);

        float minB = *min_element(P, P + 4);
        float maxB = *max_element(P, P + 4);

        if (maxA < minB || minA > maxB)
            return false;
    }
    return true;
}

Vec2 Collision::GetCollisionNormal(Collider& a, Collider& b)
{
    Rect r1 = a.box;
    Rect r2 = b.box;
    
    Vec2 dist = r1.GetCenter() - r2.GetCenter();
    float width = (r1.w + r2.w) / 2.f;
    float height = (r1.h + r2.h) / 2.f;
    float crossWidth = width * dist.y;
    float crossHeight = height * dist.x;

    if (crossWidth > crossHeight)
        return crossWidth > -crossHeight ? Vec2(0.f, -1.f) : Vec2(1.f, 0.f); // Bottom : Left

    return crossWidth > -crossHeight ? Vec2(-1.f, 0.f) : Vec2(0.f, 1.f); // Right : Top
}

float Collision::GetPenetrationDepth(Collider& a, Collider& b)
{
    // Calculate half extents along x axis for each object
    float a_extent = a.box.w / 2.f;
    float b_extent = b.box.w / 2.f;

    // Calculate distance between boxes
    Vec2 n = b.box.GetCenter() - a.box.GetCenter();

    // Calculate overlap on x axis
    float x_overlap = a_extent + b_extent - abs(n.x);

    // SAT test on x axis
    if (x_overlap <= 0)
        return 0.f;
    
    // Calculate half extents along y axis for each object
    a_extent = a.box.h / 2.f;
    b_extent = b.box.h / 2.f;

    // Calculate overlap on y axis
    float y_overlap = a_extent + b_extent - abs(n.y);

    // SAT test on y axis
    if (y_overlap <= 0 || x_overlap > y_overlap)
        return y_overlap;
    
    return x_overlap;
}

void Collision::SetPositionalCorrection(Collider& a, Collider& b, Vec2 n)
{
    // Only one object is corrected
    float penetration = GetPenetrationDepth(a, b);
    a.correction = n * penetration * -1.0f;
}

void Collision::ResolveCollision(Collider& a, Collider& b)
{
    // Calculate relative velocity
    Vec2 relativeVelocity = b.velocity - a.velocity;

    // Calculate collision normal
    Vec2 normal = GetCollisionNormal(a, b);
    float relVel = normal.GetDot(relativeVelocity);

    // Objects moving in oposing directions
    if (relVel > 0.f)
        return;

    // Correct hitbox overlap
    SetPositionalCorrection(a, b, normal);
    
    if (!a.kinematic && !b.kinematic)
        return;

    // Calculate restitution
    float e = min(a.restitution, b.restitution);

    // Calculate impulse scalar
    float j = -(1.f + e) * relVel / (a.invMass + b.invMass);

    // Apply impulse
    Vec2 impulse = normal * j;

    if (a.kinematic)
        a.ApplyImpulse(impulse * -1.f);
    
    if (b.kinematic)
        b.ApplyImpulse(impulse);
}
