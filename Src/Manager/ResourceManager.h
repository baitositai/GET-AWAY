#pragma once
#include <memory>
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	//キャラクター画像
	static constexpr int CHARA_DIV_X = 3;
	static constexpr int CHARA_DIV_Y = 2;
	static constexpr int CHARA_SIZE = 32;

	//エフェクト画像
	static constexpr int DAMAGE_EFE_DIV_X = 5;
	static constexpr int DAMAGE_EFE_DIV_Y = 1;
	static constexpr int DAMAGE_EFE_SIZE = 64;

	static constexpr int RESPAWN_EFE_DIV_X = 14;
	static constexpr int RESPAWN_EFE_DIV_Y = 1;
	static constexpr int RESPAWN_EFE_SIZE = 64;

	static constexpr int EXIT_EFE_DIV_X = 15;
	static constexpr int EXIT_EFE_DIV_Y = 1;
	static constexpr int EXIT_EFE_SIZE = 64;

	//ゲーム開始メッセージ
	static constexpr int GAME_START_MESS_DIV_X = 1;
	static constexpr int GAME_START_MESS_DIV_Y = 2;
	static constexpr int GAME_START_MESS_SIZE_X = 512;
	static constexpr int GAME_START_MESS_SIZE_Y = 256;

	//ゲームメッセージ
	static constexpr int GAME_MESS_DIV_X = 1;
	static constexpr int GAME_MESS_DIV_Y = 3;
	static constexpr int GAME_MESS_SIZE_X = 512;
	static constexpr int GAME_MESS_SIZE_Y = 256;

	//マップチップ
	static constexpr int MAP_CHIPS_DIV_X = 8;
	static constexpr int MAP_CHIPS_DIV_Y = 292;
	static constexpr int MAP_CHIPS_SIZE = 32;


	// リソース名
	enum class SRC
	{
		//タイトル
		GAME_LOGO,

		//キャラクター
		BAKU,
		REI,

		//エフェクト
		DAMAGE_EFE,
		EXIT_EFE,
		RESPAWN_EFE,

		//UI
		GAME_MESS,

		//ステージ
		MAP_CHIPS
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
