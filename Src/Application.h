#pragma once
#include <string>
#include <memory>

class FpsControl;
class FontRegistry;

class Application
{

public:

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1024;
	static constexpr int SCREEN_SIZE_Y = 640;

	// スクリーンサイズハーフ
	static constexpr int SCREEN_HALF_X = SCREEN_SIZE_X / 2;
	static constexpr int SCREEN_HALF_Y = SCREEN_SIZE_Y / 2;

	// データパス関連
	//-------------------------------------------
	static const std::wstring PATH_IMAGE;
	static const std::wstring PATH_MODEL;
	static const std::wstring PATH_EFFECT;
	static const std::wstring PATH_SOUND;
	static const std::wstring PATH_FONT;
	static const std::wstring PATH_TEXT;
	static const std::wstring PATH_JSON;
	static const std::wstring PATH_CSV;
	//-------------------------------------------

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);

	// ゲームループの開始
	void Run(void);

	// リソースの破棄
	void Destroy(void);

	// 初期化成功／失敗の判定
	bool IsInitFail(void) const;

	// 解放成功／失敗の判定
	bool IsReleaseFail(void) const;

private:

	// 静的インスタンス
	static Application* instance_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	//FPS
	std::unique_ptr<FpsControl> fps_;

	//フォント登録
	std::unique_ptr<FontRegistry> fontReg_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application(void);
	Application(const Application& manager) = default;
	~Application(void) = default;

	// Effekseerの初期化
	void InitEffekseer(void);

};