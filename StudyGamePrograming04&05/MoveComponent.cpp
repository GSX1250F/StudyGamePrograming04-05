#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	: Component(owner)
	, mVelocity(Vector3::Zero)		// 並進移動速度
	, mRotSpeed(0.0f)				// 回転速度
{}

void MoveComponent::Update(float deltatime)
{
	// 位置と向きを更新
	mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltatime);		//x = xo + vt
	mOwner->SetRotation(mOwner->GetRotation() + mRotSpeed * deltatime);		//Θ = Θo + ωt
}