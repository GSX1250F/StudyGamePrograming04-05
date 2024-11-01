#include "Floor.h"
#include "Game.h"
#include "Renderer.h"
#include "SpriteComponent.h"
#include "Math.h"

Floor::Floor(Game* game)
	: Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this, 100);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Floor.png"));
	Quaternion q = Quaternion(Vector3::UnitX, Math::PiOver2);
	SetRotation(q);
}
