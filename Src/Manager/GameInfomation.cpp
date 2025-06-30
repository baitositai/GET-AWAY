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
	//�v���C���[�l�����`
	gameInfo_.playerNum_ = playerNum;

	//�T�C�Y���m��
	playerInfo_.resize(gameInfo_.playerNum_);
}

void GameInfomation::SetPlayerUnitType(const int playerNo, const CHARA_TYPE charaType)
{
	//�����̒l���͈͊O�̏ꍇ�̓G���[
	if (playerNo < 0 || playerNo >= playerInfo_.size())
	{
		//�G���[�����i��: ��O�𓊂���A���O���o�͂���Ȃǁj
		return; // �G���[�����͂����ł͍s��Ȃ�
	}
	//�v���C���[�̃��j�b�g�̎�ނ�ݒ�
	playerInfo_[playerNo].charaType_ = charaType;
}

const GameInfomation::PlayerInfo GameInfomation::GetPlayerInfomation(const int playerNo) const
{
	//�����̒l���͈͊O�̏ꍇ�̓G���[
	if (playerNo < 0 || playerNo >= playerInfo_.size())
	{
		//�G���[�����i��: ��O�𓊂���A���O���o�͂���Ȃǁj
		return PlayerInfo(); // ���PlayerInfomation��Ԃ�
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