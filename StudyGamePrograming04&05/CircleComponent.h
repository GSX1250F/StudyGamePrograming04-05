#pragma once
#include "Component.h"
#include "Math.h"

class CircleComponent : public Component
{
public:
	CircleComponent(class Actor* owner);

	float GetRadius() const;
	const Vector2& GetCenter() const;

};

bool Intersect(const CircleComponent& a, const CircleComponent& b);