#include "Brave.h"
#include "Game.h"
#include "Renderer.h"
#include "Maze.h"
#include "Tile.h"
#include "Treasure.h"
#include "SpriteComponent.h"
#include "CircleComponent.h"
#include "MoveComponent.h"
#include "SquareComponent.h"

Brave::Brave(Game* game)
	: Actor(game)
	, speed(1000.0f)
{
	SetScale(0.85f);
	
	sc = new SpriteComponent(this, 100);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/ClearPict.png"));
	sc->SetVisible(false);

	//CircleComponentì¬
	cc = new CircleComponent(this);
		
	//MoveComponentì¬
	mc = new MoveComponent(this);	
}

void Brave::ActorInput(const SDL_Event& event){
	if (GetGame()->GetMaze()->GetGameStart()) {
	
	}	
}

void Brave::UpdateActor(float deltaTime){
	if (GetGame()->GetMaze()->GetGameStart()) {
		if (Intersect(*cc, *GetGame()->GetMaze()->GetTreasure()->GetCircle())) {
			// ƒS[ƒ‹
			GetGame()->GetMaze()->SetGameClear(true);
			sc->SetVisible(true);
		}

		for (auto tilecol : GetGame()->GetMaze()->GetTiles())
		{
			for (auto tile : tilecol)
			{
				if (tile->GetTileState() == Tile::EWall) {
					//•Ç‚ÉÕ“Ë‚µ‚Ä‚¢‚½‚ç—£‚·B
					if (Intersect(*cc, *tile->GetCircle())) {
						//‚Q‚Â‚Ì’†S‚ðŒ‹‚Ôü•ªã‚ÅA‚Q‚Â‚Ì”¼Œa‚¾‚¯—£‚·
						Vector3 diff = Vector3::Normalize(this->GetPosition() - tile->GetPosition());
						this->SetPosition(tile->GetPosition() + diff * (this->GetRadius() + tile->GetRadius()));
					}
				}				
			}
		}
	}
	Vector3 cameraPos = GetPosition();
	Vector3 target = GetPosition() + GetForward() * 300.0f;
	Vector3 up = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}