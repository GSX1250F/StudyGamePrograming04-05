#pragma once
#include "MoveComponent.h"
#include "Math.h"

class NavComponent : public MoveComponent
{
public:
	NavComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;

	void SetSpeed(float value) { speed = value; }
	void SetStartNode(class Tile* start);
	class Tile* GetNextNode() { return mNextNode; }
	Vector3 GetNextDirVector() { return mNextDirVector; }
	float GetNextDirAngle() { return mNextDirAngle; }


private:
	class Tile* mNextNode;
	float speed;
	float mNextDirAngle;
	Vector3 mNextDirVector;
};