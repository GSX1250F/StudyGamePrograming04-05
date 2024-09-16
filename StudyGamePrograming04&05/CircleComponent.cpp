#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(Actor* owner) : Component(owner)
{}

float CircleComponent::GetRadius() const
{
	return mOwner->GetRadius();
}

const Vector3& CircleComponent::GetCenter() const
{
	return  mOwner->GetPosition();
}

bool Intersect(const CircleComponent& a, const CircleComponent& b)
{
	Vector2 a_center = Vector2(a.GetCenter().x, a.GetCenter().y);
	Vector2 b_center = Vector2(b.GetCenter().x, b.GetCenter().y);

	// ‚Q‚Â‚ÌCircleComponent‚Ì’†SŠÔ‹——£‚Ì2æ‚ğŒvZ
	float distSq = (a_center - b_center).LengthSq();

	// ‚Q‚Â‚ÌCircleComponent‚Ì”¼Œa‚Ì˜a‚Ì2æ‚ğŒvZ 
	float sumRadiusSq = (a.GetRadius() + b.GetRadius()) * (a.GetRadius() + b.GetRadius());

	// ’†SŠÔ‹——£ <= ”¼Œa‚Ì˜a ‚Ì‚Æ‚«AÕ“Ë‚µ‚½‚Æ”»’è
	if (distSq <= sumRadiusSq)
	{
		return true;
	}
	else
	{
		return false;
	}
}
