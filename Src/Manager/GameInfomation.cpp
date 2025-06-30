#include "GameInfomation.h"
#include "../Object/Unit/UnitBase.h"
#include "../Object/Unit/Player.h"

GameInfomation* GameInfomation::instance_ = nullptr;

GameInfomation::~GameInfomation()
{
}

void GameInfomation::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new GameInfomation();
	}
}

GameInfomation& GameInfomation::GetInstance(void)
{
	return *instance_;
}

void GameInfomation::SetPlayerNum(const int playerNum)
{
	//プレイヤー人数を定義
	gameInfo_.playerNum_ = playerNum;

	//サイズを確定
	playerInfo_.resize(gameInfo_.playerNum_);
}

void GameInfomation::SetPlayerUnitType(const int playerNo, const CHARA_TYPE charaType)
{
	//引数の値が範囲外の場合はエラー
	if (playerNo < 0 || playerNo >= playerInfo_.size())
	{
		//エラー処理（例: 例外を投げる、ログを出力するなど）
		return; // エラー処理はここでは行わない
	}
	//プレイヤーのユニットの種類を設定
	playerInfo_[playerNo].charaType_ = charaType;
}

const GameInfomation::PlayerInfo GameInfomation::GetPlayerInfomation(const int playerNo) const
{
	//引数の値が範囲外の場合はエラー
	if (playerNo < 0 || playerNo >= playerInfo_.size())
	{
		//エラー処理（例: 例外を投げる、ログを出力するなど）
		return PlayerInfo(); // 空のPlayerInfomationを返す
	}

	return playerInfo_[playerNo];
}

void GameInfomation::Destroy()
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

GameInfomation::GameInfomation()
{
	gameInfo_ = { };
	playerInfo_.clear();
}