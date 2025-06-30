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
	//�X�V�֐��̃Z�b�g
	updataFunc_ = std::bind(&TitleScene::LoadingUpdate, this);
	//�`��֐��̃Z�b�g
	drawFunc_ = std::bind(&TitleScene::LoadingDraw, this);

	imgLogo_ = -1;	//�^�C�g�����S�̉摜�n���h��
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Load(void)
{
	auto& res = ResourceManager::GetInstance();

	//�t�H���g�̓o�^
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::FONT_DOT.c_str(), FONT_SIZE, 0);

	//�摜
	imgLogo_ = res.Load(ResourceManager::SRC::GAME_LOGO).handleId_;

	//�X�e�[�W����
	stage_ = std::make_unique<Stage>();
	stage_->Load();
}

void TitleScene::Init(void)
{
	stage_->Init();
}

void TitleScene::NormalUpdate(void)
{	
	// �V�[���J��
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

	//�^�C�g�����S�̕`��
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
	//�����ύX
	updataFunc_ = std::bind(&TitleScene::NormalUpdate, this);
	drawFunc_ = std::bind(&TitleScene::NormalDraw, this);
}

