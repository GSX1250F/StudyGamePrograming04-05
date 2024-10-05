#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);
	~MoveComponent();

	void Update(float deltatime) override;

	// 全座標系
	void SetMass(float v) { mMass = v; }		// 質量
	float GetImoment() const;		// 慣性モーメント
	void SetMoveResist(float v) { mResist = v; }		//移動の抵抗率
	void SetRotResist(float v) { mRotResist = v; }		//回転の抵抗率

	// ワールド座標系
	void SetVelocity(Vector3 value) { mVelocity = value; }		// 速度
	Vector3 GetVelocity() { return mVelocity; }
	void SetRotSpeed(Vector3 value) { mRotSpeed = value; }	// 回転速度モーメントベクトル
	Vector3 GetRotSpeed() { return mRotSpeed; }
	Vector3 GetAccel() const;		// 加速度
	Vector3 GetRotAccel() const;		// 回転加速度モーメントベクトル
	void SetForce(Vector3 value) { mForce = value; }		//力	
	void SetRotForce(Vector3 value) { mRotForce = value; }		//回転力モーメントベクトル
	Vector3 GetTorque() const;		// トルク(=回転力×半径）のモーメントベクトル


private:
	float mMass;			// 質量
	float mResist;		// 重心速度抵抗率(%)
	float mRotResist;		// 回転速度抵抗率(%)

	// ワールド座標系
	Vector3 mVelocity;		// 速度
	Vector3 mRotSpeed;		// 回転速度モーメントベクトル
	Vector3 mForce;		// 重心にかかる力
	Vector3 mRotForce;		// 回転方向の力モーメントベクトル

};