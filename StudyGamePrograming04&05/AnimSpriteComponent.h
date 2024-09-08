#pragma once
#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override;
	
	// アニメーションに使うテクスチャを設定する
	void SetAnimTextures(const std::vector<Texture*>& textures);
	// アニメーションに使うテクスチャの範囲を設定する。lastが-1なら全範囲とする。
	void SetAnimNum(int beg, int last, bool loop_flag);
	// アニメーションのフレームレートを設定 / 取得
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
	// 現在表示中のアニメーションの情報取得
	int GetAnimNumBeg() { return mAnimNumBeg; }
	int GetAnimNumLast() { return mAnimNumLast; }
	int GetAnimNumCurr() { return static_cast<int>(mCurrFrame); }

	
private:
	std::vector<Texture*> mAnimTextures;	// アニメーションでのすべてのテクスチャ
	int mAnimNumBeg;	//アニメーションの最初の配列番号
	int mAnimNumLast;	//アニメーションの最後の配列番号
	float mCurrFrame;	//現在のフレーム
	float mAnimFPS;		// アニメーションのフレームレート
	bool mLoopFlag;		// アニメーションをループさせるかしないか
	bool mIsAnimating;	// アニメーション中かどうか
};

