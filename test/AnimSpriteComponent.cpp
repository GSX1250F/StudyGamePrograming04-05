#include "AnimSpriteComponent.h"
#include "Math.h"
#include <iostream>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(5.0f)
	, mAnimNumBeg(0)
	, mAnimNumLast(0)
	, mLoopFlag(false)
{}

void AnimSpriteComponent::Update(float deltaTime)
{
	if (mAnimTextures.size() > 0)
	{
		mCurrFrame += mAnimFPS * deltaTime;		// 現在のフレームを更新する

		// ループさせないアニメーションが終わったら止める。
		if (mLoopFlag == false) {
			if (mCurrFrame >= mAnimNumLast + 1)
			{
				mIsAnimating = false;	// アニメーションが止まった
				mCurrFrame = mAnimNumLast;
			}
			else
			{
				mIsAnimating = true;		// アニメーション中
			}
		}
		else {
			mIsAnimating = true;		// ループアニメはアニメーション中とする。
			if (mCurrFrame >= mAnimNumLast + 1)
			{
				mCurrFrame = mAnimNumBeg;				// 必要に応じて現在のフレームを巻き戻す
			}
		}
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);		// 現時点でのテクスチャを設定する
	}
	SpriteComponent::Update(deltaTime);
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		SetTexture(mAnimTextures[0]);		// 最初のフレームのテクスチャで初期設定
	}
}
void AnimSpriteComponent::SetAnimNum(int beg, int last, bool loop_flag)
{
	if (last == -1)last = mAnimTextures.size();		//lastが-1の場合すべての範囲をアニメーションとする
	mCurrFrame = static_cast<float>(beg);
	mAnimNumBeg = beg;
	mAnimNumLast = last;
	mLoopFlag = loop_flag;
}