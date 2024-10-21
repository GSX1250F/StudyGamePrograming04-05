#pragma once
#include "Actor.h"
#include <vector>
class Tile : public Actor
{
public:
	enum TileState
	{
		EDefault,
		EWall,
		EStart,
		EGoal
	};

	Tile(class Game* game);
	void UpdateActor(float deltaTime) override;

	void UpdateTexture();

	class SpriteComponent* GetSprite() { return sc; }
	class CircleComponent* GetCircle() { return cc; }
	TileState GetTileState() { return mTileState; }
	float GetTexSize() { return mTexSize; }
	void SetTileState(TileState);
	void SetAdjacent(class Tile* tile) { mAdjacent.push_back(tile); }
	void ClearAdjacent() { mAdjacent.clear(); }
	std::vector<class Tile*> GetAdjacent() { return mAdjacent; }
	void SetParent(class Tile* tile) { mParent = tile; }
	class Tile* GetParent() { return mParent; }


private:
	class SpriteComponent* sc;
	class CircleComponent* cc;

	TileState mTileState;
	float mTexSize;

	std::vector<class Tile*> mAdjacent;	//隣接ノード配列
	Tile* mParent;	//親ノード

};