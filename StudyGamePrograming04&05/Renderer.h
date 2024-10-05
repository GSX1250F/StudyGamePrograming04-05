#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL.h>
#include "Math.h"
#include "Texture.h"
#include "VertexInfo.h"

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void UnloadData();
	void Shutdown();

	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	class Texture* GetTexture(const std::string& fileName);
	class VertexInfo* GetVertexInfo() { return mVertexInfo; }

	void SetViewMatrix(const Matrix4& matrix) { mView = matrix; }
	void SetProjMatrix(const Matrix4& matrix) { mProj = matrix; }

	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }

private:
	// テクスチャのマップ
	std::unordered_map<std::string, class Texture*> mTextures;

	// スプライトコンポーネントの配列
	std::vector<class SpriteComponent*> mSprites;

	// Game
	class Game* mGame;

	// Width/height of screen
	float mScreenWidth;
	float mScreenHeight;

	// SDL_Window
	SDL_Window* mWindow;
	// SDL Renderer
	SDL_Renderer* mRenderer;
	// OpenGL context
	SDL_GLContext mContext;

	// バーテックス配列オブジェクト
	class VertexInfo* mVertexInfo;
	// シェーダー
	class Shader* mShader;
	// ビュー変換と射影変換行列
	Matrix4 mView;
	Matrix4 mProj;

	void CreateVertexInfo();
	bool LoadShaders();
};