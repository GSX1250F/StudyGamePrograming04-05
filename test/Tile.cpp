#include "Tile.h"
#include "SomeSpriteComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "CircleComponent.h"

Tile::Tile(class Game* game)
	:Actor(game)
	, mTileState(EDefault)
	, mParent(nullptr)
{
	//スプライトコンポーネントを作成
	ssc = new SomeSpriteComponent(this, 10);
	ssc->TextureFiles = {
		"Assets/Default.png",
		"Assets/Wall.png"
	};
	ssc->SetSomeTextures(ssc->TextureFiles);
	mTexSize = ssc->GetTexWidth();
	
	//CircleComponent作成
	cc = new CircleComponent(this);	
}

void Tile::UpdateActor(float deltaTime)
{
	UpdateTexture();
}

void Tile::SetTileState(TileState state)
{
	mTileState = state;
	UpdateTexture();
}

void Tile::UpdateTexture()
{
	switch (mTileState)
	{
		case EWall:
			ssc->SelectTexture(ssc->TextureFiles[1]);
			break;
		default:
			ssc->SelectTexture(ssc->TextureFiles[0]);
	}
}