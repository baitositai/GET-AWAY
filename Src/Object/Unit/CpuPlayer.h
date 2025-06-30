#pragma once
#include <vector>
#include <string>
#include "UnitBase.h"

class PathFinder;
enum class PATH_FIND_TYPE;

class CpuPlayer : public UnitBase
{
public:

	//体力
	static constexpr int DEFAULT_LIFE = 1;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="charaType">キャラ種類</param>
	/// <param name="pathFinder">経路探索</param>
	/// <param name="pathFindType">経路探索種類</param>
	CpuPlayer(const CHARA_TYPE charaType, PathFinder& pathFinder, const PATH_FIND_TYPE pathFindType);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~CpuPlayer(void) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	
	/// <summary>
	/// 目的地を設定する
	/// </summary>
	/// <param name="target">目的地</param>
	void SetTargetaPos(const Vector2 target);

	/// <summary>
	/// マップ座標を設定
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	void SetMapToWorldPos(const Vector2 mapPos);

private:

	//経路探索
	PathFinder& pathFinder_;

	//経路探索手法
	PATH_FIND_TYPE pathFindType_;

	//最短経路
	std::vector<Vector2> movePath_;

	//たどっている経路のインデックス
	int pathIndex_;

	//パラメーター設定
	void SetParameter() override;

	//移動処理
	void ProcessMoveUpdate(GameScene& parent) override;

	//経路探索名称
	std::wstring pathFindNames_[3] = {
		L"ダイクストラ",
		L"右手探索",
		L"Aスター" };
};

