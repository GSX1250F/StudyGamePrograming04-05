#include "MazeCreator.h"
#include "Random.h"
#include "Math.h"

int mWidth;
int mHeight;
Vector2 mCell;
Vector2 startCell;
int digCount;
std::vector<Vector2> startCells;
std::vector<std::vector<int> > maze;
const int start = 2;
const int goal = 3;
const int path = 0;
const int wall = 1;

enum Direction { up = 0, right = 1, down = 2, left = 3 };

void Dig(Vector2);
void SetPath(Vector2);
bool GetStartCell();

std::vector<std::vector<int>> MazeCreate(int& width, int& height)
{
    //横幅、縦幅をともに奇数にする。
    if (width % 2 == 0) { width++; };
    if (height % 2 == 0) { height++; };
    // 迷路情報を初期化
    mWidth = width;
    mHeight = height;
    // 迷路情報を配列化
    maze.resize(mWidth);
    for (int i = 0; i < mWidth; i++)
    {
        for (int j = 0; j < mHeight; j++)
        {
            maze[i].resize(mHeight);
        }
    }
    // 5未満のサイズや偶数では生成できない
    if (width >= 5 && height >= 5)
    {
        // 全てを壁で埋める
        // 穴掘り開始候補(x,yともに偶数)座標を保持しておく
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
                {
                    maze[x][y] = path;  // 外壁は判定の為通路にしておく(最後に戻す)
                }
                else
                {
                    maze[x][y] = wall;
                }
            }
        }
        //スタート位置をランダムに設定。できるだけ左上にしておく。最初のCellがスタート位置とする。
        Random::Init();
        startCell = Random::GetVector(Vector2::Zero, Vector2(2.9f, 2.9f));
        // 1,3,5の奇数にする
        startCell.x = static_cast<int>(startCell.x) * 2 + 1;
        startCell.y = static_cast<int>(startCell.y) * 2 + 1;
        mCell = startCell;

        // 穴掘り開始
        digCount = 0;
        Dig(mCell);

        // 外壁を戻す
        for (int y = 0; y < mHeight; y++)
        {
            for (int x = 0; x < mWidth; x++)
            {
                if (x == 0 || y == 0 || x == mWidth - 1 || y == mHeight - 1)
                {
                    maze[x][y] = wall;
                }
            }
        }
    }
    else {
        //全部通路の情報を返す（迷路にならない）スタートは左上、ゴールは右下
        for (int i = 0; i < mWidth; i++)
        {
            for (int j = 0; j < mHeight; j++)
            {
                maze[i][j] = path;
            }
        }
        maze[0][0] = start;
        maze[mWidth - 1][mHeight - 1] = goal;
    }
	return maze;
}

// 座標(x, y)に穴を掘る
void Dig(Vector2 cell)
{
    // 指定座標から掘れなくなるまで堀り続ける
    while (true)
    {
        // 掘り進めることができる方向のリストを作成
        std::vector<Direction> directions;
        int x = static_cast<int>(cell.x);
        int y = static_cast<int>(cell.y);
        if (y >= 2) {
            if(maze[x][y - 1] == wall && maze[x][y - 2] == wall) { directions.push_back(up); }
        }
        if (x + 2 <= mWidth - 1) {
            if (maze[x + 1][y] == wall && maze[x + 2][y] == wall) { directions.push_back(right); }
        }
        if (y + 2 <= mHeight - 1) {
            if (maze[x][y + 1] == wall && maze[x][y + 2] == wall) { directions.push_back(down); }
        }
        if (x >= 2) {
            if (maze[x - 1][y] == wall && maze[x - 2][y] == wall) { directions.push_back(left); }
        }
        // 掘り進められない場合、ループを抜ける。掘り進めが１回目ならそこをゴールとする。
        if (directions.size() == 0) {
            if (digCount == 0)
            {
                maze[x][y] = goal;
            }
            digCount++;

            break;
        }
        else
        {
            // 指定座標を通路とし穴掘り候補座標から削除
            SetPath(cell);
            // 掘り進められる場合はランダムに方向を決めて掘り進める
            int dirIndex = Random::GetIntRange(0, directions.size() - 1);
            Random::Init();
            // 決まった方向に先2マス分を通路とする
            switch (directions[dirIndex])
            {
            case up:
                cell.y--;
                SetPath(cell);
                cell.y--;
                SetPath(cell);
                break;
            case right:
                cell.x++;
                SetPath(cell);
                cell.x++;
                SetPath(cell);
                break;
            case down:
                cell.y++;
                SetPath(cell);
                cell.y++;
                SetPath(cell);
                break;
            case left:
                cell.x--;
                SetPath(cell);
                cell.x--;
                SetPath(cell);
                break;
            }
        }
        directions.clear();
    }

    // どこにも掘り進められない場合、穴掘り開始候補座標から掘りなおし
    // 候補座標が存在しないとき、穴掘り完了
    bool flag = GetStartCell();
    if (flag)
    {
        Dig(mCell);
    }
    else
    {
        //start位置を設定
        maze[static_cast<int>(startCell.x)][static_cast<int>(startCell.y)] = start;
    }
}

// 座標を通路とする(穴掘り開始座標候補の場合は保持)
void SetPath(Vector2 cell)
{
    maze[static_cast<int>(cell.x)][static_cast<int>(cell.y)] = path;
    if ((static_cast<int>(cell.x) % 2 == 1) && (static_cast<int>(cell.y) % 2 == 1))
    {
        // 穴掘り候補座標
        startCells.push_back(cell);
    }
}

bool GetStartCell()
{
    if (startCells.size() == 0) { return false; }
    else
    {
        // ランダムに開始座標を取得する
        int index = Random::GetIntRange(0, startCells.size() - 1);
        Random::Init();
        mCell = startCells[index];
        startCells.erase(startCells.begin() + index);
        return true;
    }

}