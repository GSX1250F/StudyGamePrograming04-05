#include "Renderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include "SpriteComponent.h"
#include <glew.h>
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
	// OpenGL�����ݒ�
	// OpenGL�̃R�A�v���t�@�C�����g�p
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// version 3.3���w��
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// �e�J���[�`�����l��8�r�b�g(RGBA)
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// �_�u���o�b�t�@�L����
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// OpenGL�̃n�[�h�E�F�A�A�N�Z�����[�^�g�p
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// SDL�E�B���h�E���쐬
	mWindow = SDL_CreateWindow(
		"Game Programming in C++",
		100,
		100,
		static_cast<int>(screenWidth),
		static_cast<int>(screenHeight),
		SDL_WINDOW_OPENGL					//SDL�E�B���h�E��OpenGL���g�p����ۂɎw��
	);

	if (!mWindow)
	{
		SDL_Log("�E�B���h�E�̍쐬�Ɏ��s���܂���: %s", SDL_GetError());
		return false;
	}
	
	// OpenGL�R���e�N�X�g�𐶐��i���ׂĂ�OpenGL�@�\�ɃA�N�Z�X����j
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW��������
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEW�̏������Ɏ��s���܂����B");
		return false;
	}
	glGetError();

	// �o�[�e�b�N�X�z��I�u�W�F�N�g�̐���
	CreateVertexInfo();

	// �V�F�[�_�[�̐���
	if (!LoadShaders())
	{
		SDL_Log("�V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂����B");
		return false;
	}


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
	// �w�i�F���w�肵�ĉ�ʂ��N���A & �[�x�o�b�t�@���N���A
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �J���[�o�b�t�@�̃A���t�@�u�����f�B���O��L����
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,				// srcFactor��srcAlpha
		GL_ONE_MINUS_SRC_ALPHA		// dstFactor��(1-srcAlpha)
	);
	//�[�x�L����
	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(true);

	// �V�F�[�_�[�ƃo�[�e�b�N�X�z��I�u�W�F�N�g��L����
	mVertexInfo->SetActive();
	mShader->SetActive();
	for (auto sprite : mSprites)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(mShader);
		}
	}
	// �_�u���o�b�t�@������
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
	//���_���W(vector3)
	float vertPos[] = {
		-0.5f, -0.5f,  0.5f,	//top left lower
		 0.5f, -0.5f,  0.5f,	//top right lower
		-0.5f,  0.5f,  0.5f,	//top left upper
		 0.5f,  0.5f,  0.5f,	//top rght upper
		 /*
		-0.5f, -0.5f, -0.5f,	//front left lower
		 0.5f, -0.5f, -0.5f,	//front right lower
		-0.5f, -0.5f,  0.5f,	//front left upper
		 0.5f, -0.5f,  0.5f,	//front right upper
		 0.5f, -0.5f, -0.5f,	//right left lower
		 0.5f,  0.5f, -0.5f,	//right right lower
		 0.5f, -0.5f,  0.5f,	//right left upper
		 0.5f,  0.5f,  0.5f,	//right right upper
		-0.5f,  0.5f, -0.5f,	//left left lower
		-0.5f, -0.5f, -0.5f,	//left right lower
		-0.5f,  0.5f,  0.5f,	//left left upper
		-0.5f, -0.5f,  0.5f,	//left right upper
		 0.5f,  0.5f, -0.5f,	//back left lower
		-0.5f,  0.5f, -0.5f,	//back right lower
		 0.5f,  0.5f,  0.5f,	//back left upper
		-0.5f,  0.5f,  0.5f		//back right upper
		*/
	};
	//�e�N�X�`�����W(vector2)
	float texCoord[] = {
		 0.0f, 1.0f,
		 1.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 /*
		 0.0f, 1.0f,
		 1.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 0.0f, 1.0f,
		 1.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 0.0f, 1.0f,
		 1.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 0.0f, 1.0f,
		 1.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f
		 */
	};
	//���_�J���[(vector4 RGBA)
	float vertColor[] = {
		 1.0f, 0.0f, 0.0f, 1.0f,
		 0.0f, 1.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f,
		 /*
		 1.0f, 0.0f, 0.0f, 1.0f,
		 0.0f, 1.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 0.0f, 0.0f, 1.0f,
		 0.0f, 1.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 0.0f, 0.0f, 1.0f,
		 0.0f, 1.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 0.0f, 0.0f, 1.0f,
		 0.0f, 1.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f
		 */
	};

	//�C���f�b�N�X
	unsigned int indices[] = {
		 0, 1, 2,
		 2, 1, 3,
		 /*
		 4, 5, 6,
		 6, 5, 7,
		 8, 9, 10,
		 10, 9, 11,
		 12, 13, 14,
		 14, 13, 15,
		 16, 17, 18,
		 18, 17, 19
		 */
	};

	mVertexInfo = new VertexInfo(vertPos, texCoord, vertColor, indices, 4, 6);
}

bool Renderer::LoadShaders()
{
	// �V�F�[�_�[�𐶐�
	mShader = new Shader();
	if (!mShader->Load("Shaders/shader.vert", "Shaders/shader.frag"))
	{
		return false;
	}
	mShader->SetActive();
	// �r���[�ϊ��s����쐬�B
	
	// ���s���e�̏ꍇ
	//Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	Matrix4 viewProj = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 2000.0f)) * Matrix4::CreateOrtho(mScreenWidth, mScreenHeight, 0.1f, 3000.0f);
	/*
	// �������e�̏ꍇ
	float cameraDistance = 1000;
	Vector3 eye = Vector3(0.0f, 0.0f,-500.0f);
	Vector3 target = Vector3::Zero;
	Vector3 up = Vector3::UnitY;
	Matrix4 viewProj = Matrix4::CreateLookAt(eye, target, up);
	viewProj *= Matrix4::CreatePerspectiveFOV(Math::Pi / 4, mScreenWidth , mScreenHeight, 0.01f, 3000.0f);
	*/
	mShader->SetMatrixUniform("uViewProj", viewProj);
	return true;
}