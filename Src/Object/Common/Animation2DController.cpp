#include "Animation2DController.h"
#include <DxLib.h>

Animation2DController::Animation2DController()
{
	animIndex_ = -1;
	startIndex_ = -1;
	finishIndex_ = -1;
	animStep_ = 0.0f;
	animSpeed_ = 0.0f;
	isLoop_ = false;
	isPlay_ = false;
}

Animation2DController::~Animation2DController()
{
}

void Animation2DController::SetAnimParam(const int startIndex, const int finishIndex, const float animSpeed, const bool isLoop)
{
	//�ݒ肷��l���ς���ĂȂ��ꍇ
	if (startIndex_ == startIndex && 
		finishIndex_ == finishIndex && 
		animSpeed_ == animSpeed)
	{
		return; 
	}

	//�ݒ�
	startIndex_ = startIndex;
	finishIndex_ = finishIndex;
	animSpeed_ = animSpeed;
	isLoop_ = isLoop;

	//������
	animIndex_ = startIndex_;
}

void Animation2DController::Update()
{	
	//�A�j���[�V��������Đ��̏ꍇ
	if (!isPlay_) { return; }	
	
	//�A�j���[�V�����I�������[�v���s��Ȃ��ꍇ
	if (animIndex_ == finishIndex_ && !isLoop_)
	{
		isPlay_ = false;	//�Đ����Ȃ�
		animStep_ = 0.0f;	//������
		return;				//�����I��
	}

	//�X�e�b�v�X�V
	animStep_+= animSpeed_;

	//�A�j���[�V�����ő�l�����
	int animMax = finishIndex_ + 1 - startIndex_;

	//�A�j���[�V�����ݒ�
	animIndex_ = startIndex_ + static_cast<int>(animStep_) % animMax;
}