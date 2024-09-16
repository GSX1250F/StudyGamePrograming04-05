#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	//コンストラクタ。Update Orderは早め。
	MoveComponent(class Actor* owner , int updateOrder = 10);
	~MoveComponent();

	//Updateをオーバーライド
	void Update(float deltatime) override;

	// 単純に動かすのに必要なパラメータのセッター＆ゲッター
	// 並進移動速度
	void SetVelocity(Vector3 vel) { mVelocity = vel; }
	// 回転速度
	void SetRotSpeed(float rotspeed) { mRotSpeed = rotspeed; }

	// 古典物理に必要なパラメータのセッター＆ゲッター
	// 並進移動加速度
	Vector3 GetMoveAccel() const;
	// 回転移動加速度
	float GetRotAccel() const;

	// 質量
	void SetMass(float v) { mMass = v; }
	//重心にかかる力
	void SetMoveForce(Vector3 v) { mMoveForce = v; }
	//回転方向の力
	void SetRotForce(float v) { mRotForce = v; }
	// 慣性モーメント
	float GetImoment() const;
	// トルク
	float GetTorque() const;
	//並進移動の抵抗率
	void SetMoveResist(float v) { mMoveResist = v; }
	//回転移動の抵抗率
	void SetRotResist(float v) { mRotResist = v; }
	
private:
	// 単純移動パラメータ
	Vector3 mVelocity;		// 並進移動速度
	float mRotSpeed;		// 回転速度

	// ニュートン力学パラメータ
	float mMass;			// 質量
	Vector3 mMoveForce;		// 重心にかかる力
	float mRotForce;		// 回転方向の力F +方向はCCW
	float mMoveResist;		// 重心速度抵抗率(%)
	float mRotResist;		// 回転速度抵抗率(%)
};