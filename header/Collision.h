#include "Collider.h"
#include "Rect.h"
#include "Vec2.h"

class Collision {
	public:
		static bool IsColliding(Rect& a, Rect& b, float angleOfA, float angleOfB);
		static Vec2 GetCollisionNormal(Collider& a, Collider& b);
		static float GetPenetrationDepth(Collider& a, Collider& b);
		static void SetPositionalCorrection(Collider& a, Collider& b, Vec2 n);
		static void ResolveCollision(Collider& a, Collider& b);
};
