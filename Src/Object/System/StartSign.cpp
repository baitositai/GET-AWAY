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
    //�������W
    pos_ = {
        Application::SCREEN_SIZE_X + ResourceManager::GAME_START_MESS_SIZE_X / 2,
        Application::SCREEN_HALF_Y
    };

    //�����g�嗦
    rate_ = 1.0f;

    //�����\���摜
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
    //�������W
    pos_ = {
        Application::SCREEN_SIZE_X + ResourceManager::GAME_START_MESS_SIZE_X / 2,
        Application::SCREEN_HALF_Y
    };

    //�����g�嗦
    rate_ = 1.0f;

    //�����\���摜
    imgIndex_ = static_cast<int>(STATE::FINISH);

	//��ԕύX
	ChangeState(STATE::FINISH);
}

void StartSign::RegisterStateUpdate(const STATE state, std::function<void()> func)
{
    stateMap_[state] = func;
}

void StartSign::UpdateStateReady()
{
    //�ړ�
    pos_.x -= MOVE_SPEED;

    if (pos_.x <= -ResourceManager::GAME_START_MESS_SIZE_X / 2)
    {
        //��ԑJ��
        ChangeState(STATE::GO);

        //�摜�ύX
        imgIndex_ = static_cast<int>(STATE::GO);

        //���W�ύX
        pos_ = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

        //�g�嗦�ݒ�
        rate_ = 0.0f;
    }
}

void StartSign::UpdateStateGo()
{
    //�g��
    rate_ += RATE_SPEED;

    if (rate_ >= 1.0f)
    {
        //�g�嗦�ݒ�
        rate_ = 1.0f;

        //�ҋ@�X�e�b�v�X�V
        waitStep_ += SceneManager::GetInstance().GetDeltaTime();
    }

    if (waitStep_ >= WAIT_TIME)
    {
        //��ԕύX
        ChangeState(STATE::NONE);
    }
}

void StartSign::UpdateStateFinish()
{
    //�ړ�
    pos_.x -= MOVE_SPEED;

    if (pos_.x <= -ResourceManager::GAME_START_MESS_SIZE_X / 2)
    {
        //��ԑJ��
        ChangeState(STATE::NONE);
    }
}

void StartSign::UpdateStateNone()
{
}
