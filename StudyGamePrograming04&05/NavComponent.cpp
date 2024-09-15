#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(class Actor* owner, int updateOrder)
	: MoveComponent(owner, updateOrder)
	, mNextNode(nullptr)
{}

void NavComponent::Update(float deltaTime)
{
	if (mNextNode)
	{
		Vector3 diff = mNextNode->GetPosition() - mOwner->GetPosition();
		float nextspeed = speed;
		if (speed * deltaTime > diff.Length()) { nextspeed = diff.Length() / deltaTime; }
		if (diff.Length() <= speed * deltaTime)
		{
			mNextNode = mNextNode->GetParent();
		}
		mNextDirVector = Vector3::Normalize(diff);
		mNextDirAngle = Math::Atan2(mNextDirVector.y, mNextDirVector.x);
		SetVelocity(nextspeed * mNextDirVector);		
	}

	MoveComponent::Update(deltaTime);
}

void NavComponent::SetStartNode(class Tile* start)
{
	if (start) {
		mNextNode = start->GetParent();
	}
}

