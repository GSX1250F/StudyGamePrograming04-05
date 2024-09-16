#pragma once
#include <SDL.h>

class Component
{
public:
	// コンストラクタ
	// updateOrderが小さいコンポーネントほど早く更新される
	Component(class Actor* owner, int updateOrder = 100);

	// デストラクタ
	virtual ~Component();

	// 各コンポーネント入力処理（オーバーライド可能）
	virtual void ProcessInput(const uint8_t* keyState) {}

	// 各コンポーネント更新（オーバーライド可能）
	virtual void Update(float deltaTime) {}

	// 更新順ゲッター
	int GetUpdateOrder() const { return mUpdateOrder; }

	// ワールド変換の通知を受け取る。
	virtual void OnUpdateWorldTransform() {}


protected:
	// 所有アクター
	class Actor* mOwner;

	// コンポーネントの更新順序
	int mUpdateOrder;
};