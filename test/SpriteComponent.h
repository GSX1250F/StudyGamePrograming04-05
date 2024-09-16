#pragma once
#include "Component.h"
#include <SDL.h>

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	//virtual void Draw(SDL_Renderer* renderer);		// ïœçX
	virtual void Draw(class Shader* shader);
	
	virtual void SetTexture(class Texture* texture);
	class Texture* GetTexture() const { return mTexture; }

	int GetDrawOrder() const { return mDrawOrder; }
	void SetTexWidth(int width) { mTexWidth = width; }
	int GetTexWidth() const { return mTexWidth; }
	void SetTexHeight(int height) { mTexHeight = height; }
	int GetTexHeight() const { return mTexHeight; }
	void SetVisible(bool value) { mVisible = value; }
	bool GetVisible() const { return mVisible; }

private:
	class Texture* mTexture;
	int mDrawOrder;
	int mTexWidth;
	int mTexHeight;
	bool mVisible;
};