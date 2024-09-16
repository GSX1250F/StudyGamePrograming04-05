#include "Treasure.h"
#include "Game.h"
#include "Renderer.h"
#include "SpriteComponent.h"
#include "CircleComponent.h"

Treasure::Treasure(Game* game) : Actor(game){
	//スプライトコンポーネント作成、テクスチャ設定
	sc = new SpriteComponent(this, 20);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Treasure.png"));
	
	//CircleComponent作成
	cc = new CircleComponent(this);
}
