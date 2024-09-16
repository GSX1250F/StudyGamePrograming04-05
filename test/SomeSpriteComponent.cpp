#include "SomeSpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

SomeSpriteComponent::SomeSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
{}

void SomeSpriteComponent::SetSomeTextures(std::vector<std::string> texturefiles)
{
	for (auto file : texturefiles)
	{
		SetTexture(mOwner->GetGame()->GetRenderer()->GetTexture(file));
	}
	
}

void SomeSpriteComponent::SelectTexture(std::string filename)
{
	SetTexture(mOwner->GetGame()->GetRenderer()->GetTexture(filename));
}