#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;
	// 並進移動速度
	void SetVelocity(Vector3 vel) { mVelocity = vel; }
	// 回転速度
	void SetRotSpeed(float rotspeed) { mRotSpeed = rotspeed; }

private:
	Vector3 mVelocity;		// 並進移動速度
	float mRotSpeed;		// 回転速度
};