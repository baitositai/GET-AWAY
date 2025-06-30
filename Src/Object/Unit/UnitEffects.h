#pragma once
#include <memory>
#include <unordered_map>
#include "../../Common/Vector2.h"

class Animation2DController;	

/// <summary>
/// �G�t�F�N�g���
/// </summary>
enum class EFFECT_TYPE
{
	DAMAGE,
	EXIT,
	RESPAWN,
	MAX
};

class UnitEffects
{
public:

	//�A�j���[�V�������x
	static constexpr float DAMAGE_EFE_SPEED = 0.5f;	//�_���[�W
	static constexpr float EXIT_EFE_SPEED = 0.5f;	//�ޏo
	static constexpr float RESPAWN_EFE_SPEED = 0.5f;//�o��

	//�f�t�H���g�G�t�F�N�g�g�嗦
	static constexpr float DEFAULT_RATE = 1.3f;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UnitEffects();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~UnitEffects();

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	void Load();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �G�t�F�N�g���̐ݒ�
	/// </summary>
	/// <param name="type">�G�t�F�N�g���</param>
	/// <param name="pos">���W</param>
	/// <param name="rate">�g�嗦</param>
	/// <param name="angle">�p�x</param>
	void SetEffectParam(const EFFECT_TYPE type, const Vector2 pos, const float rate = DEFAULT_RATE, const float angle = 0.0f);

	/// <summary>
	/// �G�t�F�N�g�Đ�
	/// </summary>
	/// <param name="type">�G�t�F�N�g���</param>
	/// <param name="pos">���W</param>
	/// <param name="rate">�g�嗦</param>
	/// <param name="angle">�p�x</param>
	void PlayEffect(const EFFECT_TYPE type, const Vector2 pos, const float rate = DEFAULT_RATE, const float angle = 0.0f);

private:

	/// <summary>
	/// �G�t�F�N�g���
	/// </summary>
	struct EffectInfo
	{
		//�R���g���[���[
		std::unique_ptr<Animation2DController> controller;

		//�A�j���[�V�����X�v���C�g
		int* sprite;

		//�`��ʒu
		Vector2 pos;

		//�g�嗦
		float rate;

		//�p�x
		float angle;

		//���x
		float speed;

		//�A�j���[�V�����ő吔
		int animMax;
	};

	//��ނ��ƂɃG�t�F�N�g�����i�[
	std::unordered_map<EFFECT_TYPE, EffectInfo>effectMap_;

};

