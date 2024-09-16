#include "SquareComponent.h"
#include "Actor.h"
#include "Math.h"

SquareComponent::SquareComponent(Actor* owner) : Component(owner)
{}

const Vector2& SquareComponent::GetCenter() const
{
	return  mOwner->GetPosition();
}

float SquareComponent::GetRadius() const
{
	return mOwner->GetRadius();
}

bool Intersect(const SquareComponent& a, const SquareComponent& b)
{
	float a_minX = a.GetCenter().x - a.GetRadius();
	float a_maxX = a.GetCenter().x + a.GetRadius();
	float a_minY = a.GetCenter().y - a.GetRadius();
	float a_maxY = a.GetCenter().y + a.GetRadius();
	float b_minX = b.GetCenter().x - b.GetRadius();
	float b_maxX = b.GetCenter().x + b.GetRadius();
	float b_minY = b.GetCenter().y - b.GetRadius();
	float b_maxY = b.GetCenter().y + b.GetRadius();

	if ( a_maxX < b_minX ||
		 a_minX > b_maxX || 
		 a_maxY < b_minY ||
		 a_minY > b_maxY
		)
	{
		return false;
	}
	else
	{
		return true;
	}
}
