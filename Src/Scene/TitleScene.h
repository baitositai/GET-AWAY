#pragma once
#include "SceneBase.h"
#include <memory>

class Stage;

class TitleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void);

	//�ǂݍ��ݏ���
	void Load(void) override;

	//����������
	void Init(void) override;

private:

	//�^�C�g�����S
	int imgLogo_;

	// �X�e�[�W
	std::unique_ptr<Stage> stage_;

	//�X�V�֐�
	void NormalUpdate(void) override;

	//�`��֐�
	void NormalDraw(void) override;

	//�����̕ύX
	void ChangeNormal(void) override;
};
