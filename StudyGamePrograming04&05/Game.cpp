#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include "Actor.h"
#include "Renderer.h"
#include "Random.h"
#include "Maze.h"
#include "Brave.h"
#include "Shadow.h"
#include "ClearPict.h"
#include "Tile.h"
#include "Treasure.h"
#include <thread>
#include <chrono>

Game::Game()
	: mRenderer(nullptr)
	, mIsRunning(true)
	, mUpdatingActors(false)
	, mTicksCount(0)
	, mWindowWidth(1600)
	, mWindowHeight(900)
{}

bool Game::Initialize()
{
	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("SDLを初期化できません: %s", SDL_GetError());
		return false;
	}
	// レンダラー作成
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(mWindowWidth, mWindowHeight))
	{
		SDL_Log("レンダラーの初期化に失敗しました");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	Random::Init();		//乱数設定の初期化?

	LoadData();

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			mIsRunning = false;
		}
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				mIsRunning = false;
			}
		}

		mUpdatingActors = true;
		for (auto actor : mActors)
		{
			actor->ProcessInput(event);
		}
		mUpdatingActors = false;
	}
}

void Game::UpdateGame()
{
	// フレームレート調整（62.5fps)
	if (SDL_GetTicks() - mTicksCount < 16) {
		int sleep = 16 - (SDL_GetTicks() - mTicksCount);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));    // sleepミリ秒処理を止める
	}

	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	// 前のフレームから 16ms 経つまで待つ.※sleepしないのでCPU使用率が上がる。
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;		// デルタタイムの計算
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }			// デルタタイムを最大値で制限する
	mTicksCount = SDL_GetTicks();

	// アクターを更新
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 待ちになっていたアクターをmActorsに移動
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 死んだアクターを一時配列に追加
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// 死んだアクターを削除
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	//mMaze = new Maze(this, 51, 29);		//迷路クラス
	mMaze = new Maze(this, 7, 7);		//テスト用
}

void Game::UnloadData()
{
	// actorsを消去
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	if (mRenderer)
	{
		mRenderer->UnloadData();
	}	
}

void Game::Shutdown()
{
	UnloadData();
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// アクターの更新中ならmPendingActorsに追加
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}