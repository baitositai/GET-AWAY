#include <DxLib.h>
#include <string>
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/Utility.h"
#include "SceneBase.h"

SceneBase::SceneBase(void) :
	resMng_(ResourceManager::GetInstance())
	,scnMng_(SceneManager::GetInstance())
	,inputMng_(InputManager::GetInstance())
{
	buttnFontHandle_ = -1;
	loadingTime_ = -1;
}

SceneBase::~SceneBase(void)
{
}

void SceneBase::Update(void)
{
	updataFunc_();
	return;
}

void SceneBase::Draw(void)
{
	drawFunc_();
	return;
}

void SceneBase::LoadingUpdate(void)
{
	bool loadTimeOver = Utility::IsTimeOver(loadingTime_, LOADING_TIME);

	//���[�h���������������f
	if (GetASyncLoadNum() == 0 && loadTimeOver)
	{
		//�񓯊������𖳌��ɂ���
		SetUseASyncLoadFlag(false);

		//����������
		Init();

		//�t�F�[�h�C���J�n
		scnMng_.StartFadeIn();

		//�ʏ�̏����ֈڂ�
		ChangeNormal();
	}
}

void SceneBase::LoadingDraw(void)
{
	//NowLoading�̕`��
	DrawNowLoading();
}

void SceneBase::DrawNowLoading(void)
{
	//���[�h��
	auto time = scnMng_.GetTotalTime();
	int count = static_cast<int>(time / COMMA_TIME);
	count %= COMMA_MAX_NUM;

	std::wstring loadStr = L"Now loading";
	std::wstring dotStr = L".";

	for (int i = 0; i < count; i++)
	{
		loadStr += dotStr;
	}
	DrawStringToHandle(LOADING_STRING_POS_X, LOADING_STRING_POS_Y, loadStr.c_str(), 0xffffff, buttnFontHandle_);

}
