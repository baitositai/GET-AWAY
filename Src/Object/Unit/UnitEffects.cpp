#include "UnitEffects.h"
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../Common/Animation2DController.h"

UnitEffects::UnitEffects()
{
}

UnitEffects::~UnitEffects()
{
}

void UnitEffects::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();

	//�_���[�W
	EffectInfo damageInfo = {};
	damageInfo.sprite = res.Load(ResourceManager::SRC::DAMAGE_EFE).handleIds_;
	damageInfo.controller = std::make_unique<Animation2DController>();
	effectMap_.try_emplace(EFFECT_TYPE::DAMAGE, std::move(damageInfo));

	//�ޏo
	EffectInfo exitInfo = {};
	exitInfo.sprite = res.Load(ResourceManager::SRC::EXIT_EFE).handleIds_;
	exitInfo.controller = std::make_unique<Animation2DController>();
	effectMap_.try_emplace(EFFECT_TYPE::EXIT, std::move(exitInfo));

	//�o��
	EffectInfo respawnInfo = {};
	respawnInfo.sprite = res.Load(ResourceManager::SRC::RESPAWN_EFE).handleIds_;
	respawnInfo.controller = std::make_unique<Animation2DController>();
	effectMap_.try_emplace(EFFECT_TYPE::RESPAWN, std::move(respawnInfo));
}

void UnitEffects::Init()
{
	//�A�j���[�V��������ݒ�
	effectMap_[EFFECT_TYPE::DAMAGE].animMax = ResourceManager::DAMAGE_EFE_DIV_X * ResourceManager::DAMAGE_EFE_DIV_Y;
	effectMap_[EFFECT_TYPE::EXIT].animMax = ResourceManager::EXIT_EFE_DIV_X * ResourceManager::EXIT_EFE_DIV_Y;
	effectMap_[EFFECT_TYPE::RESPAWN].animMax = ResourceManager::RESPAWN_EFE_DIV_X * ResourceManager::RESPAWN_EFE_DIV_Y;

	//���x�ݒ�
	effectMap_[EFFECT_TYPE::DAMAGE].speed = DAMAGE_EFE_SPEED;
	effectMap_[EFFECT_TYPE::EXIT].speed = EXIT_EFE_SPEED;
	effectMap_[EFFECT_TYPE::RESPAWN].speed = RESPAWN_EFE_SPEED;

	//�����A�j���[�V�����ݒ�
	effectMap_[EFFECT_TYPE::DAMAGE].controller->SetAnimParam(0, effectMap_[EFFECT_TYPE::DAMAGE].animMax, effectMap_[EFFECT_TYPE::DAMAGE].speed);
	effectMap_[EFFECT_TYPE::EXIT].controller->SetAnimParam(0, effectMap_[EFFECT_TYPE::EXIT].animMax, effectMap_[EFFECT_TYPE::EXIT].speed);
	effectMap_[EFFECT_TYPE::RESPAWN].controller->SetAnimParam(0, effectMap_[EFFECT_TYPE::RESPAWN].animMax, effectMap_[EFFECT_TYPE::RESPAWN].speed);
}

void UnitEffects::Update()
{
	//�G�t�F�N�g�A�j���[�V�����̍X�V
	for (auto it = effectMap_.begin(); it != effectMap_.end(); ++it)
	{
		it->second.controller->Update();
	}
}

void UnitEffects::Draw()
{
	for (auto it = effectMap_.begin(); it != effectMap_.end(); ++it)
	{
		//���i�[
		EffectInfo& info = it->second;

		//�Đ����ĂȂ��ꍇ�`�悵�Ȃ�
		if (!info.controller->IsPlay()) { continue; }

		//�A�j���[�V�����C���f�b�N�X
		int animIndex = info.controller->GetAnmationIndex();

		//�`��
		DrawRotaGraph(
			info.pos.x,
			info.pos.y,
			info.rate,
			info.angle,
			info.sprite[animIndex],
			true,
			false
		);
	}
}

void UnitEffects::SetEffectParam(const EFFECT_TYPE type, const Vector2 pos, const float rate, const float angle)
{
	effectMap_[type].pos = pos;
	effectMap_[type].rate = rate;
	effectMap_[type].angle = angle;
}

void UnitEffects::PlayEffect(const EFFECT_TYPE type, const Vector2 pos, const float rate, const float angle)
{
	//�A�j���[�V�����J�n
	effectMap_[type].controller->PlayAnimation();

	//�`����̊i�[
	effectMap_[type].pos = pos;
	effectMap_[type].rate = rate;
	effectMap_[type].angle = angle;
}