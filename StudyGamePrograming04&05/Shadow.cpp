#include "Shadow.h"
#include "Game.h"
#include "Renderer.h"
#include "Maze.h"
#include "Tile.h"
#include "Treasure.h"
#include "AnimSpriteComponent.h"
#include "CircleComponent.h"
#include "NavComponent.h"
#include "Math.h"


Shadow::Shadow(Game* game)
	: Actor(game)
	, speed(75.0f)
{
	SetScale(0.85f);
	// アニメーションのスプライトコンポーネントを作成
	asc = new AnimSpriteComponent(this, 40);
	std::vector<Texture*> anims = {
		game->GetRenderer()->GetTexture("Assets/Shadow01.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow02.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow03.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow04.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow05.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow06.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow07.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow08.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow09.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow10.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow11.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow12.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow13.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow14.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow15.png"),
		game->GetRenderer()->GetTexture("Assets/Shadow16.png")
	};
	asc->SetAnimTextures(anims);
	asc->SetAnimNum(0, 3, true);

	//CircleComponent作成
	cc = new CircleComponent(this);

	//NavComponent作成
	nc = new NavComponent(this);
	nc->SetSpeed(speed);
}

void Shadow::ActorInput(const SDL_Event& event)
{
	if (GetGame()->GetMaze()->GetGameStart()) {
		if (event.type == SDL_KEYUP)
		{
			// キーが離されたとき
			if (event.key.keysym.sym == SDLK_r)
			{
				//リセットされたら、スピードを少し上げる。
				speed += 10;
				if (speed > 145) { speed = 145; }
				nc->SetSpeed(speed);
			}
		}
	}
}

void Shadow::UpdateActor(float deltaTime)
{
	if (GetGame()->GetMaze()->GetGameStart())
	{
		if (Intersect(*cc, *GetGame()->GetMaze()->GetTreasure()->GetCircle())) {
			// ゴール
			GetGame()->GetMaze()->SetGameClear(true);
		}
		for (auto ctile : GetGame()->GetMaze()->GetTiles())
		{
			for (auto tile : ctile)
			{
				if (tile->GetTileState() == Tile::EWall) {
					//壁に衝突していたら離す。
					if (Intersect(*cc, *tile->GetCircle())) {
						//２つの中心を結ぶ線分上で、２つの半径だけ離す
						Vector3 diff = Vector3::Normalize(this->GetPosition() - tile->GetPosition());
						this->SetPosition(tile->GetPosition() + diff * (this->GetRadius() + tile->GetRadius()));
					}
				}
			}
		}
		//スプライトアニメーション
		int bg = asc->GetAnimNumBeg();
		int ed = asc->GetAnimNumLast();
		float angle = nc->GetNextDirAngle();
		if (angle >= -Math::Pi * 3 / 4 && angle < -Math::Pi / 4)
		{
			if (bg != 0 || ed != 3) { asc->SetAnimNum(0, 3, true); }
		}
		else if (angle >= -Math::Pi / 4 && angle < Math::Pi / 4)
		{
			if (bg != 8 || ed != 11) { asc->SetAnimNum(8, 11, true); }
		}
		else if (angle >= Math::Pi / 4 && angle < Math::Pi * 3 / 4)
		{
			if (bg != 4 || ed != 7) { asc->SetAnimNum(4, 7, true); }
		}
		else if ((angle >= -Math::Pi && angle < -Math::Pi * 3 / 4) ||
			(angle >= Math::Pi * 3 / 4 && angle <= Math::Pi))
		{
			if (bg != 12 || ed != 15) { asc->SetAnimNum(12, 15, true); }
		}
	}
}