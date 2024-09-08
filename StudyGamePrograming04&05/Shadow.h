#pragma once
#include "Actor.h"
class Shadow : public Actor
{
public:
	Shadow(Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const SDL_Event& event) override;

	class AnimSpriteComponent* GetSprite() { return asc; }
	class CircleComponent* GetCircle() { return cc; }
	float GetSpeed() { return speed; }
	class NavComponent* GetNav() { return nc; }
	
private:
	class AnimSpriteComponent* asc;
	class CircleComponent* cc;
	class NavComponent* nc;	
	float speed;
};

