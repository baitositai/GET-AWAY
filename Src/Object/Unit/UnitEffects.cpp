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

	//ダメージ
	EffectInfo damageInfo = {};
	damageInfo.sprite = res.Load(ResourceManager::SRC::DAMAGE_EFE).handleIds_;
	damageInfo.controller = std::make_unique<Animation2DController>();
	effectMap_.try_emplace(EFFECT_TYPE::DAMAGE, std::move(damageInfo));

	//退出
	EffectInfo exitInfo = {};
	exitInfo.sprite = res.Load(ResourceManager::SRC::EXIT_EFE).handleIds_;
	exitInfo.controller = std::make_unique<Animation2DController>();
	effectMap_.try_emplace(EFFECT_TYPE::EXIT, std::move(exitInfo));

	//出現
	EffectInfo respawnInfo = {};
	respawnInfo.sprite = res.Load(ResourceManager::SRC::RESPAWN_EFE).handleIds_;
	respawnInfo.controller = std::make_unique<Animation2DController>();
	effectMap_.try_emplace(EFFECT_TYPE::RESPAWN, std::move(respawnInfo));
}

void UnitEffects::Init()
{
	//アニメーション数を設定
	effectMap_[EFFECT_TYPE::DAMAGE].animMax = ResourceManager::DAMAGE_EFE_DIV_X * ResourceManager::DAMAGE_EFE_DIV_Y;
	effectMap_[EFFECT_TYPE::EXIT].animMax = ResourceManager::EXIT_EFE_DIV_X * ResourceManager::EXIT_EFE_DIV_Y;
	effectMap_[EFFECT_TYPE::RESPAWN].animMax = ResourceManager::RESPAWN_EFE_DIV_X * ResourceManager::RESPAWN_EFE_DIV_Y;

	//速度設定
	effectMap_[EFFECT_TYPE::DAMAGE].speed = DAMAGE_EFE_SPEED;
	effectMap_[EFFECT_TYPE::EXIT].speed = EXIT_EFE_SPEED;
	effectMap_[EFFECT_TYPE::RESPAWN].speed = RESPAWN_EFE_SPEED;

	//初期アニメーション設定
	effectMap_[EFFECT_TYPE::DAMAGE].controller->SetAnimParam(0, effectMap_[EFFECT_TYPE::DAMAGE].animMax, effectMap_[EFFECT_TYPE::DAMAGE].speed);
	effectMap_[EFFECT_TYPE::EXIT].controller->SetAnimParam(0, effectMap_[EFFECT_TYPE::EXIT].animMax, effectMap_[EFFECT_TYPE::EXIT].speed);
	effectMap_[EFFECT_TYPE::RESPAWN].controller->SetAnimParam(0, effectMap_[EFFECT_TYPE::RESPAWN].animMax, effectMap_[EFFECT_TYPE::RESPAWN].speed);
}

void UnitEffects::Update()
{
	//エフェクトアニメーションの更新
	for (auto it = effectMap_.begin(); it != effectMap_.end(); ++it)
	{
		it->second.controller->Update();
	}
}

void UnitEffects::Draw()
{
	for (auto it = effectMap_.begin(); it != effectMap_.end(); ++it)
	{
		//情報格納
		EffectInfo& info = it->second;

		//再生してない場合描画しない
		if (!info.controller->IsPlay()) { continue; }

		//アニメーションインデックス
		int animIndex = info.controller->GetAnmationIndex();

		//描画
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
	//アニメーション開始
	effectMap_[type].controller->PlayAnimation();

	//描画情報の格納
	effectMap_[type].pos = pos;
	effectMap_[type].rate = rate;
	effectMap_[type].angle = angle;
}