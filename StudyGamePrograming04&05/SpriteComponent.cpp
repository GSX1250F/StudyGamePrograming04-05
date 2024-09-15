#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	: Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
	, mVisible(true)
{
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture && mVisible)
	{
		// テクスチャサイズで再スケーリングしたワールド変換行列を作成
		Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexWidth));
		Matrix4 world = scaleMat * mOwner->GetWorldTransform();
		// ワールド変換
		shader->SetMatrixUniform("uWorldTransform", world);
		// 現在のテクスチャをセット
		mTexture->SetActive();
		// 短形を描画
		glDrawElements(
			GL_TRIANGLES,		// 描画するポリゴン／プリミティブの種類
			24,					// インデックスバッファにあるインデックスの数
			GL_UNSIGNED_INT,	// インデックスの型
			nullptr				// 通常はnullptr
		);
	}
}

void SpriteComponent::SetTexture(class Texture* texture)
{
	mTexture = texture;
	// 高さと幅を設定
	mTexWidth = texture->GetTexWidth();
	mTexHeight = texture->GetTexHeight();
	// 高さと幅の平均をActorの直径とする。
	mOwner->SetRadius((mTexWidth + mTexHeight) * 0.25f);
}