#pragma once
#include <vector>
#include <queue>
#include <utility>
#include <climits>
#include <algorithm>
#include "../Common/Vector2.h"


enum class PATH_FIND_TYPE
{
	DIJKSTRA,			//ダイクストラ
	RIGHT_PATH_FIND,	//右手探索
	A_STAR,				//A*
};


class PathFinder
{
public:

	// 最大経路長
	static constexpr int MAX_PATH_LENGTH = 1000; 

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PathFinder();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PathFinder() = default;

	/// <summary>
	/// マップを設定
	/// </summary>
	/// <param name="map">マップデータ</param>
	void SetMap(const std::vector<std::vector<int>>& map);

	/// <summary>
	/// ダイクストラによる経路探索
	/// </summary>
	/// <param name="start">探索スタート位置</param>
	/// <param name="goal">探索ゴール位置</param>
	/// <param name="path">探索した経路</param>
	/// <returns>探索失敗した場合falseを返す</returns>
	bool FindPath(const Vector2& start, const Vector2& goal, std::vector<Vector2>& path);

	/// <summary>
	/// 右手探索法でルート決定
	/// </summary>
	/// <param name="start">探索スタート位置</param>
	/// <param name="goal">探索ゴール位置</param>
	/// <param name="pathOut">出力経路</param>
	/// <returns>探索失敗した場合falseを返す</returns>
	bool RightHandSearch(const Vector2& start, const Vector2& goal, std::vector<Vector2>& pathOut);

	/// <summary>
	/// A*法による経路探索
	/// </summary>
	/// <param name="start">探索スタート位置</param>
	/// <param name="goal">探索ゴール位置</param>
	/// <param name="pathOut">出力経路</param>
	/// <returns>探索失敗した場合falseを返す</returns>
	bool FindPathAStar(const Vector2& start, const Vector2& goal, std::vector<Vector2>& pathOut);

private:
	
	// 探索ノード（位置とコスト）を管理する構造体
	struct Node 
	{
		int cost; // 始点からこのノードまでのコスト（距離）
		Vector2 pos;

		// 優先度付きキューでコストの小さい順に並ぶように比較演算子を定義
		bool operator<(const Node& other) const 
		{
			return cost > other.cost; // 小さい順になるよう逆順
		}

		// 小さい順に並ぶよう比較演算子定義
		bool operator>(const Node& other) const
		{
			return cost > other.cost;
		}
	};

	std::vector<std::vector<int>> mapData_;				// マップデータ本体（0＝通行可、BLOCK＝壁）
	std::vector<std::vector<int>> distData_;			// 始点から各マスまでの最短距離（コスト）
	std::vector<std::vector<Vector2>> prevData_;		// 経路復元用、各マスの直前のマスを保持

	// 移動方向（上, 右, 下, 左）
	static constexpr int DIR_NUM = 4;         // 方向の数
	static constexpr int DX[DIR_NUM] = { 0, 1, 0, -1 };
	static constexpr int DY[DIR_NUM] = { -1, 0, 1, 0 };

	// 通行不可値
	static constexpr int BLOCK = 1;

	// マップ範囲チェック
	bool IsInside(int x, int y) const;

	//右手探索---------------------------------------------------------

	enum class DIR
	{
		UP,
		RIGHT, 
		DOWN, 
		LEFT
	};

	/// <summary>
	/// マップ上でそのマスが通行可能か動かを判断する
	/// </summary>
	bool IsWalkable(const Vector2& pos) const;

	// 右に曲がる
	DIR TurnRight(DIR dir) {return static_cast<DIR>((static_cast<int>(dir) + 1) % 4);}

	// 左に曲がる
	DIR TurnLeft(DIR dir) {return static_cast<DIR>((static_cast<int>(dir) + 3) % 4);}

	// 逆を向く
	DIR TurnBack(DIR dir) {return static_cast<DIR>((static_cast<int>(dir) + 2) % 4);}

	// 向きから移動ベクトルを取得
	Vector2 DirToVector(DIR dir)
	{
		switch (dir) 
		{
			case DIR::UP:
				return { 0, -1 };

			case DIR::RIGHT:
				return { 1, 0 };

			case DIR::DOWN:
				return { 0, 1 };

			case DIR::LEFT:
				return { -1, 0 };

			default: 
				return { 0, 0 };
		}
	}

	//A*法---------------------------------------------------------------------------------

	// ヒューリスティック関数（マンハッタン距離）
	static int Heuristic(const Vector2& a, const Vector2& b);

};

