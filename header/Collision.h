#include "Collider.h"
#include "Rect.h"
#include "Vec2.h"

#include <algorithm>
#include <cmath>

class Collision {
	public:
		static inline bool IsColliding(Rect& a, Rect& b, float angleOfA, float angleOfB)
		{
			Vec2 A[] = { Vec2(a.x, a.y + a.h), Vec2(a.x + a.w, a.y + a.h), Vec2(a.x + a.w, a.y), Vec2(a.x, a.y) };
			Vec2 B[] = { Vec2(b.x, b.y + b.h), Vec2(b.x + b.w, b.y + b.h), Vec2(b.x + b.w, b.y), Vec2(b.x, b.y) };

			for (auto& v : A)
				v = (v - a.GetCenter()).GetRotated(angleOfA) + a.GetCenter();

			for (auto& v : B)
				v = (v - b.GetCenter()).GetRotated(angleOfB) + b.GetCenter();

			Vec2 axes[] = { (A[0] - A[1]).GetNormal(), (A[1] - A[2]).GetNormal(), (B[0] - B[1]).GetNormal(), (B[1] - B[2]).GetNormal() };

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
		
		static inline Vec2 GetCollisionNormal(Collider& a, Collider& b)
		{
			Rect r1 = a.box;
			Rect r2 = b.box;
			
			Vec2 dist = r1.GetCenter() - r2.GetCenter();
			float width = (r1.w + r2.w) / 2.f;
			float height = (r1.h + r2.h) / 2.f;
			float crossWidth = width * dist.y;
			float crossHeight = height * dist.x;

			if (crossWidth > crossHeight)
				return crossWidth > -crossHeight ? Vec2(0.f, -1.f) : Vec2(-1.f, 0.f); // Bottom : Left

			return crossWidth > -crossHeight ? Vec2(1.f, 0.f) : Vec2(0.f, 1.f); // Right : Top
		}

		static inline void ResolveCollision(Collider& a, Collider& b)
		{
			// Calculate relative velocity
			Vec2 relativeVelocity = b.velocity - a.velocity;

			// Calculate collision normal
			Vec2 normal = GetCollisionNormal(a, b);

			// Calculate restitution
			float e = min(a.restitution, b.restitution);

			// Calculate invert mass
			float invA = 1.f / a.mass;
			float invB = 1.f / b.mass;

			// Calculate impulse scalar
			float j = -(1.f + e) * normal.GetDot(relativeVelocity) / (invA + invB);

			// Apply impulse
			Vec2 impulse = normal * j * 1.2f;

			a.velocity = a.velocity - (impulse * invA);
			b.velocity = b.velocity + (impulse * invB);
		}
};
