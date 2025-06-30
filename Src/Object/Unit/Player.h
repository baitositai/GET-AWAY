#pragma once
#include "UnitBase.h"

class Player : public UnitBase
{
public:

	//�̗�
	static constexpr int DEFAULT_LIFE = 3;

	//�Փ˔��莞�ɒ��ׂ�ӏ�
	enum class CHECK_COL
	{
		RIGHT,	//�E
		LEFT,	//��
		CENTER,	//����
		MAX
	};

	//�ړ����
	enum class MOVE_TYPE
	{
		NONE,
		NORMAL,
		MASS,
		NONSTOP,
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="charaType">�L�����N�^�[���</param>
	/// <param name="playerNo">�v���C���[���ʔԍ�</param>
	Player(const CHARA_TYPE charaType, const int playerNo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player(void) override;

private:

	//�v���C���[���ʔԍ�
	int playerNo_;	

	//�ړ����
	MOVE_TYPE moveType_;

	//�ړ���̍��W
	Vector2F targetPos_;

	//��ԕύX�����̊Ǘ�
	std::unordered_map<MOVE_TYPE, std::function<void(GameScene&)>> processMoveMap_;

	//��Ԃ̏�����o�^
	void RegisterProcessMove(const MOVE_TYPE moveType, std::function<void(GameScene&)> func);

	//��ԕύX����
	inline void ChangeMoveType(const MOVE_TYPE moveType) { moveType_ = moveType; }

	//�p�����[�^�[�ݒ�
	void SetParameter() override;

	//�X�V����
	void ProcessMoveUpdate(GameScene& parent)override;

	//�ړ����쏈��
	void ProcessMoveNormal(GameScene& parent);	//�ʏ푀��
	void ProcessMoveMass(GameScene& parent);	//�}�X���ƂɈړ�
	void ProcessMoveNonstop(GameScene& parent);	//�~�܂�Ȃ�


};

