#pragma once
#include "Actor.h"
class Brave : public Actor
{
public:
	Brave(Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const SDL_Event& event) override;

	class SpriteComponent* GetSprite() { return sc; }
	class CircleComponent* GetCircle() { return cc; }
	float GetSpeed() { return speed; }


private:
	class SpriteComponent* sc;
	class CircleComponent* cc;
	class MoveComponent* mc;

	float speed;
};