#pragma once
#include <memory>
#include <unordered_map>
#include "SceneBase.h"
#include "../Common/Vector2.h"

class Stage;
class UnitManager;
class PathFinder;	
class StartSign;

/// <summary>
/// 状態
/// </summary>
enum class GAME_STATE
{
	READY,
	PLAY,
	RESULT,
};

class GameScene : public SceneBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// プレイヤーとマップチップの衝突判定
	/// </summary>
	/// <param name="playerPos">プレイヤー位置</param>
	/// <returns>判定結果</returns>
	bool CheckPlayerMapCollision(const Vector2 playerPos);

private:	

	//状態
	GAME_STATE state_;
	
	//ステージ
	std::unique_ptr<Stage> stage_;

	//プレイヤー管理クラス
	std::unique_ptr<UnitManager> unitMng_;

	//経路探索
	std::unique_ptr<PathFinder> pathFinder_;

	//スタート合図
	std::unique_ptr<StartSign> startSign_;

	//状態別処理の管理
	std::unordered_map<GAME_STATE, std::function<void()>> stateUpdateMap_;
	std::unordered_map<GAME_STATE, std::function<void()>> stateDrawMap_;

	//更新関数
	void NormalUpdate() override;

	//描画関数
	void NormalDraw() override;

	//処理の変更
	void ChangeNormal() override;
	
	//状態遷移
	inline const void ChangeState(const GAME_STATE state) { state_ = state; }
	
	//状態の処理を登録
	void RegisterStateProcess(const GAME_STATE state, std::function<void()> update, std::function<void() >draw);

	//状態別処理
	void UpdateStateReady();	//準備
	void UpdateStatePlay();		//プレイ
	void UpdateStateResult();	//リザルト

	//状態別描画
	void DrawStateReady();
	void DrawStatePlay();
	void DrawStateResult();

	//デバッグ処理
	void DebagUpdate();	
	void DebagDraw();
};