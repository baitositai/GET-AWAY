#include "UnitManager.h"
#include "../../Utility/Utility.h"
#include "../../Utility/PathFinder.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/GameInfomation.h"
#include "../Stage.h"
#include "UnitBase.h"
#include "CpuPlayer.h"
#include "Player.h"
#include "UnitEffects.h"

enum class PATH_FIND_TYPE;

UnitManager::UnitManager(PathFinder& pathFinder) : pathFinder_(pathFinder)
{
	player_ = nullptr;
	cpus_.clear();
	reTargetIntervalStep_ = 0;
}

UnitManager::~UnitManager()
{
}

void UnitManager::Load()
{
	GameInfomation& gameInfo = GameInfomation::GetInstance();

#ifdef _DEBUG
	gameInfo.SetPlayerNum(1); //�f�o�b�O���[�h�ł̓v���C���[�l����1�l�ɐݒ�
	gameInfo.SetPlayerUnitType(0, CHARA_TYPE::BAKU); //�f�o�b�O���[�h�ł̓v���C���[�̃��j�b�g���o�N�ɐݒ�
#endif

	//�v���C���[�𐶐�
	player_ = std::make_unique<Player>(gameInfo.GetPlayerInfomation(0).charaType_, 0);
	player_->Load();

	//cpu�𐶐�
	int cpuMakeCount = 3;
	for (int i = 0; i < cpuMakeCount; i++)
	{
		auto cpu = std::make_unique<CpuPlayer>(GetRandomCharaType(), pathFinder_, static_cast<PATH_FIND_TYPE>(i));
		cpu->Load();
		cpus_.push_back(std::move(cpu));
	}
}

void UnitManager::Init()
{
	//�v���C���[������
	player_->Init();

	//�^�[�Q�b�g���݈ʒu���擾(1P�̈ʒu)
	Vector2 targetMapPos = Utility::WorldToMapPos(player_->GetPos(), { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE });
	//CPU�̏����}�b�v���W��錾
	std::vector<Vector2> firstMaps;
	firstMaps.push_back({16, 2});
	firstMaps.push_back({5, 5});
	firstMaps.push_back({23, 5});

	int i = 0;
	for (auto& cpu : cpus_)
	{
		//CPU������
		cpu->Init();

		//�����ʒu�ݒ�
		cpu->SetMapToWorldPos(firstMaps[i]);
		
		//�^�[�Q�b�g�ݒ�
		cpu->SetTargetaPos(targetMapPos);

		//�C���f�b�N�X�X�V
		i++;
	}

	//�C���^�[�o����ݒ�
	reTargetIntervalStep_ = RE_SET_TARGET_INTERVAL_FPS;
}

void UnitManager::Update(GameScene& parent)
{
	player_->Update(parent);

	for (auto& cpu : cpus_)
	{
		cpu->Update(parent);
	}

	//CPU�̃^�[�Q�b�g���Đݒ�
	ReSetTargetPos();

	//�Փ˔���
	CollisonPlayerToCpu();

	//CPU���폜
	
}

void UnitManager::Draw()
{
	player_->Draw();

	for (auto& cpu : cpus_)
	{
		cpu->Draw();
	}
}

void UnitManager::ChangeAllUnitState(const UNIT_STATE state)
{
	//���ׂẴ��j�b�g�̏�Ԃ�ύX����
	player_->ChangeState(state);

	for (auto& cpu : cpus_)
	{
		cpu->ChangeState(state);
	}
}

const bool UnitManager::IsPlayerDeath() const
{
	return player_->IsDeath();
}

void UnitManager::ReSetTargetPos()
{
	//�X�e�b�v�X�V
	reTargetIntervalStep_ -= SceneManager::GetInstance().GetDeltaTime();

	//20F��1�񏈗������s����
	//�܂������𖞂����Ă��Ȃ��ꍇ
	if (reTargetIntervalStep_ > 0.0f)
	{
		//��������߂�
		return;
	}

	//�C���^�[�o�����Đݒ�
	reTargetIntervalStep_ = RE_SET_TARGET_INTERVAL_FPS;

	//�^�[�Q�b�g���݈ʒu���擾(1P�̈ʒu)
	Vector2 targetMapPos = Utility::WorldToMapPos(player_->GetPos(), { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE });

	//CPU�̃^�[�Q�b�g�֐ݒ�
	for (auto& cpu : cpus_)
	{
		cpu->SetTargetaPos(targetMapPos);
	}
}

CHARA_TYPE UnitManager::GetRandomCharaType(void) const
{
	int random = GetRand(static_cast<int>(CHARA_TYPE::MAX) - 1);
	return static_cast<CHARA_TYPE>(random);
}

void UnitManager::CollisonPlayerToCpu()
{
	//���G���Ԃ��m�F
	if (player_->IsInvincible())
	{
		//���G�̏ꍇ�����͍s��Ȃ�
		return;
	}

	//���a
	float radius = UnitBase::RADIUS / 2;	//�ʏ��菬��������

	//CPU����������
	for (auto& cpu : cpus_)
	{
		if (cpu->IsDeath()) { continue; }
			
		if (Utility::IsHitCircles(player_->GetPos(), radius, cpu->GetPos(), radius))
		{
			//�Փ˂������j�b�g��Ƀ_���[�W
			player_->SetDamage(DEFAULT_DAMAGE);
			cpu->SetDamage(DEFAULT_DAMAGE);

			//�Փ˂����疳�G�ɂȂ�̂ł���ȏ�͏������s��Ȃ�
			break;
		}
	}
}

void UnitManager::DeleteCpu()
{

}

void UnitManager::CreateCpu()
{
}
