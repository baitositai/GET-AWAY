#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"

class GameScene;
class Animation2DController;
class UnitEffects;
enum class EFFECT_TYPE;

//ユニット種類
enum class CHARA_TYPE
{
	BAKU,
	REI,
	MAX
};

//ユニットのステート状態
enum class UNIT_STATE
{
	NONE,
	RESPWAN,
	PLAY,
	DIE,
};

//ユニットのコンディション状態
enum class UNIT_CONDITION
{
	NONE,
	DAMAGE,
};

class UnitBase
{
public:

	//半径
	static constexpr float RADIUS = 14.0f;

	//移動速度
	static constexpr float MOVE_SPEED = 3.0f;

	//アニメーション速度
	static constexpr float ANIM_SPEED = 0.1f;

	//拡大率
	static constexpr float RATE_SCALE = 1.2f;

	//無敵時間
	static constexpr float INVINCIBLE_TIME = 2.0f;

	//点滅用アルファ値
	static constexpr float BLINKING_ALPHA = 128.0f;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="charaType">キャラクター種類</param>
	UnitBase(const CHARA_TYPE charaType);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UnitBase();

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="parent">ゲームクラス</param>
	void Update(GameScene & parent);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage"ダメージ値</param>
	void SetDamage(const int damage);
	
	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="state">変更先の状態</param>
	inline void ChangeState(const UNIT_STATE state) { state_ = state; }

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="effectTYpe">エフェクト種類</param>
	void PlayEffect(const EFFECT_TYPE effectType);

	/// <summary>
	/// 座標を返す
	/// </summary>
	/// <returns>座標</returns>
	inline const Vector2 GetPos() { return pos_.ToVector2(); }

	/// <summary>
	/// 死亡判定を返す
	/// </summary>
	/// <returns>死亡判定</returns>
	inline const bool IsDeath()const { return state_ == UNIT_STATE::DIE; }

	/// <summary>
	/// 無敵かどうか判定
	/// </summary>
	/// <returns>無敵時間が0以上の場合true返す</returns>
	inline const bool IsInvincible()const { return invincibleCnt_ > 0.0f; }

protected:

	//アニメーション関連
	static constexpr int ANIM_STOP_INDEX = 0;		//停止状態のインデックス
	static constexpr int ANIM_RUN_INDEX = 1;		//走行状態のインデックス
	static constexpr int ANIM_RUNS = 2;				//走行状態のアニメーション数
	static constexpr int ANIM_ADD_INDEX = 3;		//アニメーションのオフセット

	//画像
	int* imgChara_;

	//ユニットカラー
	int unitColor_;

	//ライフ
	int life_;

	//座標
	Vector2F pos_;

	//移動量
	Vector2 movePow_;

	//アニメーションスピード
	float animSpeed_;

	//アニメーション開始インデックス
	//アニメーション終了インデックス
	int animStartIndex_;
	int animFinishIndex_;

	//向き反転
	bool dir_;

	//エフェクト
	std::unique_ptr<UnitEffects> effects_;

private:	

	//描画アルファ値
	int drawAlpha_;

	//無敵時間用カウント
	float invincibleCnt_;
	
	//ユニットの種類
	CHARA_TYPE charaType_;	

	//ステート状態
	UNIT_STATE state_;

	//コンディション状態
	UNIT_CONDITION condition_;

	//2Dアニメーションコントローラー
	std::unique_ptr<Animation2DController> animController_;

	//ステート状態ごとの処理管理
	std::unordered_map<UNIT_STATE, std::function<void(GameScene&)>> stateMap_;

	//コンディション状態ごとの処理管理
	std::unordered_map<UNIT_CONDITION, std::function<void()>> conditionMap_;

	//状態の処理を登録
	void RegisterStateUpdate(const UNIT_STATE state, std::function<void(GameScene&)> func);
	void RegisterCondition(const UNIT_CONDITION condition, std::function<void()> func);

	//キャラクターのリソースを読み込む
	void CharacterResourceLoad();

	//状態別更新処理
	void UpdateStateNone(GameScene&);		//なし
	void UpdateStatePlay(GameScene&);		//プレイ
	void UpdateStateRespawn(GameScene&);	//リスポーン
	void UpdateStateDie(GameScene&);		//死亡

	//パラメータ等の設定
	virtual void SetParameter() = 0;

	//アニメーション初期化
	void InitAnimation();

	//移動の更新
	virtual void ProcessMoveUpdate(GameScene& parent) = 0;

	//アニメーションの更新
	void AnimationUpdate();

	//コンディション状態
	inline void const ChangeCondition(const UNIT_CONDITION condition) { condition_ = condition; }
	void ConditionNone();
	void ConditionDamage();

};