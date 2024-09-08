#pragma once
#include "Actor.h"
class Treasure : public Actor
{
public:
	Treasure(class Game* game);

	class CircleComponent* GetCircle() { return cc; }
	class SpriteComponent* GetSprite() { return sc; }
	

private:
	class SpriteComponent* sc;
	class CircleComponent* cc;
};

