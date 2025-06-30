#pragma once
#include "../../Common/Vector2.h"

class Animation2DController
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Animation2DController();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Animation2DController();

	/// <summary>
	/// �A�j���[�V�����p�����[�^�[�̐ݒ�
	/// </summary>
	/// <param name="startIndex">�A�j���[�V�����X�^�[�g�C���f�b�N�X</param>
	/// <param name="finishIndex">�A�j���[�V�����I���C���f�b�N�X</param>
	/// <param name="animSpeed">�A�j���[�V�����X�s�[�h</param>
	/// <param name="isLoop">���[�v�Đ������邩</param>
	void SetAnimParam(const int startIndex, const int finishIndex, const float animSpeed, const bool isLoop = false);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();	
	
	/// <summary>
	/// �A�j���[�V�����̊J�n
	/// </summary>
	inline const void PlayAnimation() { isPlay_ = true; }

	/// <summary>
	/// �Đ�����
	/// </summary>
	/// <returns>�Đ����������Ԃ�</returns>
	inline const bool IsPlay()const { return isPlay_; }

	/// <summary>
	/// �A�j���[�V�����C���f�b�N�X��Ԃ�
	/// </summary>
	/// <returns>�A�j���[�V�����C���f�b�N�X</returns>
	inline const int GetAnmationIndex()const { return animIndex_; }

private:

	//�A�j���[�V�����J�n�C���f�b�N�X
	int startIndex_;

	//�A�j���[�V�����I���C���f�b�N�X
	int finishIndex_;

	//�A�j���[�V�����C���f�b�N�X
	int animIndex_;

	//�A�j���[�V�����X�e�b�v
	float animStep_;

	//�A�j���[�V�����X�s�[�h
	float animSpeed_;

	//���[�v���s����
	bool isLoop_;

	//�A�j���[�V�����Đ�
	bool isPlay_;

};

