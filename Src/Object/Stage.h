#pragma once
#include <vector>
#include "../Common/Vector2.h"

//ステージの種類
enum class STAGE_TYPE
{
	TYPE1,
	TYPE2,
	TYPE3,
};

class Stage
{
public:

	//マップチップの役割
	enum class ROLE
	{
		NONE,
		WALL,
		MAX
	};

	//マップチップサイズ
	static constexpr int MAP_CHIP_SIZE = 32;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stage();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Stage();

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 通常描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーを上書きして描画
	/// </summary>
	void OverDraw();

	/// <summary>
	/// そのマップチップが衝突判定を持つか調べる
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	/// <returns>衝突判定結果</returns>
	bool IsCollidableMapChip(const Vector2 mapPos);

	/// <summary>
	/// マップデータを返す
	/// </summary>
	/// <returns>マップデータ</returns>
	std::vector<std::vector<int>> const GetMapData();

private:

	//マップデータの役割と色を格納する構造体
	struct MapData
	{
		int role;		//役割
		int color;		//色
	};

	//ステージ情報の格納
	std::vector<std::vector<int>> planeData_;	//地面
	std::vector<std::vector<int>> hitData_;		//障害物
	std::vector<std::vector<int>> overData_;	//上書き描画用

	//マップデータ
	std::vector<std::vector<MapData>> mapDatas_;

	//マップサイズ
	Vector2 size_;

	//マップチップ画像
	int* imgMapChips_;

};

