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
	gameInfo.SetPlayerNum(1); //デバッグモードではプレイヤー人数を1人に設定
	gameInfo.SetPlayerUnitType(0, CHARA_TYPE::BAKU); //デバッグモードではプレイヤーのユニットをバクに設定
#endif

	//プレイヤーを生成
	player_ = std::make_unique<Player>(gameInfo.GetPlayerInfomation(0).charaType_, 0);
	player_->Load();

	//cpuを生成
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
	//プレイヤー初期化
	player_->Init();

	//ターゲット現在位置を取得(1Pの位置)
	Vector2 targetMapPos = Utility::WorldToMapPos(player_->GetPos(), { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE });
	//CPUの初期マップ座標を宣言
	std::vector<Vector2> firstMaps;
	firstMaps.push_back({16, 2});
	firstMaps.push_back({5, 5});
	firstMaps.push_back({23, 5});

	int i = 0;
	for (auto& cpu : cpus_)
	{
		//CPU初期化
		cpu->Init();

		//初期位置設定
		cpu->SetMapToWorldPos(firstMaps[i]);
		
		//ターゲット設定
		cpu->SetTargetaPos(targetMapPos);

		//インデックス更新
		i++;
	}

	//インターバルを設定
	reTargetIntervalStep_ = RE_SET_TARGET_INTERVAL_FPS;
}

void UnitManager::Update(GameScene& parent)
{
	player_->Update(parent);

	for (auto& cpu : cpus_)
	{
		cpu->Update(parent);
	}

	//CPUのターゲットを再設定
	ReSetTargetPos();

	//衝突判定
	CollisonPlayerToCpu();

	//CPUを削除
	
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
	//すべてのユニットの状態を変更する
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
	//ステップ更新
	reTargetIntervalStep_ -= SceneManager::GetInstance().GetDeltaTime();

	//20Fに1回処理を実行する
	//まだ条件を満たしていない場合
	if (reTargetIntervalStep_ > 0.0f)
	{
		//処理をやめる
		return;
	}

	//インターバルを再設定
	reTargetIntervalStep_ = RE_SET_TARGET_INTERVAL_FPS;

	//ターゲット現在位置を取得(1Pの位置)
	Vector2 targetMapPos = Utility::WorldToMapPos(player_->GetPos(), { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE });

	//CPUのターゲットへ設定
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
	//無敵時間を確認
	if (player_->IsInvincible())
	{
		//無敵の場合処理は行わない
		return;
	}

	//半径
	float radius = UnitBase::RADIUS / 2;	//通常より小さくする

	//CPU分処理を回す
	for (auto& cpu : cpus_)
	{
		if (cpu->IsDeath()) { continue; }
			
		if (Utility::IsHitCircles(player_->GetPos(), radius, cpu->GetPos(), radius))
		{
			//衝突したユニットらにダメージ
			player_->SetDamage(DEFAULT_DAMAGE);
			cpu->SetDamage(DEFAULT_DAMAGE);

			//衝突したら無敵になるのでこれ以上は処理を行わない
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
