#pragma once
#include <memory>
#include <unordered_map>
#include "../../Common/Vector2.h"

class Animation2DController;	

/// <summary>
/// エフェクト種類
/// </summary>
enum class EFFECT_TYPE
{
	DAMAGE,
	EXIT,
	RESPAWN,
	MAX
};

class UnitEffects
{
public:

	//アニメーション速度
	static constexpr float DAMAGE_EFE_SPEED = 0.5f;	//ダメージ
	static constexpr float EXIT_EFE_SPEED = 0.5f;	//退出
	static constexpr float RESPAWN_EFE_SPEED = 0.5f;//出現

	//デフォルトエフェクト拡大率
	static constexpr float DEFAULT_RATE = 1.3f;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	UnitEffects();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~UnitEffects();

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
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// エフェクト情報の設定
	/// </summary>
	/// <param name="type">エフェクト種類</param>
	/// <param name="pos">座標</param>
	/// <param name="rate">拡大率</param>
	/// <param name="angle">角度</param>
	void SetEffectParam(const EFFECT_TYPE type, const Vector2 pos, const float rate = DEFAULT_RATE, const float angle = 0.0f);

	/// <summary>
	/// エフェクト再生
	/// </summary>
	/// <param name="type">エフェクト種類</param>
	/// <param name="pos">座標</param>
	/// <param name="rate">拡大率</param>
	/// <param name="angle">角度</param>
	void PlayEffect(const EFFECT_TYPE type, const Vector2 pos, const float rate = DEFAULT_RATE, const float angle = 0.0f);

private:

	/// <summary>
	/// エフェクト情報
	/// </summary>
	struct EffectInfo
	{
		//コントローラー
		std::unique_ptr<Animation2DController> controller;

		//アニメーションスプライト
		int* sprite;

		//描画位置
		Vector2 pos;

		//拡大率
		float rate;

		//角度
		float angle;

		//速度
		float speed;

		//アニメーション最大数
		int animMax;
	};

	//種類ごとにエフェクト情報を格納
	std::unordered_map<EFFECT_TYPE, EffectInfo>effectMap_;

};

