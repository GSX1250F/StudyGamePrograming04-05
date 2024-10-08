#include "Renderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include <glew.h>
#include "SpriteComponent.h"
#include "Shader.h"
#include "VertexInfo.h"
#include "Texture.h"

Renderer::Renderer(Game* game)
	: mGame(game)
	, mWindow(nullptr)
	, mShader(nullptr)
	, mVertexInfo(nullptr)
{}

Renderer::~Renderer()
{}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	// OpenGL初期設定
	// OpenGLのコアプロファイルを使用
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// version 3.3を指定
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// 各カラーチャンネル8ビット(RGBA)
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// ダブルバッファ有効化
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// OpenGLのハードウェアアクセラレータ使用
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// SDLウィンドウを作成
	mWindow = SDL_CreateWindow(
		"Game Programming in C++",
		100,
		100,
		static_cast<int>(screenWidth),
		static_cast<int>(screenHeight),
		SDL_WINDOW_OPENGL					//SDLウィンドウでOpenGLを使用する際に指定
	);

	if (!mWindow)
	{
		SDL_Log("ウィンドウの作成に失敗しました: %s", SDL_GetError());
		return false;
	}
	
	// OpenGLコンテクストを生成（すべてのOpenGL機能にアクセスする）
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEWを初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEWの初期化に失敗しました。");
		return false;
	}
	glGetError();

	// バーテックス配列オブジェクトの生成
	CreateVertexInfo();

	// シェーダーの生成
	if (!LoadShaders())
	{
		SDL_Log("シェーダーの読み込みに失敗しました。");
		return false;
	}

	// 画面クリアの色を設定
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	return true;
}

void Renderer::UnloadData()
{
	mTextures.clear();
}

void Renderer::Shutdown()
{
	delete mVertexInfo;
	mShader->Unload();
	delete mShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}


void Renderer::Draw()
{
	// 画面をクリア & 深度バッファをクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// カラーバッファのアルファブレンディングを有効化
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,				// srcFactorはsrcAlpha
		GL_ONE_MINUS_SRC_ALPHA		// dstFactorは(1-srcAlpha)
	);
	//深度有効化
	glEnable(GL_DEPTH_TEST);

	// シェーダーとバーテックス配列オブジェクトを有効化
	mVertexInfo->SetActive();
	mShader->SetActive();
	
	// シェーダーのビュー変換&射影変換行列を更新
	mShader->SetMatrixUniform("uViewProj", mView * mProj);

	for (auto sprite : mSprites)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(mShader);
		}
	}
	// ダブルバッファを交換
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

class Texture* Renderer::GetTexture(const std::string& filename)
{
	class Texture* tex = nullptr;
	auto iter = mTextures.find(filename);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(filename))
		{
			mTextures.emplace(filename, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

void Renderer::CreateVertexInfo()
{
	//左手座標系	
	//頂点座標(vector3)
	int numVerts = 16;			//頂点の数
	float vertPos[] = {
		-0.5f, -0.5f, -0.5f,	//front left upper
		-0.5f, -0.5f,  0.5f,	//front left lower
		 0.5f, -0.5f,  0.5f,	//front right lower
		 0.5f, -0.5f, -0.5f,	//front right upper
		 0.5f, -0.5f, -0.5f,	//right left upper
		 0.5f, -0.5f,  0.5f,	//right left lower
		 0.5f,  0.5f,  0.5f,	//right right lower
		 0.5f,  0.5f, -0.5f,	//right right upper
		 0.5f,  0.5f, -0.5f,	//back left upper
		 0.5f,  0.5f,  0.5f,	//back left lower
		-0.5f,  0.5f,  0.5f,	//back right lower
		-0.5f,  0.5f, -0.5f,	//back right upper
		-0.5f,  0.5f, -0.5f,	//left left upper
		-0.5f,  0.5f,  0.5f,	//left left lower
		-0.5f, -0.5f,  0.5f,	//left right lower
		-0.5f, -0.5f, -0.5f		//left right upper		
	};
	
	//テクスチャ座標(vector2)	※Clear画像のためにわざと左右も反転
	float texCoord[] = {
		 1.0f, 0.0f,
		 1.0f, 1.0f,
		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f,
		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f,
		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f,
		 0.0f, 1.0f,
		 0.0f, 0.0f
	};
	//頂点カラー(vector4 RGBA)
	float vertColor[] = {
		 0.5f, 0.5f, 1.0f, 1.0f,	//頂点0=頂点15
		 0.5f, 0.5f, 0.5f, 1.0f,	//頂点1=頂点14
		 1.0f, 0.5f, 0.5f, 1.0f,	//頂点2=頂点5
		 1.0f, 0.5f, 1.0f, 1.0f,	//頂点3=頂点4
		 1.0f, 0.5f, 1.0f, 1.0f,	//頂点4=頂点3
		 1.0f, 0.5f, 0.5f, 1.0f,	//頂点5=頂点2
		 1.0f, 1.0f, 0.5f, 1.0f,	//頂点6=頂点9
		 1.0f, 1.0f, 1.0f, 1.0f,	//頂点7=頂点8
		 1.0f, 1.0f, 1.0f, 1.0f,	//頂点8=頂点7
		 1.0f, 1.0f, 0.5f, 1.0f,	//頂点9=頂点6
		 0.5f, 1.0f, 0.5f, 1.0f,	//頂点10=頂点13
		 0.5f, 1.0f, 1.0f, 1.0f,	//頂点11=頂点12
		 0.5f, 1.0f, 1.0f, 1.0f,	//頂点12=頂点11
		 0.5f, 1.0f, 0.5f, 1.0f,	//頂点13=頂点10
		 0.5f, 0.5f, 0.5f, 1.0f,	//頂点14=頂点1
		 0.5f, 0.5f, 1.0f, 1.0f,	//頂点15=頂点0
	};
	//インデックス
	int numIndices = 24;			//インデックスの数
	unsigned int indices[] = {
		 0, 1, 2,
		 2, 3, 0,
		 4, 5, 6,
		 6, 7, 4,
		 8, 9, 10,
		 10, 11, 8,
		 12, 13, 14,
		 14, 15, 12,
	};

	mVertexInfo = new VertexInfo(vertPos, texCoord, vertColor, indices, numVerts, numIndices);
}

bool Renderer::LoadShaders()
{
	// シェーダーを生成
	mShader = new Shader();
	if (!mShader->Load("Shaders/shader.vert", "Shaders/shader.frag"))
	{
		return false;
	}
	mShader->SetActive();
	// ビュー変換と射影変換行列を作成。
	mView = Matrix4::Identity;
	mProj = Matrix4::CreatePerspectiveFOV(Math::Pi*0.5, mScreenWidth, mScreenHeight, 0.01f, 5000.0f);
	//mProj = Matrix4::CreateOrtho(mScreenWidth, mScreenHeight, 0.01f, 5000.0f);
	mShader->SetMatrixUniform("uViewProj", mView * mProj);

	return true;
}