#pragma once
#include "Component.h"
#include "Math.h"

class SquareComponent : public Component
{
public:
	SquareComponent(class Actor* owner);

	const Vector2& GetCenter() const;
	float GetRadius() const;
};

bool Intersect(const SquareComponent& a, const SquareComponent& b);