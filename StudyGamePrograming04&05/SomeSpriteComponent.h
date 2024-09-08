#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>

class SomeSpriteComponent : public SpriteComponent
{
public:
	SomeSpriteComponent(class Actor* owner, int drawOrder);

	void SetSomeTextures(std::vector<std::string> texturefiles);
	void SelectTexture(std::string filename);

	std::vector<std::string> TextureFiles;
};