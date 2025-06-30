#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>
#include "PathFinder.h"

PathFinder::PathFinder()
{
}

void PathFinder::SetMap(const std::vector<std::vector<int>>& map)
{
    //マップをコピー
    mapData_ = map;

	//マップのサイズを取得
    int height = map.size();
    int width = map[0].size();

	//距離データと前のデータを初期化
    distData_.assign(height, std::vector<int>(width, INT_MAX));
    prevData_.assign(height, std::vector<Vector2>(width, { -1, -1 }));
}

bool PathFinder::FindPath(const Vector2& start, const Vector2& goal, std::vector<Vector2>& path)
{
    //優先度付きキューを用意
    std::priority_queue<Node> pq;

    //マップサイズを取得
    int height = static_cast<int>(mapData_.size());
    if (height == 0) return false;  //高さがない場合終了
    int width = static_cast<int>(mapData_[0].size());

    //距離データと前のデータを初期化
    distData_.assign(height, std::vector<int>(width, INT_MAX));
    prevData_.assign(height, std::vector<Vector2>(width, { -1, -1 }));

    //始点の距離は0に設定し、キューに追加
    distData_[start.y][start.x] = 0;
    pq.push({ 0, start });

	//探索ループ
    while (!pq.empty()) 
    {
        Node current = pq.top(); pq.pop();

        //現在ノードの座標がゴールなら探索終了
        if (current.pos.x == goal.x && current.pos.y == goal.y) { break; }

        //４方向を順に探索
        for (int i = 0; i < DIR_NUM; ++i) 
        {
            Vector2 newPos = { current.pos.x + DX[i], current.pos.y + DY[i] };

			//範囲外チェック
            if (!IsInside(newPos.x, newPos.y)) { continue; }

            //壁の場合は通れないためスキップ
            if (mapData_[newPos.y][newPos.x] == BLOCK) continue;
            
			//新たに通るコストは現在ノードのコスト+1
            int newCost = distData_[current.pos.y][current.pos.x] + 1;

            //もし記録されているコストより小さければ更新
            if (newCost < distData_[newPos.y][newPos.x])
            {
				distData_[newPos.y][newPos.x] = newCost;    //最短距離を更新
				prevData_[newPos.y][newPos.x] = { current.pos.x, current.pos.y }; //前のノードを更新
				pq.push({ newCost, newPos }); //キューに追加(次の探索候補として追加)
            }
        }
    }

    //ゴールまでの距離が更新されていなければ経路なし
    if (distData_[goal.y][goal.x] == INT_MAX)
    {
        return false;   //探索失敗
    }

    //経路復元
	//ゴールを起点にして、前のノードを辿っていく
    path.clear();
    Vector2 cur = goal;
    int loopCount = 0;
    while (cur.x != -1 && cur.y != -1) 
    {
        if (loopCount++ > MAX_PATH_LENGTH)
        {
            // 無限ループ防止
            break;
        }

        path.push_back(cur);
        cur = prevData_[cur.y][cur.x];

        //// 無限ループ防止（自分自身を指している）
        //if (Vector2::IsSameVector2(goal, cur)) break;

        //cur = goal;
    }

	//経路を逆順にする
    std::reverse(path.begin(), path.end());

    //探索大成功
    return true;
}

bool PathFinder::IsInside(int x, int y) const
{
	//マップの範囲内かチェック
    return y >= 0 && y < static_cast<int>(mapData_.size()) &&
        x >= 0 && x < static_cast<int>(mapData_[0].size());
}

bool PathFinder::RightHandSearch(const Vector2& start, const Vector2& goal, std::vector<Vector2>& pathOut)
{
    pathOut.clear();

    Vector2 pos = start;
    DIR dir = DIR::RIGHT; // 初期向き（任意）

    static constexpr int MAX_STEPS = 1000;
    int step = 0;

    while (step++ < MAX_STEPS)
    {
        pathOut.push_back(pos);

        if (Vector2::IsSameVector2(pos, goal))
        {
            return true;
        }

        // 右を見る
        DIR rightDir = TurnRight(dir);
        Vector2 rightVec = DirToVector(rightDir);
        Vector2 rightPos = { pos.x + rightVec.x, pos.y + rightVec.y };

        // 前を見る
        Vector2 forwardVec = DirToVector(dir);
        Vector2 forwardPos = { pos.x + forwardVec.x, pos.y + forwardVec.y };

        // 右が通れるなら右へ曲がる
        if (IsWalkable(rightPos))
        {
            dir = rightDir;
            pos = rightPos;
        }
        // 前が通れるならそのまま進む
        else if (IsWalkable(forwardPos))
        {
            pos = forwardPos;
        }
        // それ以外は左へ回る
        else 
        {
            dir = TurnLeft(dir);
        }
    }

    // ステップ上限超過＝ルート無し（無限ループ回避）
    return false;
}

bool PathFinder::FindPathAStar(const Vector2& start, const Vector2& goal, std::vector<Vector2>& pathOut)
{
    pathOut.clear();

    const int height = static_cast<int>(mapData_.size());
    const int width = static_cast<int>(mapData_[0].size());

    // 移動方向（上下左右）
    const std::vector<Vector2> directions = 
    {
        { 0, -1 },
        { 1, 0 }, 
        { 0, 1 }, 
        { -1, 0 }
    };

    // g(現在までのコスト)
    std::vector<std::vector<int>> gScore(height, std::vector<int>(width, INT_MAX));
    gScore[start.y][start.x] = 0;

    // 前のノードを保存
    prevData_ = std::vector<std::vector<Vector2>>(height, std::vector<Vector2>(width, { -1, -1 }));

    // 優先度付きキュー（最小コスト順）
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    openList.push({ Heuristic(start, goal), start });

    while (!openList.empty())
    {
        Node current = openList.top();
        openList.pop();

        if (Vector2::IsSameVector2(current.pos, goal))
        {
            // 経路復元
            Vector2 cur = goal;
            while (cur.x != -1 && cur.y != -1)
            {
                pathOut.push_back(cur);
                cur = prevData_[cur.y][cur.x];
            }
            std::reverse(pathOut.begin(), pathOut.end());
            return true;
        }

        for (const Vector2& dir : directions)
        {
            Vector2 next = { current.pos.x + dir.x, current.pos.y + dir.y };

            // 範囲外チェック
            if (next.x < 0 || next.y < 0 || next.x >= width || next.y >= height)
            {
                continue;
            }

            // 壁チェック
            if (mapData_[next.y][next.x] != 0)
            {
                continue;
            }

            int tentativeG = gScore[current.pos.y][current.pos.x] + 1;

            if (tentativeG < gScore[next.y][next.x])
            {
                gScore[next.y][next.x] = tentativeG;
                int f = tentativeG + Heuristic(next, goal);
                openList.push({ f,next });
                prevData_[next.y][next.x] = current.pos;
            }
        }
    }

    return false; // 経路なし
}

bool PathFinder::IsWalkable(const Vector2& pos) const
{
    if (pos.x < 0 || pos.y < 0 || pos.y >= static_cast<int>(mapData_.size()) || pos.x >= static_cast<int>(mapData_[0].size()))
        return false;

    return mapData_[pos.y][pos.x] == 0; // 0 = 通れる、1 = 壁
}

int PathFinder::Heuristic(const Vector2& a, const Vector2& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
