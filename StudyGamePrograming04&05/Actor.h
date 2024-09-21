#pragma once
#include <vector>
#include "Math.h"
#include <SDL.h>

class Actor
{
public:
	enum State
	{
		EActive,		//稼働中
		EPaused,		//更新停止中
		EDead			//削除対象
	};

	Actor(class Game* game);
	virtual ~Actor();

	// ゲームから呼び出されるProcess Input(オーバーライド不可)
	void ProcessInput(const SDL_Event& event);
	// アクター独自の入力処理(オーバーライド可能)
	virtual void ActorInput(const SDL_Event& event);

	// ゲームから呼び出される更新関数(オーバーライド不可)
	void Update(float deltaTime);
	// アクターが持つ全コンポーネントを更新(オーバーライド不可)
	void UpdateComponents(float deltaTime);
	// アクター独自の更新処理(オーバーライド可能)
	virtual void UpdateActor(float deltaTime);

	// ゲッター・セッター
	// 位置
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	// 拡大率（質量は変えない。半径に影響する）
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	// 回転
	const float& GetRotation() const { return mRotation; }
	void SetRotation(const float& rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }
	// 半径
	float GetRadius() const { return mRadius * mScale; }	//拡大率を考慮
	void SetRadius(float radius) { mRadius = radius; mRecomputeWorldTransform = true; }

	// 向きの単位ベクトル
	Vector3 GetForward() const { return Vector3(Math::Cos(mRotation), Math::Sin(mRotation), 0.0f); }

	// 状態
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	// ワールド変換
	void ComputeWorldTransform();
	Matrix4 GetWorldTransform() { return mWorldTransform; }

private:
	State mState;			//アクター状態
	Vector3 mPosition;		//画面上の位置
	float mScale;			//拡大率
	float mRotation;		//回転
	float mRadius;			//半径（拡大率は無視）
	std::vector<class Component*> mComponents;
	class Game* mGame;

	Matrix4 mWorldTransform;
	bool mRecomputeWorldTransform;
};