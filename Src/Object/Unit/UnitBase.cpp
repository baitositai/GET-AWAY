#include <DxLib.h>
#include "UnitBase.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/GameInfomation.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/Utility.h"
#include "../Common/Animation2DController.h"
#include "UnitEffects.h"

UnitBase::UnitBase(const CHARA_TYPE charaType) : charaType_(charaType)
{
	//�ړ����쏈���̓o�^
	RegisterStateUpdate(UNIT_STATE::NONE, [this](GameScene& parent) {UpdateStateNone(parent); });
	RegisterStateUpdate(UNIT_STATE::RESPWAN, [this](GameScene& parent) {UpdateStateRespawn(parent); });
	RegisterStateUpdate(UNIT_STATE::PLAY, [this](GameScene& parent) {UpdateStatePlay(parent); });
	RegisterStateUpdate(UNIT_STATE::DIE, [this](GameScene& parent) {UpdateStateDie(parent); });

	RegisterCondition(UNIT_CONDITION::NONE, [this]() {ConditionNone(); });
	RegisterCondition(UNIT_CONDITION::DAMAGE, [this]() {ConditionDamage(); });

	//�ϐ�������
	int i = -1;
	imgChara_ = &i;
	pos_ = {};
	unitColor_ = -1;
	dir_ = false;
	animController_ = nullptr;
	effects_ = nullptr;
	animStartIndex_ = 0;
	animFinishIndex_ = 0;
	invincibleCnt_ = 0.0f;
	drawAlpha_ = 0;
	condition_ = UNIT_CONDITION::NONE;
}

UnitBase::~UnitBase()
{
}

void UnitBase::Load()
{
	//�L�����N�^�[�̃��\�[�X��ǂݍ���
	CharacterResourceLoad();	
	
	//�A�j���[�V�����R���g���[���[
	animController_ = std::make_unique<Animation2DController>();

	//�G�t�F�N�g
	effects_ = std::make_unique<UnitEffects>();
	effects_->Load();
}

void UnitBase::Init()
{
	//�p�����[�^�[�ݒ�
	SetParameter();

	//�A�j���[�V����������
	InitAnimation();

	//�������
	ChangeState(UNIT_STATE::RESPWAN);

	//�G�t�F�N�g������
	effects_->Init();

	//�G�t�F�N�g�Đ�
	effects_->PlayEffect(EFFECT_TYPE::RESPAWN, pos_.ToVector2());

	//�A�j���[�V�������Đ�
	animController_->PlayAnimation();

	//�A���t�@�l��ݒ�
	drawAlpha_ = Utility::ALPHA_MAX;
}

void UnitBase::Update(GameScene& parent)
{
	stateMap_[state_](parent);

	//�G�t�F�N�g�X�V
	effects_->Update();
}

void UnitBase::Draw()
{
	Vector2 pos = pos_.ToVector2();

	//���j�b�g�`��
	DrawCircle(pos.x, pos.y, RADIUS, unitColor_);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, drawAlpha_);
	DrawRotaGraph(
		pos.x, pos.y,
		RATE_SCALE,
		0.0f,
		imgChara_[animController_->GetAnmationIndex()],
		true,
		dir_);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�G�t�F�N�g�`��
	effects_->Draw();
}

void UnitBase::SetDamage(const int damage)
{
	//�̗͂����Z
	life_ -= damage;

	//�c�ʊm�F
	if (life_ <= 0)
	{
		//��ԑJ��
		ChangeState(UNIT_STATE::DIE);

		//�G�t�F�N�g�Đ�
		PlayEffect(EFFECT_TYPE::EXIT);
	}
	else
	{
		//�G�t�F�N�g�Đ�
		PlayEffect(EFFECT_TYPE::DAMAGE);

		//��莞�Ԗ��G�ݒ�
		invincibleCnt_ = INVINCIBLE_TIME;

		//�R���f�B�V�������_���[�W�ɐݒ�
		ChangeCondition(UNIT_CONDITION::DAMAGE);
	}
}

void UnitBase::PlayEffect(const EFFECT_TYPE effectType)
{
	effects_->PlayEffect(effectType,pos_.ToVector2());
}

void UnitBase::RegisterStateUpdate(const UNIT_STATE state, std::function<void(GameScene&)> func)
{
	stateMap_[state] = func;
}

void UnitBase::RegisterCondition(const UNIT_CONDITION condition, std::function<void()> func)
{
	conditionMap_[condition] = func;
}

void UnitBase::CharacterResourceLoad()
{
	ResourceManager& res = ResourceManager::GetInstance();
	
	switch (charaType_)
	{
	case CHARA_TYPE::BAKU:
		imgChara_ = res.Load(ResourceManager::SRC::BAKU).handleIds_;
		break;

	case CHARA_TYPE::REI:
		imgChara_ = res.Load(ResourceManager::SRC::REI).handleIds_;
		break;

	default:
		break;
	};
}

void UnitBase::UpdateStateNone(GameScene& parent)
{
}

void UnitBase::UpdateStatePlay(GameScene& parent)
{
	//�ړ�����
	ProcessMoveUpdate(parent);

	//�A�j���[�V�����X�V
	AnimationUpdate();

	//�R���f�B�V�������Ƃ̍X�V����
	conditionMap_[condition_]();
}

void UnitBase::UpdateStateRespawn(GameScene& parent)
{
	effects_->SetEffectParam(EFFECT_TYPE::RESPAWN, pos_.ToVector2());
}

void UnitBase::UpdateStateDie(GameScene& parent)
{
	//�t�F�[�h�A�E�g����
	constexpr int ALPHA_SPEED = 20;
	drawAlpha_ -= ALPHA_SPEED;
	//�Œ�
	if (drawAlpha_ <= 0) { drawAlpha_ = 0; }
}

void UnitBase::InitAnimation()
{
	animSpeed_ = ANIM_SPEED;	//�A�j���[�V�������x
	dir_ = false;				//�����͍���������

	//�A�j���[�V�����R���g���[���[�̐ݒ�
	animController_->SetAnimParam(0, 0, animSpeed_);
}

void UnitBase::AnimationUpdate()
{
	//�A�j���[�V�����p�ϐ�
	int anims = ANIM_RUNS;	//�A�j���[�V������	
	
	//�ړ��ʂ��Ȃ��ꍇ
	if (Vector2::IsSameVector2({ 0,0 }, movePow_))
	{
		//��~���
		anims = 0;
	}
	else
	{
		//�ړ��ʂŉ摜�𔽓]������
		if (movePow_.x < 0)
		{
			dir_ = false; //������
			animStartIndex_ = 0; //������
		}
		else if (movePow_.x > 0)
		{
			dir_ = true; //�E����
			animStartIndex_ = 0; //������
		}
		//Y�̈ړ��ʂŉ摜�̃C���f�b�N�X�𒲐�
		if (movePow_.y < 0)
		{
			animStartIndex_ = ANIM_ADD_INDEX; //�����
		}
		else if (movePow_.y > 0)
		{
			animStartIndex_ = 0; //������
		}
	}

	animFinishIndex_ = animStartIndex_ + anims;
	animController_->SetAnimParam(animStartIndex_, animFinishIndex_, animSpeed_, true);
	animController_->Update();
}

void UnitBase::ConditionNone()
{
	//���ʏ����Ȃ�
}

void UnitBase::ConditionDamage()
{
	//�_���[�W�G�t�F�N�g�̒Ǐ]
	effects_->SetEffectParam(EFFECT_TYPE::DAMAGE, pos_.ToVector2());

	//���G����
	invincibleCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	if (invincibleCnt_ <= 0.0f)
	{
		//�A���t�@�l��߂�
		drawAlpha_ = Utility::ALPHA_MAX;
		//�R���f�B�V�����ύX
		ChangeCondition(UNIT_CONDITION::NONE);
		return;
	}

	// ���G���̓_�ŊԊu�i�b�P�ʁj
	static constexpr float FLASH_INTERVAL = 0.5f;

	// ���G���̓_�Ŕ���p�X�P�[���W���i�b �� �t���[���ϊ����s���{���j
	static constexpr float INVINCIBLE_FLASH_FRAME_SCALE = 10.0f;

	//�_�ŗp�A���t�@�l���v�Z
	int phase = static_cast<int>(invincibleCnt_ * INVINCIBLE_FLASH_FRAME_SCALE / FLASH_INTERVAL) % 2;
	drawAlpha_ = (phase == 0) ? BLINKING_ALPHA : Utility::ALPHA_MAX;

}