#pragma once
#include "Actor.h"
class Brave : public Actor
{
public:
	Brave(Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const SDL_Event& event) override;

	class AnimSpriteComponent* GetSprite() { return asc; }
	class CircleComponent* GetCircle() { return cc; }
	float GetSpeed() { return speed; }

	class SquareComponent* GetSquare() { return sq; }

private:
	class AnimSpriteComponent* asc;
	class CircleComponent* cc;
	class MoveComponent* mc;

	float speed;

	class SquareComponent* sq;
};