#pragma once
#include "UnitBase.h"

class Player : public UnitBase
{
public:

	//体力
	static constexpr int DEFAULT_LIFE = 3;

	//衝突判定時に調べる箇所
	enum class CHECK_COL
	{
		RIGHT,	//右
		LEFT,	//左
		CENTER,	//中央
		MAX
	};

	//移動種類
	enum class MOVE_TYPE
	{
		NONE,
		NORMAL,
		MASS,
		NONSTOP,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="charaType">キャラクター種類</param>
	/// <param name="playerNo">プレイヤー識別番号</param>
	Player(const CHARA_TYPE charaType, const int playerNo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player(void) override;

private:

	//プレイヤー識別番号
	int playerNo_;	

	//移動種類
	MOVE_TYPE moveType_;

	//移動先の座標
	Vector2F targetPos_;

	//状態変更処理の管理
	std::unordered_map<MOVE_TYPE, std::function<void(GameScene&)>> processMoveMap_;

	//状態の処理を登録
	void RegisterProcessMove(const MOVE_TYPE moveType, std::function<void(GameScene&)> func);

	//状態変更処理
	inline void ChangeMoveType(const MOVE_TYPE moveType) { moveType_ = moveType; }

	//パラメーター設定
	void SetParameter() override;

	//更新処理
	void ProcessMoveUpdate(GameScene& parent)override;

	//移動操作処理
	void ProcessMoveNormal(GameScene& parent);	//通常操作
	void ProcessMoveMass(GameScene& parent);	//マスごとに移動
	void ProcessMoveNonstop(GameScene& parent);	//止まらない


};

