#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	: Component(owner)
	, mVelocity(Vector3::Zero)
	, mRotSpeed(Vector3::Zero)
	, mMass(1.0f)
	, mForce(Vector3::Zero)
	, mRotForce(Vector3::Zero)
	, mResist(0.0f)
	, mRotResist(0.0f)
{}

MoveComponent::~MoveComponent() {}

void MoveComponent::Update(float deltatime)
{
	if (!Math::NearZero(mVelocity.Length())) {
		// 位置を更新
		mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltatime);
	}
	if (!Math::NearZero(mRotSpeed.Length())) {
		// 向きを更新
		Quaternion rot = mOwner->GetRotation();
		// クォータニオン生成。回転速度モーメントベクトルの大きさが角速度の大きさ
		Vector3 axis = mRotSpeed;
		axis.Normalize();	// 回転軸。正規化する。
		float angle = deltatime * mRotSpeed.Length();	//角度変化量
		Quaternion inc(axis, angle);
		// もとのrotと増分のクォータニオンを結合
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}

	// 速度と角速度を更新
	SetVelocity(mVelocity + GetAccel() * deltatime);	//v = vo + at
	SetRotSpeed(mRotSpeed + GetRotAccel() * deltatime);		//ω = ωo + bt
}

Vector3 MoveComponent::GetAccel() const
{
	if (!Math::NearZero(mMass))
	{
		Vector3 accel = mForce * (1 / mMass);    //重心加速度の計算　F=ma  a=F*(1/m)
		accel -= mVelocity * mResist * 0.01f * (1 / mMass);
		return accel;
	}
	else
	{
		return Vector3::Zero;
	}
}

Vector3 MoveComponent::GetRotAccel() const
{
	if (!Math::NearZero(GetImoment()))
	{
		// 回転加速度の計算　回転加速度 = トルク / 慣性モーメント
		Vector3 accel = GetTorque() * (1.0f / GetImoment());		//回転加速度の計算 Fr=Ia  a=Fr/I
		//抵抗力 = 速さ*抵抗係数    減速 = -速さ*抵抗係数*半径/慣性モーメント
		accel -= mRotSpeed * mOwner->GetRadius() * mRotResist * (1.0f / GetImoment());
		return accel;
	}
	else
	{
		return Vector3::Zero;
	}
}

float MoveComponent::GetImoment() const
{
	// 慣性モーメント計算　※2次元においては、一様密度の円板とする。 I=0.5*質量*半径^2
	return 0.5f * mMass * mOwner->GetRadius() * mOwner->GetRadius();
}

Vector3 MoveComponent::GetTorque() const
{
	// トルク計算　トルク=回転方向の力 * 半径
	return mRotForce * mOwner->GetRadius();
}