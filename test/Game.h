#pragma once
#include "SDL.h"
#include <string>
#include <vector>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class Renderer* GetRenderer() { return mRenderer; }

	void SetRunning(bool isrunning) { mIsRunning = isrunning; }

	//âÊñ ÉTÉCÉY
	const int mWindowWidth;
	const int mWindowHeight;

	//Game-specific	
	class Maze* GetMaze() { return mMaze; }


private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	class Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors;

	//Game-specific	
	class Maze* mMaze;

};
