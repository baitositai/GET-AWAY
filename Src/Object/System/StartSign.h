#pragma once
#include <functional>
#include "../../Common/Vector2.h"
class StartSign
{
public:

	//移動速度
	static constexpr int MOVE_SPEED = 8;

	//拡大速度
	static constexpr float RATE_SPEED = 0.05f;

	//待機時間
	static constexpr float WAIT_TIME = 0.9f;

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		READY,
		GO,
		FINISH,
		NONE,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	StartSign();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StartSign();

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
	/// サインの終了を返す
	/// </summary>
	const bool FinishSign()const;

	/// <summary>
	/// 終了に変更
	/// </summary>
	void ChangeFinishSign();

private:

	//状態
	STATE state_;

	//画像
	int* img_;
	int imgFinish_;

	//画像用インデックス
	int imgIndex_;

	//拡大率
	float rate_;

	//待機ステップ
	float waitStep_;
	
	//座標
	Vector2 pos_;

	//状態別更新処理
	std::unordered_map<STATE, std::function<void()>> stateMap_;

	//状態別更新処理の登録
	void RegisterStateUpdate(const STATE state, std::function<void()> func);

	//状態遷移
	inline const void ChangeState(const STATE state) { state_ = state; }

	//状態別更新処理
	void UpdateStateReady();
	void UpdateStateGo();
	void UpdateStateFinish();
	void UpdateStateNone();

};

