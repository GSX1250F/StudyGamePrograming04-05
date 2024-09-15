#pragma once
#include <vector>
#include "Actor.h"
#include <unordered_map>

class Maze : public Actor
{
public:
	Maze(class Game* game, int mapWidth, int mapHeight);
	void ActorInput(const SDL_Event& event) override;
	void UpdateActor(float deltaTime) override;

	int GetGameStart() { return gameStart; }
	void SetGameClear(bool value) { gameClear = value; }

	// マップを（再）構成
	void GenerateMap();

	Vector3 GetTilePos(int i, int j);

	class Tile* GetStartTile();
	class Tile* GetGoalTile();

	std::vector<std::vector<class Tile*>> GetTiles() { return mTiles; }
	class Treasure* GetTreasure() { return mTreasure; }
	void SetClearPictPos(Vector3 pos);

private:
	// ゲームスタート
	bool gameStart;
	// ゲームクリア
	bool gameClear;
	// リセット開始
	bool resetStart;
	// リセット中
	bool resetPending;
	// リセット完了
	bool resetEnd;
	
	std::vector<std::vector<int>> mMapIndex;
	int mMapWidth;
	int mMapHeight;
	
	//スタート位置
	int starti;
	int startj;
	//ゴール位置
	int goali;
	int goalj;
	// タイルサイズ
	float mTileSize;
	
	class Tile* mTile;
	std::vector<std::vector<class Tile*>> mTiles;
	class Brave* mBrave;
	class Shadow* mShadow;
	class ClearPict* mClearPict;
	class Treasure* mTreasure;

	// 経路探索用
	void MakeGraphNodes(std::vector<std::vector<class Tile*>> &tiles);
	bool FindPath(class Tile* start, class Tile* goal);
};

