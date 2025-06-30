#include "Player.h"
#include "../../Utility/Utility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/GameInfomation.h"
#include "../../Scene/GameScene.h"
#include "../Stage.h"
#include "UnitEffects.h"


namespace
{
	Vector2 mapChipSize = { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE };		//マップチップサイズ
	Vector2 localPos = { Stage::MAP_CHIP_SIZE / 2,Stage::MAP_CHIP_SIZE / 2 };	//相対座標
}

Player::Player(const CHARA_TYPE charaType, const int playerNo) :
	UnitBase(charaType),
	playerNo_(playerNo)
{	
	//移動操作処理の登録
	RegisterProcessMove(MOVE_TYPE::NORMAL, [this](GameScene& parent) {ProcessMoveNormal(parent); });
	RegisterProcessMove(MOVE_TYPE::MASS, [this](GameScene& parent) {ProcessMoveMass(parent); });
	RegisterProcessMove(MOVE_TYPE::NONSTOP, [this](GameScene& parent) {ProcessMoveNonstop(parent); });

	//初期化
	moveType_ = MOVE_TYPE::NONE;
	movePow_ = {};
	targetPos_ = {};
}

Player::~Player(void)
{
}

void Player::RegisterProcessMove(const MOVE_TYPE moveType, std::function<void(GameScene&)> func)
{
	//登録
	processMoveMap_[moveType] = func;
}

void Player::SetParameter()
{
	unitColor_ = Utility::BLUE;

	//初期座標
	Vector2 mapPos = { 3,16 };
	pos_ = Utility::MapToWorldPos(mapPos, { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE }).ToVector2F();
	pos_ = Vector2F::AddVector2F(pos_, { Stage::MAP_CHIP_SIZE / 2,Stage::MAP_CHIP_SIZE / 2 });

	//体力
	life_ = DEFAULT_LIFE;

	//状態変更
	ChangeMoveType(MOVE_TYPE::MASS);
}

void Player::ProcessMoveUpdate(GameScene& parent)
{
	//移動種類ごとの処理
	processMoveMap_[moveType_](parent);
}

void Player::ProcessMoveNormal(GameScene& parent)
{
	auto& input = InputManager::GetInstance();
	Vector2F move = {};
	Vector2F offSetPos = {};
	bool dir = false;	//false : X方向, true : Y方向

	//操作処理(4方向かつ,同時押しなし)
	if (input.IsNew(KEY_INPUT_RIGHT)) { move.x += MOVE_SPEED; offSetPos.x += RADIUS; }
	else if (input.IsNew(KEY_INPUT_LEFT)) { move.x -= MOVE_SPEED; offSetPos.x -= RADIUS; }
	else if (input.IsNew(KEY_INPUT_DOWN)) { move.y += MOVE_SPEED; offSetPos.y += RADIUS; dir = true; }
	else if (input.IsNew(KEY_INPUT_UP)) { move.y -= MOVE_SPEED; offSetPos.y -= RADIUS; dir = true; }

	//入力確認（移動量を確認できなかった場合）
	if (!(move.x != 0.0f || move.y != 0.0f))
	{
		return;	//処理をやめる
	}

	//入力方向ごとにさらに位置を調整
	Vector2F range = {};
	if (!dir) { range.y = RADIUS; }
	else { range.x = RADIUS; }

	//衝突判定を調べる
	bool isCollision = false;
	for (int i = -1; i < (int)CHECK_COL::MAX - 1; i++)
	{
		//座標を調整する
		Vector2F checkPos = {
			pos_.x + move.x + offSetPos.x + i * range.x,
			pos_.y + move.y + offSetPos.y + i * range.y };

		if (parent.CheckPlayerMapCollision(checkPos.ToVector2()))
		{
			isCollision = true;
			break;
		}
	}

	//衝突してない場合移動する
	if (!isCollision) { pos_ = Vector2F::AddVector2F(pos_, move); }
}

void Player::ProcessMoveMass(GameScene& parent)
{
	auto& input = InputManager::GetInstance();

	//操作処理(4方向かつ,同時押しなし)
	if (movePow_.x == 0 && movePow_.y == 0)	//移動中は無視
	{
		if (input.IsNew(KEY_INPUT_RIGHT)) { 
			movePow_ = { 1,0 };}
		else if (input.IsNew(KEY_INPUT_LEFT)) { movePow_ = { -1,0 };}
		else if (input.IsNew(KEY_INPUT_DOWN)) { movePow_ = { 0,1 }; }
		else if (input.IsNew(KEY_INPUT_UP)) { movePow_ = { 0, -1 }; }
		else { movePow_ = { 0,0 }; }

		//入力確認（移動量を確認できなかった場合）
		if (movePow_.x == 0 && movePow_.y == 0)
		{
			return;	//処理をやめる
		}

		//移動先の衝突判定
		//座標を調整する
		Vector2F checkPos = {
			pos_.x + movePow_.x * mapChipSize.x,
			pos_.y + movePow_.y * mapChipSize.y };
		//衝突確認
		if (parent.CheckPlayerMapCollision(checkPos.ToVector2()))
		{
			movePow_ = { 0,0 };	//初期化
			return;//処理終了
		}

		//現在位置のマップ座標を取得
		Vector2 mapPos = Utility::WorldToMapPos(pos_.ToVector2(), mapChipSize);

		//移動先の座標を設定
		targetPos_ = { 
			static_cast<float>((mapPos.x + movePow_.x) * mapChipSize.x) + localPos.x,
			static_cast<float>((mapPos.y + movePow_.y) * mapChipSize.y) + localPos.y };
	}

	//移動処理
	if((movePow_.x != 0 || movePow_.y != 0) && 
		!Vector2::IsSameVector2(targetPos_.ToVector2(), {0,0}))
	{
		//ターゲットに向かって移動
		if (std::abs(pos_.x - targetPos_.x) > MOVE_SPEED)
		{
			pos_.x += static_cast<float>(MOVE_SPEED * movePow_.x);
		}
		else
		{
			pos_.x = targetPos_.x;
		}
		if (std::abs(pos_.y - targetPos_.y) > MOVE_SPEED)
		{
			pos_.y += static_cast<float>(MOVE_SPEED * movePow_.y);
		}
		else
		{
			pos_.y = targetPos_.y;
		}
		// ターゲットに到達したら停止
		if (Vector2::IsSameVector2(pos_.ToVector2(), targetPos_.ToVector2()))
		{
			movePow_ = { 0,0 };
		}
	}
	
}

void Player::ProcessMoveNonstop(GameScene& parent)
{
	auto& input = InputManager::GetInstance();

	//操作処理(4方向かつ,同時押しなし)
	if (movePow_.x == 0 && movePow_.y == 0)	//移動中は無視
	{
		if (input.IsNew(KEY_INPUT_RIGHT)) { movePow_ = { 1,0 }; }
		else if (input.IsNew(KEY_INPUT_LEFT)) { movePow_ = { -1,0 }; }
		else if (input.IsNew(KEY_INPUT_DOWN)) { movePow_ = { 0,1 }; }
		else if (input.IsNew(KEY_INPUT_UP)) { movePow_ = { 0, -1 }; }

		//入力確認（移動量を確認できなかった場合）
		if (movePow_.x == 0 && movePow_.y == 0)
		{
			return;	//処理をやめる
		}

		//移動先の衝突判定
		//座標を調整する
		Vector2F checkPos = {
			pos_.x + movePow_.x * mapChipSize.x,
			pos_.y + movePow_.y * mapChipSize.y };
		//衝突確認
		if (parent.CheckPlayerMapCollision(checkPos.ToVector2()))
		{
			//処理終了
			return;
		}

		//現在位置のマップ座標を取得
		Vector2 mapPos = Utility::WorldToMapPos(pos_.ToVector2(), mapChipSize);

		//移動先の座標を設定
		targetPos_ = {
			static_cast<float>((mapPos.x + movePow_.x) * mapChipSize.x) + localPos.x,
			static_cast<float>((mapPos.y + movePow_.y) * mapChipSize.y) + localPos.y };
	}

	//移動処理
	if (movePow_.x != 0 || movePow_.y != 0)
	{
		//ターゲットに向かって移動
		if (std::abs(pos_.x - targetPos_.x) > MOVE_SPEED)
		{
			pos_.x += static_cast<float>(MOVE_SPEED * movePow_.x);
		}
		else
		{
			pos_.x = targetPos_.x;
		}
		if (std::abs(pos_.y - targetPos_.y) > MOVE_SPEED)
		{
			pos_.y += static_cast<float>(MOVE_SPEED * movePow_.y);
		}
		else
		{
			pos_.y = targetPos_.y;
		}
		// ターゲットに到達したら停止
		if (Vector2::IsSameVector2(pos_.ToVector2(), targetPos_.ToVector2()))
		{
			movePow_ = { 0,0 };
		}
	}
}