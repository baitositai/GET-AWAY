#include "StartSign.h"
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"

StartSign::StartSign()
{
    RegisterStateUpdate(STATE::READY, [this] {UpdateStateReady(); });
    RegisterStateUpdate(STATE::GO, [this] {UpdateStateGo(); });
    RegisterStateUpdate(STATE::FINISH, [this] {UpdateStateFinish(); });
    RegisterStateUpdate(STATE::NONE, [this] {UpdateStateNone(); });

    int i = -1;
	state_ = STATE::READY;
	img_ = &i;
	imgFinish_ = -1;
	imgIndex_ = -1;
	rate_ = 0.0f;
    pos_ = {};
}

StartSign::~StartSign()
{
}

void StartSign::Load()
{
    ResourceManager& res = ResourceManager::GetInstance();
    img_ = res.Load(ResourceManager::SRC::GAME_MESS).handleIds_;
}

void StartSign::Init()
{
    //初期座標
    pos_ = {
        Application::SCREEN_SIZE_X + ResourceManager::GAME_START_MESS_SIZE_X / 2,
        Application::SCREEN_HALF_Y
    };

    //初期拡大率
    rate_ = 1.0f;

    //初期表示画像
    imgIndex_ = static_cast<int>(STATE::READY);
}

void StartSign::Update()
{
    stateMap_[state_]();
}

void StartSign::Draw()
{
    DrawRotaGraph(
        pos_.x,
        pos_.y,
        rate_,
        0.0f,
        img_[imgIndex_],
        true,
        false);
}

const bool StartSign::FinishSign() const
{
    return state_ == STATE::NONE;
}

void StartSign::ChangeFinishSign()
{
    //初期座標
    pos_ = {
        Application::SCREEN_SIZE_X + ResourceManager::GAME_START_MESS_SIZE_X / 2,
        Application::SCREEN_HALF_Y
    };

    //初期拡大率
    rate_ = 1.0f;

    //初期表示画像
    imgIndex_ = static_cast<int>(STATE::FINISH);

	//状態変更
	ChangeState(STATE::FINISH);
}

void StartSign::RegisterStateUpdate(const STATE state, std::function<void()> func)
{
    stateMap_[state] = func;
}

void StartSign::UpdateStateReady()
{
    //移動
    pos_.x -= MOVE_SPEED;

    if (pos_.x <= -ResourceManager::GAME_START_MESS_SIZE_X / 2)
    {
        //状態遷移
        ChangeState(STATE::GO);

        //画像変更
        imgIndex_ = static_cast<int>(STATE::GO);

        //座標変更
        pos_ = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

        //拡大率設定
        rate_ = 0.0f;
    }
}

void StartSign::UpdateStateGo()
{
    //拡大
    rate_ += RATE_SPEED;

    if (rate_ >= 1.0f)
    {
        //拡大率設定
        rate_ = 1.0f;

        //待機ステップ更新
        waitStep_ += SceneManager::GetInstance().GetDeltaTime();
    }

    if (waitStep_ >= WAIT_TIME)
    {
        //状態変更
        ChangeState(STATE::NONE);
    }
}

void StartSign::UpdateStateFinish()
{
    //移動
    pos_.x -= MOVE_SPEED;

    if (pos_.x <= -ResourceManager::GAME_START_MESS_SIZE_X / 2)
    {
        //状態遷移
        ChangeState(STATE::NONE);
    }
}

void StartSign::UpdateStateNone()
{
}
