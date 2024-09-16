#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder) 
	: Component(owner) 
	,mVelocity(Vector2::Zero)		// •ÀiˆÚ“®‘¬“x
	,mRotSpeed(0.0f)				// ‰ñ“]‘¬“x
	,mMass(1.0f)					// ¿—Ê
	,mMoveForce(Vector2::Zero)		// dS‚É‚©‚©‚é—Í
	,mRotForce(0.0f)				// ‰ñ“]•ûŒü‚Ì—ÍF +•ûŒü‚ÍCCW
	,mMoveResist(0.0f)				// dS‘¬“x’ïR—¦(%)
	,mRotResist(0.0f)				// ‰ñ“]‘¬“x’ïR—¦(%)
{
}

MoveComponent::~MoveComponent()
{
}

void MoveComponent::Update(float deltatime)
{
	// ˆÊ’u‚ÆŒü‚«‚ğXV
	mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltatime);		//x = xo + vt
	mOwner->SetRotation(mOwner->GetRotation() + mRotSpeed * deltatime);		//ƒ¦ = ƒ¦o + ƒÖt

	// ‘¬“x‚ÆŠp‘¬“x‚ğXV
	SetVelocity(mVelocity + GetMoveAccel() * deltatime);	//v = vo + at
	SetRotSpeed(mRotSpeed + GetRotAccel() * deltatime);		//ƒÖ = ƒÖo + bt
}

Vector2 MoveComponent::GetMoveAccel() const
{
	if (!Math::NearZero(mMass))
	{
		Vector2 accel = mMoveForce * (1 / mMass);    //dS‰Á‘¬“x‚ÌŒvZ@F=ma  a=F*(1/m)
		accel -= mVelocity * mMoveResist * 0.01f * (1 / mMass);
		return accel;
	}
	else
	{
		return Vector2::Zero;
	}
}

float MoveComponent::GetRotAccel() const
{
	if (!Math::NearZero(GetImoment()))
	{
		// ‰ñ“]‰Á‘¬“x‚ÌŒvZ@‰ñ“]‰Á‘¬“x = ƒgƒ‹ƒN / Šµ«ƒ‚[ƒƒ“ƒg
		float accel = GetTorque() / GetImoment();		//‰ñ“]‰Á‘¬“x‚ÌŒvZ Fr=Ia  a=Fr/I
		//’ïR—Í = ‘¬‚³*’ïRŒW”    Œ¸‘¬ = -‘¬‚³*’ïRŒW”*”¼Œa/Šµ«ƒ‚[ƒƒ“ƒg
		accel -= mRotSpeed * mOwner->GetRadius() * mRotResist / GetImoment();
		return accel;
	}
	else
	{
		return 0.0f; 
	}
}

float MoveComponent::GetImoment() const
{
	// Šµ«ƒ‚[ƒƒ“ƒgŒvZ@¦2ŸŒ³‚É‚¨‚¢‚Ä‚ÍAˆê—l–§“x‚Ì‰~”Â‚Æ‚·‚éB I=0.5*¿—Ê*”¼Œa^2
	return 0.5f * mMass * mOwner->GetRadius() * mOwner->GetRadius();
}

float MoveComponent::GetTorque() const
{
	// ƒgƒ‹ƒNŒvZ@ƒgƒ‹ƒN=‰ñ“]•ûŒü‚Ì—Í * ”¼Œa
	return mRotForce * mOwner->GetRadius();
}