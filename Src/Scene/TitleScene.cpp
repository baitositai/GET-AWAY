#include "TitleScene.h"
#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Common/FontRegistry.h"
#include "../Object/Stage.h"	

TitleScene::TitleScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&TitleScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&TitleScene::LoadingDraw, this);

	imgLogo_ = -1;	//タイトルロゴの画像ハンドル
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Load(void)
{
	auto& res = ResourceManager::GetInstance();

	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::FONT_DOT.c_str(), FONT_SIZE, 0);

	//画像
	imgLogo_ = res.Load(ResourceManager::SRC::GAME_LOGO).handleId_;

	//ステージ生成
	stage_ = std::make_unique<Stage>();
	stage_->Load();
}

void TitleScene::Init(void)
{
	stage_->Init();
}

void TitleScene::NormalUpdate(void)
{	
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void TitleScene::NormalDraw(void)
{
	stage_->Draw();
	stage_->OverDraw();

	//タイトルロゴの描画
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		1.0f,
		0.0f,
		imgLogo_,
		true,
		false);
}

void TitleScene::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&TitleScene::NormalUpdate, this);
	drawFunc_ = std::bind(&TitleScene::NormalDraw, this);
}

