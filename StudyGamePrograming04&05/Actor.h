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
	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	class Game* GetGame() { return mGame; }
	// ワールド変換
	void ComputeWorldTransform();
	Matrix4 GetWorldTransform() { return mWorldTransform; }

	// 全座標系
	State GetState() const { return mState; }	// 状態	
	void SetState(State state) { mState = state; }

	float GetRadius() const { return mRadius * mScale; }	// 半径
	void SetRadius(float radius) { mRadius = radius; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }		// 拡大率（質量は変えない）	
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }

	// ワールド座標系
	// 位置
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	// 回転
	const Quaternion& GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }
	// 向きの単位ベクトル
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }
	Vector3 GetStrafe() const { return Vector3::Transform(Vector3::UnitY, mRotation); }
	Vector3 GetUpward() const { return Vector3::Transform(Vector3::UnitZ, mRotation); }


	

private:
	class Game* mGame;
	std::vector<class Component*> mComponents;
	Matrix4 mWorldTransform;
	bool mRecomputeWorldTransform;

	// 全座標系
	State mState;			//アクター状態
	float mScale;			//拡大率
	float mRadius;			//半径（拡大率は無視）
	// ワールド座標系
	Vector3 mPosition;		//画面上の位置
	Quaternion mRotation;	//回転（軸と角度）
};