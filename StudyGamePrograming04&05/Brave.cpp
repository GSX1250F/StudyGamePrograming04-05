#include "Brave.h"
#include "Game.h"
#include "Renderer.h"
#include "Maze.h"
#include "Tile.h"
#include "Treasure.h"
#include "AnimSpriteComponent.h"
#include "CircleComponent.h"
#include "MoveComponent.h"

Brave::Brave(Game* game)
	: Actor(game)
	, speed(100.0f)
{
	//SetScale(0.85f);
	SetRadius(12.0f);
	/*
	// アニメーションのスプライトコンポーネントを作成
	asc = new AnimSpriteComponent(this, 45);
	std::vector<Texture*> anims = {
		game->GetRenderer()->GetTexture("Assets/Brave01.png"),
		game->GetRenderer()->GetTexture("Assets/Brave02.png"),
		game->GetRenderer()->GetTexture("Assets/Brave03.png"),
		game->GetRenderer()->GetTexture("Assets/Brave04.png"),
		game->GetRenderer()->GetTexture("Assets/Brave05.png"),
		game->GetRenderer()->GetTexture("Assets/Brave06.png"),
		game->GetRenderer()->GetTexture("Assets/Brave07.png"),
		game->GetRenderer()->GetTexture("Assets/Brave08.png"),
		game->GetRenderer()->GetTexture("Assets/Brave09.png"),
		game->GetRenderer()->GetTexture("Assets/Brave10.png"),
		game->GetRenderer()->GetTexture("Assets/Brave11.png"),
		game->GetRenderer()->GetTexture("Assets/Brave12.png"),
		game->GetRenderer()->GetTexture("Assets/Brave13.png"),
		game->GetRenderer()->GetTexture("Assets/Brave14.png"),
		game->GetRenderer()->GetTexture("Assets/Brave15.png"),
		game->GetRenderer()->GetTexture("Assets/Brave16.png")
	};
	asc->SetAnimTextures(anims);
	asc->SetAnimNum(0, 3, true);
	*/

	//CircleComponent作成
	cc = new CircleComponent(this);

	//MoveComponent作成
	mc = new MoveComponent(this);	
}

void Brave::ActorInput(const SDL_Event& event){
	if (GetGame()->GetMaze()->GetGameStart()) {
		/*
		//入力に応じて、アニメーションの設定と移動
		//int bg = asc->GetAnimNumBeg();
		//int ed = asc->GetAnimNumLast();

		if (event.type == SDL_KEYDOWN) {
			// キーが押されたとき
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				//if (bg != 0 || ed != 3) { asc->SetAnimNum(0, 3, true); }
				mc->SetVelocity(speed * Vector3(0.0f, -1.0f, 0.0f));
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				//if (bg != 4 || ed != 7) { asc->SetAnimNum(4, 7, true); }
				mc->SetVelocity(speed * Vector3(0.0f, 1.0f, 0.0f));
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				//if (bg != 8 || ed != 11) { asc->SetAnimNum(8, 11, true); }
				mc->SetVelocity(speed * Vector3(1.0f, 0.0f, 0.0f));
			}
			else if (event.key.keysym.sym == SDLK_LEFT)
			{
				//if (bg != 12 || ed != 15) { asc->SetAnimNum(12, 15, true); }
				mc->SetVelocity(speed * Vector3(-1.0f, 0.0f, 0.0f));
			}
		}		
		else
		{
			// 何も操作していないとき
			mc->SetVelocity(Vector3::Zero);
		}
		*/
		
		float forwardSpeed = 0.0f;
		float angularSpeed = 0.0f;
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_UP)
			{
				forwardSpeed += speed;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				forwardSpeed -= speed;
			}
			else if (event.key.keysym.sym == SDLK_LEFT)
			{
				angularSpeed -= Math::Pi;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				angularSpeed += Math::Pi;
			}
		}
		else
		{
			// 何も操作していないとき
			forwardSpeed = 0.0f;
			angularSpeed = 0.0f;
		}
		mc->SetVelocity(forwardSpeed * GetForward());
		mc->SetRotSpeed(angularSpeed);
		
	}	
}

void Brave::UpdateActor(float deltaTime){
	if (GetGame()->GetMaze()->GetGameStart()) {
		GetGame()->GetMaze()->SetClearPictPos(GetPosition());
		if (Intersect(*cc, *GetGame()->GetMaze()->GetTreasure()->GetCircle())) {
			// ゴール
			GetGame()->GetMaze()->SetGameClear(true);
		}

		for (auto tilecol : GetGame()->GetMaze()->GetTiles())
		{
			for (auto tile : tilecol)
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

		// Compute new camera from this actor
		Vector3 cameraPos = GetPosition();
		Vector3 target = GetPosition() + GetForward() * 300.0f;
		Vector3 up = Vector3::UnitZ;

		Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
		GetGame()->GetRenderer()->SetViewMatrix(view);



	}	
}