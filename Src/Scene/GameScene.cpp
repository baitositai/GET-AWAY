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
	//�ړ����쏈���̓o�^
	RegisterStateProcess(GAME_STATE::READY, [this]() {UpdateStateReady(); }, [this]() {DrawStateReady(); });
	RegisterStateProcess(GAME_STATE::PLAY, [this]() {UpdateStatePlay(); }, [this]() {DrawStatePlay(); });
	RegisterStateProcess(GAME_STATE::RESULT, [this]() {UpdateStateResult(); }, [this]() {DrawStateResult(); });

	//�X�V�֐��̃Z�b�g
	updataFunc_ = std::bind(&GameScene::LoadingUpdate, this);
	//�`��֐��̃Z�b�g
	drawFunc_ = std::bind(&GameScene::LoadingDraw, this);
}

GameScene::~GameScene()
{
	//�C���X�^���X�̍폜
}

void GameScene::Load()
{
	//�t�H���g�̓o�^
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::FONT_DOT.c_str(), FONT_SIZE, 0);

	//�X�e�[�W����
	stage_ = std::make_unique<Stage>();
	stage_->Load();

	//�o�H�T��
	pathFinder_ = std::make_unique<PathFinder>();

	//�v���C���[����
	unitMng_ = std::make_unique<UnitManager>(*pathFinder_);
	unitMng_->Load();

	//�X�^�[�g���}
	startSign_ = std::make_unique<StartSign>();
	startSign_->Load();
}

void GameScene::Init()
{
	//�X�e�[�W
	stage_->Init();

	//�T���}�b�v�̐ݒ�
	pathFinder_->SetMap(stage_->GetMapData());

	//�v���C���[
	unitMng_->Init();

	//�X�^�[�g���}
	startSign_->Init();

	//��ԑJ��
	ChangeState(GAME_STATE::READY);
}

bool GameScene::CheckPlayerMapCollision(const Vector2 playerPos)
{
	//�}�b�v�T�C�Y��p��
	Vector2 mapSize = { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE };

	//�v���C���[���}�b�v���W�ɕϊ�
	Vector2 playerMapPos = Utility::WorldToMapPos(playerPos, mapSize);

	//�v���C���[�ʒu�̃}�b�v�`�b�v�𒲂ׂ�
	return stage_->IsCollidableMapChip(playerMapPos);
}

void GameScene::NormalUpdate()
{
	//�f�o�b�O����
	DebagUpdate();

	//��ԕʂ̏���
	stateUpdateMap_[state_]();

	//�v���C���[�̍X�V
	unitMng_->Update(*this);
}

void GameScene::NormalDraw()
{
	//�f�o�b�O����
	DebagDraw();

	// �X�e�[�W�̕`��
	stage_->Draw();

	//�v���C���[�̕`��
	unitMng_->Draw();

	//�X�e�[�W�̏㏑���̕`��
	stage_->OverDraw();

	//��ԕʕ`�揈��
	stateDrawMap_[state_]();
}

void GameScene::ChangeNormal()
{
	//�����ύX
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
	//���}����
	startSign_->Update();

	//���}���I��������Q�[���J�n
	if (startSign_->FinishSign())
	{
		//��ԑJ��
		ChangeState(GAME_STATE::PLAY);

		//���j�b�g���s��������
		unitMng_->ChangeAllUnitState(UNIT_STATE::PLAY);
	}
}

void GameScene::UpdateStatePlay()
{
	//�v���C���[�̎��S����𒲂ׂ�
	if (unitMng_->IsPlayerDeath())
	{
		//��ԑJ��
		ChangeState(GAME_STATE::RESULT);

		//�I���T�C���ɕύX
		startSign_->ChangeFinishSign();
	}
}

void GameScene::UpdateStateResult()
{
	startSign_->Update();

	//���}���I��������^�C�g����
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
	// �V�[���J��
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
