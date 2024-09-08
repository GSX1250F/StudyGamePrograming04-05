#pragma once
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	void SetActive();

	int GetTexWidth() const { return mTexWidth; }
	int GetTexHeight() const { return mTexHeight; }

private:
	// このテクスチャのOpenGL ID
	unsigned int mTextureID;

	// テクスチャの幅と高さ
	int mTexWidth;
	int mTexHeight;
};