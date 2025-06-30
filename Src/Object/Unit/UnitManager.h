#pragma once
#include <memory>
#include <vector>

class GameScene;
class Player;
class CpuPlayer;
class PathFinder;
enum class CHARA_TYPE;
enum class UNIT_STATE;

class UnitManager
{
public:

	//ターゲット再設定用インターバル
	static constexpr float RE_SET_TARGET_INTERVAL_FPS = 1.0f;

	//ダメージ量
	static constexpr int DEFAULT_DAMAGE = 1;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pathFinder">経路探索</param>
	UnitManager(PathFinder& pathFinder);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~UnitManager();

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
	/// <param name="parent">親インスタンス</param>
	void Update(GameScene& parent);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	///	全てのユニットのステート状態を変更
	/// </summary>
	/// <param name="state">変更先の状態</param>
	void ChangeAllUnitState(const UNIT_STATE state);

	/// <summary>
	/// プレイヤー死亡判定を返す
	/// </summary>
	/// <returns>死亡判定</returns>
	const bool IsPlayerDeath() const;

private:

	//ターゲット再設定インターバルステップ
	float reTargetIntervalStep_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//CPU配列
	std::vector<std::unique_ptr<CpuPlayer>> cpus_;

	//経路探索
	PathFinder& pathFinder_;

	//CPUのターゲットを再設定する
	void ReSetTargetPos();

	//キャラクターの種類をランダムで入手
	CHARA_TYPE GetRandomCharaType(void) const;	
	
	//プレイヤーとCPUの衝突判定
	void CollisonPlayerToCpu();

	//CPUを削除
	void DeleteCpu();

	//CPUを生成
	void CreateCpu();
};


