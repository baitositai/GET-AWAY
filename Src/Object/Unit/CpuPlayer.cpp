#include "CpuPlayer.h"
#include "../../Utility/Utility.h"
#include "../../Utility/PathFinder.h"
#include "../Stage.h"
#include "UnitEffects.h"

namespace
{
	Vector2 mapChipSize = { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE };		//マップチップサイズ
	Vector2 localPos = { Stage::MAP_CHIP_SIZE / 2,Stage::MAP_CHIP_SIZE / 2 };	//相対座標
}

CpuPlayer::CpuPlayer(const CHARA_TYPE charaType, PathFinder& pathFinder, const PATH_FIND_TYPE pathFindType) :
	UnitBase(charaType),
	pathFinder_(pathFinder),
	pathFindType_(pathFindType)
{
	pathIndex_ = -1;	
}

CpuPlayer::~CpuPlayer(void)
{
}

void CpuPlayer::Draw()
{
	UnitBase::Draw(); // 基底クラスの描画を呼び出す

	Vector2 pos = pos_.ToVector2(); // 現在の位置を取得
	int length = static_cast<int>(pathFindNames_[static_cast<int>(pathFindType_)].length() * 16); // 経路探索名称の長さを計算
	pos = Vector2::AddVector2(pos, { -length / 2, -20}); // 相対座標を加算

	//経路探索名称を描画
	DrawFormatString(pos.x,pos.y,Utility::RED, pathFindNames_[static_cast<int>(pathFindType_)].c_str());
}

void CpuPlayer::SetTargetaPos(const Vector2 target)
{
	//現在の経路を削除
	movePath_.clear();
	pathIndex_ = 0;

	//プレイヤー座標をマップ座標へ変換
	Vector2 mapPos = Utility::WorldToMapPos(pos_.ToVector2(), mapChipSize);

	//経路を探す
	bool isSearch = false;
	switch (pathFindType_)
	{
	case PATH_FIND_TYPE::DIJKSTRA:
		isSearch = pathFinder_.FindPath(mapPos, target, movePath_);
		break;
	case PATH_FIND_TYPE::RIGHT_PATH_FIND:
		isSearch = pathFinder_.RightHandSearch(mapPos, target, movePath_);
		break;
	case PATH_FIND_TYPE::A_STAR:
		isSearch = pathFinder_.FindPathAStar(mapPos, target, movePath_);
		break;
	}
	//経路が見つかった場合
	if (isSearch)
	{
		//経路が見つかった場合movePath_に保存される
		//最初は現在地なので次のマスからスタートされる
		pathIndex_++;
	}
}

void CpuPlayer::SetMapToWorldPos(const Vector2 mapPos)
{
	pos_ = Utility::MapToWorldPos(mapPos, mapChipSize).ToVector2F();
	pos_ = Vector2F::AddVector2F(pos_, localPos.ToVector2F());
}

void CpuPlayer::SetParameter()
{
	unitColor_ = Utility::PINK; //ユニットカラー設定

	//体力
	life_ = DEFAULT_LIFE;

	//相対座標分足す
	pos_ = Vector2F::AddVector2F(pos_, localPos.ToVector2F());
}

void CpuPlayer::ProcessMoveUpdate(GameScene& parent)
{
	//経路がない場合処理を実行しない
	if (pathIndex_ >= movePath_.size())
	{
		return;
	}

	//相対座標分引く
	Vector2F pos = Vector2F::SubVector2F(pos_, localPos.ToVector2F());

	//次に進むべきマス
	const Vector2& nextPos = movePath_[pathIndex_];

	//プレイヤー座標をマップ座標へ変換
	Vector2 mapPos = Utility::WorldToMapPos(pos.ToVector2(), mapChipSize);

	//次のマスのワールド座標
	Vector2 targetPixelPos = Utility::MapToWorldPos(nextPos, mapChipSize);

	//差分を求める
	Vector2 delta = Vector2::SubVector2(targetPixelPos, pos.ToVector2());

	// しきい値以内なら到達したとみなす
	if (delta.Length() < MOVE_SPEED)
	{
		pos = Utility::MapToWorldPos(nextPos, mapChipSize).ToVector2F(); // 補正
		pathIndex_++;   // 次へ
		return;
	}

	// 4方向のみに限定（X優先、またはY優先）
	// delta は次の目標マスの方向ベクトル（ピクセル単位）
	movePow_ = { 0, 0 };

	if (std::abs(delta.x) > std::abs(delta.y))
	{
		movePow_.x = (delta.x > 0) ? 1 : -1;
	}
	else
	{
		movePow_.y = (delta.y > 0) ? 1 : -1;
	}

	// 移動
	pos.x += movePow_.x * MOVE_SPEED;
	pos.y += movePow_.y * MOVE_SPEED;

	//相対座標分足す
	pos_ = Vector2F::AddVector2F(pos, localPos.ToVector2F());
}