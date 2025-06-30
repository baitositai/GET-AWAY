#pragma once
#include <vector>

enum class CHARA_TYPE;
enum class STAGE_TYPE;

class GameInfomation
{
public:

	//ゲームシステムの情報を格納
	struct GameInfo
	{
		int playerNum_;			//プレイヤーの人数
		int cpuNum_;			//CPUの人数
		STAGE_TYPE stageType_;	//ステージの種類
	};

	//プレイヤーに関する情報を格納
	struct PlayerInfo
	{
		CHARA_TYPE charaType_;	//キャラクターの種類
	};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameInfomation();

	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// 静的インスタンスを取得
	/// </summary>
	/// <returns></returns>インスタンスを返す
	static GameInfomation& GetInstance();

	/// <summary>
	/// プレイヤー人数を設定
	/// </summary>
	/// <param name="playerNum"></param>
	void SetPlayerNum(const int playerNum);

	/// <summary>
	/// プレイヤーのユニットの種類を設定
	/// </summary>
	/// <param name="playerNo"></param>
	/// <param name="unitType"></param>
	void SetPlayerUnitType(const int playerNo, const CHARA_TYPE charaType);

	/// <summary>
	/// プレイヤーの情報を返す
	/// </summary>
	/// <param name="playerNo"></param>
	/// <returns></returns>
	const PlayerInfo GetPlayerInfomation(const int playerNo) const;

	/// <summary>
	/// ゲームのシステム情報を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline const GameInfo& GetGameInfo(void) const { return gameInfo_; }

	/// <summary>
	/// 解放
	/// </summary>
	void Destroy();

private:	

	//静的インスタンス
	static GameInfomation* instance_;

	//ゲームシステムの情報
	GameInfo gameInfo_;	

	//プレイヤーに関する情報を格納するベクター
	std::vector<PlayerInfo> playerInfo_;
	
	//コンストラクタ
	GameInfomation();

	//コピーコンストラクタも代入も一切呼び出さないようにする
	GameInfomation(const GameInfomation& app) = delete;

	//代入演算子も削除
	void operator = (const GameInfomation& app) = delete;

};

