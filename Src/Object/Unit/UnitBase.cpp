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
	//移動操作処理の登録
	RegisterStateUpdate(UNIT_STATE::NONE, [this](GameScene& parent) {UpdateStateNone(parent); });
	RegisterStateUpdate(UNIT_STATE::RESPWAN, [this](GameScene& parent) {UpdateStateRespawn(parent); });
	RegisterStateUpdate(UNIT_STATE::PLAY, [this](GameScene& parent) {UpdateStatePlay(parent); });
	RegisterStateUpdate(UNIT_STATE::DIE, [this](GameScene& parent) {UpdateStateDie(parent); });

	RegisterCondition(UNIT_CONDITION::NONE, [this]() {ConditionNone(); });
	RegisterCondition(UNIT_CONDITION::DAMAGE, [this]() {ConditionDamage(); });

	//変数初期化
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
	//キャラクターのリソースを読み込む
	CharacterResourceLoad();	
	
	//アニメーションコントローラー
	animController_ = std::make_unique<Animation2DController>();

	//エフェクト
	effects_ = std::make_unique<UnitEffects>();
	effects_->Load();
}

void UnitBase::Init()
{
	//パラメーター設定
	SetParameter();

	//アニメーション初期化
	InitAnimation();

	//初期状態
	ChangeState(UNIT_STATE::RESPWAN);

	//エフェクト初期化
	effects_->Init();

	//エフェクト再生
	effects_->PlayEffect(EFFECT_TYPE::RESPAWN, pos_.ToVector2());

	//アニメーションを再生
	animController_->PlayAnimation();

	//アルファ値を設定
	drawAlpha_ = Utility::ALPHA_MAX;
}

void UnitBase::Update(GameScene& parent)
{
	stateMap_[state_](parent);

	//エフェクト更新
	effects_->Update();
}

void UnitBase::Draw()
{
	Vector2 pos = pos_.ToVector2();

	//ユニット描画
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

	//エフェクト描画
	effects_->Draw();
}

void UnitBase::SetDamage(const int damage)
{
	//体力を減算
	life_ -= damage;

	//残量確認
	if (life_ <= 0)
	{
		//状態遷移
		ChangeState(UNIT_STATE::DIE);

		//エフェクト再生
		PlayEffect(EFFECT_TYPE::EXIT);
	}
	else
	{
		//エフェクト再生
		PlayEffect(EFFECT_TYPE::DAMAGE);

		//一定時間無敵設定
		invincibleCnt_ = INVINCIBLE_TIME;

		//コンディションをダメージに設定
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
	//移動処理
	ProcessMoveUpdate(parent);

	//アニメーション更新
	AnimationUpdate();

	//コンディションごとの更新処理
	conditionMap_[condition_]();
}

void UnitBase::UpdateStateRespawn(GameScene& parent)
{
	effects_->SetEffectParam(EFFECT_TYPE::RESPAWN, pos_.ToVector2());
}

void UnitBase::UpdateStateDie(GameScene& parent)
{
	//フェードアウトする
	constexpr int ALPHA_SPEED = 20;
	drawAlpha_ -= ALPHA_SPEED;
	//固定
	if (drawAlpha_ <= 0) { drawAlpha_ = 0; }
}

void UnitBase::InitAnimation()
{
	animSpeed_ = ANIM_SPEED;	//アニメーション速度
	dir_ = false;				//初期は左方向かつ

	//アニメーションコントローラーの設定
	animController_->SetAnimParam(0, 0, animSpeed_);
}

void UnitBase::AnimationUpdate()
{
	//アニメーション用変数
	int anims = ANIM_RUNS;	//アニメーション数	
	
	//移動量がない場合
	if (Vector2::IsSameVector2({ 0,0 }, movePow_))
	{
		//停止状態
		anims = 0;
	}
	else
	{
		//移動量で画像を反転させる
		if (movePow_.x < 0)
		{
			dir_ = false; //左方向
			animStartIndex_ = 0; //下方向
		}
		else if (movePow_.x > 0)
		{
			dir_ = true; //右方向
			animStartIndex_ = 0; //下方向
		}
		//Yの移動量で画像のインデックスを調整
		if (movePow_.y < 0)
		{
			animStartIndex_ = ANIM_ADD_INDEX; //上方向
		}
		else if (movePow_.y > 0)
		{
			animStartIndex_ = 0; //下方向
		}
	}

	animFinishIndex_ = animStartIndex_ + anims;
	animController_->SetAnimParam(animStartIndex_, animFinishIndex_, animSpeed_, true);
	animController_->Update();
}

void UnitBase::ConditionNone()
{
	//特別処理なし
}

void UnitBase::ConditionDamage()
{
	//ダメージエフェクトの追従
	effects_->SetEffectParam(EFFECT_TYPE::DAMAGE, pos_.ToVector2());

	//無敵時間
	invincibleCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	if (invincibleCnt_ <= 0.0f)
	{
		//アルファ値を戻す
		drawAlpha_ = Utility::ALPHA_MAX;
		//コンディション変更
		ChangeCondition(UNIT_CONDITION::NONE);
		return;
	}

	// 無敵時の点滅間隔（秒単位）
	static constexpr float FLASH_INTERVAL = 0.5f;

	// 無敵時の点滅判定用スケール係数（秒 → フレーム変換を行う倍率）
	static constexpr float INVINCIBLE_FLASH_FRAME_SCALE = 10.0f;

	//点滅用アルファ値を計算
	int phase = static_cast<int>(invincibleCnt_ * INVINCIBLE_FLASH_FRAME_SCALE / FLASH_INTERVAL) % 2;
	drawAlpha_ = (phase == 0) ? BLINKING_ALPHA : Utility::ALPHA_MAX;

}