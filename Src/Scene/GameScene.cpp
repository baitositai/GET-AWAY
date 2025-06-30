#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Utility/PathFinder.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Common/FontRegistry.h"
#include "../Object/Stage.h"
#include "../Object/Unit/UnitManager.h"
#include "../Object/Unit/UnitBase.h"
#include "../Object/System/StartSign.h"
#include "GameScene.h"

GameScene::GameScene()
{
	//移動操作処理の登録
	RegisterStateProcess(GAME_STATE::READY, [this]() {UpdateStateReady(); }, [this]() {DrawStateReady(); });
	RegisterStateProcess(GAME_STATE::PLAY, [this]() {UpdateStatePlay(); }, [this]() {DrawStatePlay(); });
	RegisterStateProcess(GAME_STATE::RESULT, [this]() {UpdateStateResult(); }, [this]() {DrawStateResult(); });

	//更新関数のセット
	updataFunc_ = std::bind(&GameScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&GameScene::LoadingDraw, this);
}

GameScene::~GameScene()
{
	//インスタンスの削除
}

void GameScene::Load()
{
	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::FONT_DOT.c_str(), FONT_SIZE, 0);

	//ステージ生成
	stage_ = std::make_unique<Stage>();
	stage_->Load();

	//経路探索
	pathFinder_ = std::make_unique<PathFinder>();

	//プレイヤー生成
	unitMng_ = std::make_unique<UnitManager>(*pathFinder_);
	unitMng_->Load();

	//スタート合図
	startSign_ = std::make_unique<StartSign>();
	startSign_->Load();
}

void GameScene::Init()
{
	//ステージ
	stage_->Init();

	//探索マップの設定
	pathFinder_->SetMap(stage_->GetMapData());

	//プレイヤー
	unitMng_->Init();

	//スタート合図
	startSign_->Init();

	//状態遷移
	ChangeState(GAME_STATE::READY);
}

bool GameScene::CheckPlayerMapCollision(const Vector2 playerPos)
{
	//マップサイズを用意
	Vector2 mapSize = { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE };

	//プレイヤーをマップ座標に変換
	Vector2 playerMapPos = Utility::WorldToMapPos(playerPos, mapSize);

	//プレイヤー位置のマップチップを調べる
	return stage_->IsCollidableMapChip(playerMapPos);
}

void GameScene::NormalUpdate()
{
	//デバッグ処理
	DebagUpdate();

	//状態別の処理
	stateUpdateMap_[state_]();

	//プレイヤーの更新
	unitMng_->Update(*this);
}

void GameScene::NormalDraw()
{
	//デバッグ処理
	DebagDraw();

	// ステージの描画
	stage_->Draw();

	//プレイヤーの描画
	unitMng_->Draw();

	//ステージの上書きの描画
	stage_->OverDraw();

	//状態別描画処理
	stateDrawMap_[state_]();
}

void GameScene::ChangeNormal()
{
	//処理変更
	updataFunc_ = std::bind(&GameScene::NormalUpdate, this);
	drawFunc_ = std::bind(&GameScene::NormalDraw, this);
}

void GameScene::RegisterStateProcess(const GAME_STATE state, std::function<void()> update, std::function<void()> draw)
{
	stateUpdateMap_[state] = update;
	stateDrawMap_[state] = draw;
}

void GameScene::UpdateStateReady()
{
	//合図処理
	startSign_->Update();

	//合図が終了したらゲーム開始
	if (startSign_->FinishSign())
	{
		//状態遷移
		ChangeState(GAME_STATE::PLAY);

		//ユニットを行動させる
		unitMng_->ChangeAllUnitState(UNIT_STATE::PLAY);
	}
}

void GameScene::UpdateStatePlay()
{
	//プレイヤーの死亡判定を調べる
	if (unitMng_->IsPlayerDeath())
	{
		//状態遷移
		ChangeState(GAME_STATE::RESULT);

		//終了サインに変更
		startSign_->ChangeFinishSign();
	}
}

void GameScene::UpdateStateResult()
{
	startSign_->Update();

	//合図が終了したらタイトルへ
	if (startSign_->FinishSign())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameScene::DrawStateReady()
{
	startSign_->Draw();
}

void GameScene::DrawStatePlay()
{
}

void GameScene::DrawStateResult()
{
	startSign_->Draw();
}

void GameScene::DebagUpdate()
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameScene::DebagDraw()
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		Utility::BLACK,
		true
	);

	DrawFormatString(
		0, 
		0,
		0x000000,
		L"GameScene"
	);
}
